#include "csvChirp.h"
#include "internal/common.h"
#include "channel.h"
#include "internal/csv.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stdio.h>

typedef enum qdc_CsvChirpCrossMode {
    qdc_CsvChirpCrossMode_NONE = 0,
    qdc_CsvChirpCrossMode_TONE = 1,
    qdc_CsvChirpCrossMode_DTCS = 2,
} qdc_CsvChirpCrossMode;

typedef enum qdc_CsvChirpToneMode {
    qdc_CsvChirpToneMode_NONE = 0,
    qdc_CsvChirpToneMode_TONE = 1,
    qdc_CsvChirpToneMode_TSQL = 2,
    qdc_CsvChirpToneMode_DTCS = 3,
    qdc_CsvChirpToneMode_DTCS_R = 4,
    qdc_CsvChirpToneMode_TSQL_R = 5,
    qdc_CsvChirpToneMode_CROSS = 6,
} qdc_CsvChirpToneMode;

typedef enum qdc_CsvChirpDuplex {
    qdc_CsvChirpDuplex_NONE = 0,
    qdc_CsvChirpDuplex_MINUS = 1,
    qdc_CsvChirpDuplex_PLUS = 2,
    qdc_CsvChirpDuplex_SPLIT = 3,
    qdc_CsvChirpDuplex_OFF = 4,
} qdc_CsvChirpDuplex;

/**
 * Some columns from Chirp CSV are inconvenient to parse imediatly.
 * Most comonly because one field in qdc_Channel coresponds to multiple columns
 * of the CSV which makes it mandatory to know values of all of the columns before
 * parsing can be performed.
 *
 * In such cases, values of these columns are saved here and processed at the fieldEnd,
 * when everything is known.
*/
typedef struct qdc_CsvChirpParserDefer {
    unsigned long location;
    qdc_CsvChirpDuplex duplex;
    double offset;
    float rToneFreq;
    float cToneFreq;
    unsigned int dtcsCode;
    unsigned int rxDtcsCode;
    bool isTxDtcsInverted;
    bool isRxDtcsInverted;
    qdc_CsvChirpToneMode toneMode;
    qdc_CsvChirpCrossMode crossModeTx;
    qdc_CsvChirpCrossMode crossModeRx;
} qdc_CsvChirpParserDefer;

typedef qdc_CsvChirpParseChannelErr qdc_CsvChirpParseFn(const char *value, unsigned long valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer);

struct qdc_CsvChirp {
    unsigned long columnCount;
    qdc_CsvChirpParseFn *columnParsers[];
};


qdc_CsvChirpParseChannelErr qdc_csvChirpParseIgnore(const char *value, unsigned long valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(value);
    qdc_unused(valueSize);
    qdc_unused(ch);
    qdc_unused(defer);
    return qdc_CsvChirpParseChannelErr_NONE;
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParseLocation(const char *value, unsigned long valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(defer);
    qdc_unused(ch);
    long location;
    if (qdc_strToLong(value, valueSize, &location) == qdc_strToNumberErr_INVALID_NUMBER_FORMAT) {
        return qdc_CsvChirpParseChannelErr_LOCATION_MUST_BE_INTEGER;
    }
    if (location < 0) {
        return qdc_CsvChirpParseChannelErr_LOCATION_MUST_BE_POSITIVE;
    }
    defer->location = location;
    return qdc_CsvChirpParseChannelErr_NONE;
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParseName(const char *value, unsigned long valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(defer);
    qdc_CsvChirpParseChannelErr result = qdc_CsvChirpParseChannelErr_NONE;
    if (valueSize > _qdc_CHANNEL_NAME_MAX_CHARACTERS) {
        result = qdc_CsvChirpParseChannelErr_NAME_TOO_LONG_AUTOFIXED;
        valueSize = _qdc_CHANNEL_NAME_MAX_CHARACTERS;
    }
    strncpy(ch->name, value, valueSize);

    ch->name[valueSize] = '\0';
    return result;
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParseFrequency(const char *value, unsigned long valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(defer);
    double frequency;
    switch (qdc_strToDouble(value, valueSize, &frequency)) {
        case qdc_strToNumberErr_INVALID_NUMBER_FORMAT:
            return qdc_CsvChirpParseChannelErr_FREQUENCY_MUST_BE_DECIMAL;
        default:
            ch->rxFrequency = frequency * 1000000;
            return qdc_CsvChirpParseChannelErr_NONE;
    }
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParseDuplex(const char *value, unsigned long valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(ch);
    if (qdc_csvAreFieldsEqual(value, valueSize, "-")) {
        defer->offset = qdc_CsvChirpDuplex_MINUS;
    } else if (qdc_csvAreFieldsEqual(value, valueSize, "+")) {
        defer->offset = qdc_CsvChirpDuplex_PLUS;
    } else if (qdc_csvAreFieldsEqual(value, valueSize, "split")) {
        defer->offset = qdc_CsvChirpDuplex_SPLIT;
    } else if (qdc_csvAreFieldsEqual(value, valueSize, "off")) {
        defer->offset = qdc_CsvChirpDuplex_OFF;
    } else {
        defer->offset = qdc_CsvChirpDuplex_NONE;
    }
    return qdc_CsvChirpParseChannelErr_NONE;
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParseOffset(const char *value, unsigned long valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(defer);
    double offset;
    switch (qdc_strToDouble(value, valueSize, &offset)) {
        case qdc_strToNumberErr_INVALID_NUMBER_FORMAT:
            return qdc_CsvChirpParseChannelErr_OFFSET_MUST_BE_DECIMAL;
        default:
            ch->txOffset = offset * 1000000;
            return qdc_CsvChirpParseChannelErr_NONE;
    }
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParseTone(const char *value, unsigned long valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(ch);
    if (qdc_csvAreFieldsEqual(value, valueSize, "Tone")) {
        defer->toneMode = qdc_CsvChirpToneMode_TONE;
    } else if (qdc_csvAreFieldsEqual(value, valueSize, "TSQL")) {
        defer->toneMode = qdc_CsvChirpToneMode_TSQL;
    } else if (qdc_csvAreFieldsEqual(value, valueSize, "DTCS")) {
        defer->toneMode = qdc_CsvChirpToneMode_DTCS;
    } else if (qdc_csvAreFieldsEqual(value, valueSize, "DTCS-R")) {
        defer->toneMode = qdc_CsvChirpToneMode_DTCS_R;
    } else if (qdc_csvAreFieldsEqual(value, valueSize, "TSQL-R")) {
        defer->toneMode = qdc_CsvChirpToneMode_TSQL_R;
    } else if (qdc_csvAreFieldsEqual(value, valueSize, "Cross")) {
        defer->toneMode = qdc_CsvChirpToneMode_CROSS;
    } else {
        defer->toneMode = qdc_CsvChirpToneMode_NONE;
    }
    return qdc_CsvChirpParseChannelErr_NONE;
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParseRToneFreq(const char *value, unsigned long valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(ch);
    float toneFreq;
    switch (qdc_strToFloat(value, valueSize, &toneFreq)) {
        case qdc_strToNumberErr_INVALID_NUMBER_FORMAT:
            return qdc_CsvChirpParseChannelErr_TONE_MUST_BE_DECIMAL;
        default:
            defer->rToneFreq = toneFreq;
            return qdc_CsvChirpParseChannelErr_NONE;
    }
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParseCToneFreq(const char *value, unsigned long valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(ch);
    float toneFreq;
    switch (qdc_strToFloat(value, valueSize, &toneFreq)) {
        case qdc_strToNumberErr_INVALID_NUMBER_FORMAT:
            return qdc_CsvChirpParseChannelErr_TONE_SQUELCH_MUST_BE_DECIMAL;
        default:
            defer->cToneFreq = toneFreq;
            return qdc_CsvChirpParseChannelErr_NONE;
    }
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParseDtscCode(const char *value, unsigned long valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(ch);
    long dtcs;
    switch (qdc_strToLong(value, valueSize, &dtcs)) {
        case qdc_strToNumberErr_INVALID_NUMBER_FORMAT:
            return qdc_CsvChirpParseChannelErr_DTCS_MUST_BE_INTEGER;
        default:
            defer->dtcsCode = dtcs;
            return qdc_CsvChirpParseChannelErr_NONE;
    }
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParseDtscPolarity(const char *value, unsigned long valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(ch);
    if (valueSize < sizeof(char) * 2) { return qdc_CsvChirpParseChannelErr_DTCS_POLARITY_FIELD_TOO_LONG; }
    defer->isTxDtcsInverted = value[0] == 'R';
    defer->isRxDtcsInverted = value[1] == 'R';
    return qdc_CsvChirpParseChannelErr_NONE;
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParseRxDtscCode(const char *value, unsigned long valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(ch);
    long dtcs;
    switch (qdc_strToLong(value, valueSize, &dtcs)) {
        case qdc_strToNumberErr_INVALID_NUMBER_FORMAT:
            return qdc_CsvChirpParseChannelErr_RX_DTCS_MUST_BE_INTEGER;
        default:
            defer->rxDtcsCode = dtcs;
            return qdc_CsvChirpParseChannelErr_NONE;
    }
}
qdc_CsvChirpCrossMode qdc_csvChirpParseCrossModeMap(const char *value, unsigned long valueSize) {
    if (qdc_csvAreFieldsEqual(value, valueSize, "Tone")) {
        return qdc_CsvChirpCrossMode_TONE;
    } else if (qdc_csvAreFieldsEqual(value, valueSize, "DTCS")) {
        return qdc_CsvChirpCrossMode_DTCS;
    } else {
        return qdc_CsvChirpCrossMode_NONE;
    }
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParseCrossMode(const char *value, unsigned long valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(ch);
    defer->crossModeTx = qdc_csvChirpParseCrossModeMap(value, valueSize);
    const char *rxPart = qdc_strFindNext(value, valueSize, '>');
    defer->crossModeRx = qdc_csvChirpParseCrossModeMap(rxPart, valueSize - (rxPart - value));
    return qdc_CsvChirpParseChannelErr_NONE;
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParseMode(const char *value, unsigned long valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(defer);
    if (qdc_csvAreFieldsEqual(value, valueSize, "NFM")) {
        ch->modulation = qdc_Modulation_FM;
        ch->bandwidth = qdc_Bandwidth_NARROW;
    } else if (qdc_csvAreFieldsEqual(value, valueSize, "AM")) {
        ch->modulation = qdc_Modulation_AM;
    } else if (qdc_csvAreFieldsEqual(value, valueSize, "USB")) {
        ch->modulation = qdc_Modulation_USB;
    } else {
        ch->modulation = qdc_Modulation_FM;
        ch->bandwidth = qdc_Bandwidth_WIDE;
    }
    return qdc_CsvChirpParseChannelErr_NONE;
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParseTStep(const char *value, unsigned long valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(defer);
    if (qdc_csvAreFieldsEqual(value, valueSize, "0.01")) ch->frequencyStep = qdc_FrequencyStep_0_01;
    else if (qdc_csvAreFieldsEqual(value, valueSize, "0.05")) ch->frequencyStep = qdc_FrequencyStep_0_05;
    else if (qdc_csvAreFieldsEqual(value, valueSize, "0.1")) ch->frequencyStep = qdc_FrequencyStep_0_1;
    else if (qdc_csvAreFieldsEqual(value, valueSize, "0.25")) ch->frequencyStep = qdc_FrequencyStep_0_25;
    else if (qdc_csvAreFieldsEqual(value, valueSize, "0.5")) ch->frequencyStep = qdc_FrequencyStep_0_5;
    else if (qdc_csvAreFieldsEqual(value, valueSize, "1")) ch->frequencyStep = qdc_FrequencyStep_1;
    else if (qdc_csvAreFieldsEqual(value, valueSize, "1.25")) ch->frequencyStep = qdc_FrequencyStep_1_25;
    else if (qdc_csvAreFieldsEqual(value, valueSize, "2.5")) ch->frequencyStep = qdc_FrequencyStep_2_5;
    else if (qdc_csvAreFieldsEqual(value, valueSize, "5")) ch->frequencyStep = qdc_FrequencyStep_5;
    else if (qdc_csvAreFieldsEqual(value, valueSize, "6.25")) ch->frequencyStep = qdc_FrequencyStep_6_25;
    else if (qdc_csvAreFieldsEqual(value, valueSize, "8.33")) ch->frequencyStep = qdc_FrequencyStep_8_33;
    else if (qdc_csvAreFieldsEqual(value, valueSize, "10")) ch->frequencyStep = qdc_FrequencyStep_10;
    else if (qdc_csvAreFieldsEqual(value, valueSize, "12.5")) ch->frequencyStep = qdc_FrequencyStep_12_5;
    else if (qdc_csvAreFieldsEqual(value, valueSize, "15")) ch->frequencyStep = qdc_FrequencyStep_15;
    else if (qdc_csvAreFieldsEqual(value, valueSize, "25")) ch->frequencyStep = qdc_FrequencyStep_25;
    else if (qdc_csvAreFieldsEqual(value, valueSize, "30")) ch->frequencyStep = qdc_FrequencyStep_30;
    else if (qdc_csvAreFieldsEqual(value, valueSize, "50")) ch->frequencyStep = qdc_FrequencyStep_50;
    else if (qdc_csvAreFieldsEqual(value, valueSize, "100")) ch->frequencyStep = qdc_FrequencyStep_100;
    else if (qdc_csvAreFieldsEqual(value, valueSize, "125")) ch->frequencyStep = qdc_FrequencyStep_125;
    else if (qdc_csvAreFieldsEqual(value, valueSize, "250")) ch->frequencyStep = qdc_FrequencyStep_250;
    else if (qdc_csvAreFieldsEqual(value, valueSize, "500")) ch->frequencyStep = qdc_FrequencyStep_500;
    else {
        ch->frequencyStep = qdc_FrequencyStep_5;
        return qdc_CsvChirpParseChannelErr_UNSUPPORTED_FREQUENCY_STEP_AUTOFIXED;
    }
    return qdc_CsvChirpParseChannelErr_NONE;
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParseSkip(const char *value, unsigned long valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(defer);
    ch->includeInScanList1 = !qdc_csvAreFieldsEqual(value, valueSize, "S");
    ch->includeInScanList2 = ch->includeInScanList1;
    return qdc_CsvChirpParseChannelErr_NONE;
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParsePower(const char *value, unsigned long valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(defer);
    float power;
    switch (qdc_strToFloat(value, valueSize, &power)) {
        case qdc_strToNumberErr_INVALID_NUMBER_FORMAT:
            return qdc_CsvChirpParseChannelErr_POWER_MUST_BE_DECIMAL;
        case qdc_strToNumberErr_NONE:
            if (power <= 2) {
                ch->outputPower = qdc_OutputPower_LOW;
            } else if (power > 2 && power <= 4) {
                ch->outputPower = qdc_OutputPower_MEDIUM;
            } else {
                ch->outputPower = qdc_OutputPower_HIGH;
            }
    }
    return qdc_CsvChirpParseChannelErr_NONE;
}

void qdc_csvChirpParseDefered(const qdc_CsvChirpParserDefer *defer, qdc_Channel *ch) {
    switch (defer->duplex) {
        case qdc_CsvChirpDuplex_MINUS:
            ch->offsetDirection = qdc_OffsetDirection_MINUS;
            ch->txOffset = defer->offset * 1000000;
            break;
        case qdc_CsvChirpDuplex_PLUS:
            ch->offsetDirection = qdc_OffsetDirection_PLUS;
            ch->txOffset = defer->offset * 1000000;
            break;
        case qdc_CsvChirpDuplex_SPLIT:;
            long long offset = ch->rxFrequency - defer->offset * 1000000;
            if (offset < 0) ch->offsetDirection = qdc_OffsetDirection_MINUS;
            else if (offset > 0) ch->offsetDirection = qdc_OffsetDirection_PLUS;
            else ch->offsetDirection = qdc_OffsetDirection_NONE;
            ch->txOffset = llabs(offset);
            break;
        default:
            ch->offsetDirection = qdc_OffsetDirection_NONE;
            break;
    }

    switch (defer->toneMode) {
        case qdc_CsvChirpToneMode_TONE:
            ch->txCode = qdc_parseCtcss(defer->rToneFreq);
            ch->rxCode = qdc_Code_NONE;
            break;
        case qdc_CsvChirpToneMode_TSQL:
            ch->txCode = qdc_parseCtcss(defer->cToneFreq);
            ch->rxCode = ch->txCode;
            break;
        case qdc_CsvChirpToneMode_DTCS:
            ch->txCode = qdc_parseDcs(defer->dtcsCode, defer->isTxDtcsInverted);
            ch->rxCode = qdc_parseDcs(defer->dtcsCode, defer->isRxDtcsInverted);
            break;
        case qdc_CsvChirpToneMode_DTCS_R:
            ch->txCode = qdc_Code_NONE;
            ch->rxCode = qdc_parseDcs(defer->dtcsCode, defer->isRxDtcsInverted);
            break;
        case qdc_CsvChirpToneMode_TSQL_R:
            ch->txCode = qdc_Code_NONE;
            ch->rxCode = qdc_parseCtcss(defer->rToneFreq);
            break;
        case qdc_CsvChirpToneMode_CROSS:
            switch (defer->crossModeTx) {
                case qdc_CsvChirpCrossMode_TONE:
                    ch->txCode = qdc_parseCtcss(defer->rToneFreq);
                    break;
                case qdc_CsvChirpCrossMode_DTCS:
                    ch->txCode = qdc_parseDcs(defer->dtcsCode, defer->isTxDtcsInverted);
                    break;
                case qdc_CsvChirpCrossMode_NONE:
                    ch->txCode = qdc_Code_NONE;
            }
            switch (defer->crossModeRx) {
                case qdc_CsvChirpCrossMode_TONE:
                    ch->rxCode = qdc_parseCtcss(defer->cToneFreq);
                    break;
                case qdc_CsvChirpCrossMode_DTCS:
                    ch->rxCode = qdc_parseDcs(defer->rxDtcsCode, defer->isRxDtcsInverted);
                    break;
                case qdc_CsvChirpCrossMode_NONE:
                    ch->rxCode = qdc_Code_NONE;
            }
            break;
        case qdc_CsvChirpToneMode_NONE:
            ch->txCode = qdc_Code_NONE;
            ch->rxCode = qdc_Code_NONE;
    }
}

void qdc_csvChirpParseHeader(qdc_CsvChirp *csv, const char *csvHeaderLine, unsigned long csvHeaderLen) {
    printf("Column COUNT: %d\n\n", (int)csv->columnCount);
    qdc_csvForEachColumn(csvHeaderLine, csvHeaderLen, csv->columnCount,
        if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "Location")) {
            csv->columnParsers[i] = qdc_csvChirpParseLocation;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "Name")) {
            csv->columnParsers[i] = qdc_csvChirpParseName;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "Frequency")) {
            csv->columnParsers[i] = qdc_csvChirpParseFrequency;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "Duplex")) {
            csv->columnParsers[i] = qdc_csvChirpParseDuplex;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "Offset")) {
            csv->columnParsers[i] = qdc_csvChirpParseOffset;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "Tone")) {
            csv->columnParsers[i] = qdc_csvChirpParseTone;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "rToneFreq")) {
            csv->columnParsers[i] = qdc_csvChirpParseRToneFreq;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "cToneFreq")) {
            csv->columnParsers[i] = qdc_csvChirpParseCToneFreq;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "DtcsCode")) {
            csv->columnParsers[i] = qdc_csvChirpParseDtscCode;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "DtcsPolarity")) {
            csv->columnParsers[i] = qdc_csvChirpParseDtscPolarity;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "RxDtcsCode")) {
            csv->columnParsers[i] = qdc_csvChirpParseRxDtscCode;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "CrossMode")) {
            csv->columnParsers[i] = qdc_csvChirpParseCrossMode;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "Mode")) {
            csv->columnParsers[i] = qdc_csvChirpParseMode;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "TStep")) {
            csv->columnParsers[i] = qdc_csvChirpParseTStep;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "Skip")) {
            csv->columnParsers[i] = qdc_csvChirpParseSkip;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "Power")) {
            csv->columnParsers[i] = qdc_csvChirpParsePower;
        } else {
            csv->columnParsers[i] = qdc_csvChirpParseIgnore;
        }
    )
}

qdc_CsvChirpParseChannelErr qdc_csvChirpParseChannel(const qdc_CsvChirp *csv, const char *csvLine, unsigned long csvLineLen, unsigned long *chIndex, qdc_Channel *ch) {
    qdc_CsvChirpParserDefer defer = {0};
    qdc_CsvChirpParseChannelErr result = qdc_CsvChirpParseChannelErr_NONE;

    qdc_csvForEachColumn(csvLine, csvLineLen, csv->columnCount,
        printf("Now doing column: %d with parser %d length: %d\n", (int)i, (int)(unsigned long)csv->columnParsers[i], (int)(fieldEnd - fieldStart - 1));
        result |= csv->columnParsers[i](fieldStart, fieldEnd - fieldStart - 1, ch, &defer);
    )

    printf("Doing defer\n");
    qdc_csvChirpParseDefered(&defer, ch);
    *chIndex = defer.location;
    printf("DOne\n");
    return result;
}

qdc_CsvChirp *qdc_csvChirpNew(const char *csvHeader, unsigned long headerLen) {
    unsigned long columnCount = qdc_csvGetColumnCount(csvHeader, headerLen);
    qdc_CsvChirp *csv = malloc(sizeof(qdc_CsvChirp) + sizeof(qdc_CsvChirpParseFn*) * columnCount);
    if (csv == NULL) return NULL;
    csv->columnCount = columnCount;
    qdc_csvChirpParseHeader(csv, csvHeader, headerLen);
    return csv;
}

void qdc_csvChirpDestroy(qdc_CsvChirp *csv) {
    free(csv);
}
