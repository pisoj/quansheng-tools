#include "csv.h"
#include "channel.h"
#include "internal/minmax.h"
#include "internal/string.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <stdio.h>

#define qdc_EPSILON 0.05f
#define qdc_csvAreFieldsEqual(csvStr, csvStrSize, strToCmp) !qdc_strCmpSafe(csvStr, csvStrSize, strToCmp, sizeof(strToCmp) - sizeof(char))
#define qdc_unused(x) (void)(x)

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

typedef qdc_CsvChirpParseChannelErr qdc_CsvChirpParserFn(const char *value, size_t valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer);

qdc_CsvChirpParseChannelErr qdc_csvChirpParseIgnore(const char *value, size_t valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(value);
    qdc_unused(valueSize);
    qdc_unused(ch);
    qdc_unused(defer);
    return qdc_CsvChirpParseChannelErr_NONE;
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParseName(const char *value, size_t valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(defer);
    qdc_CsvChirpParseChannelErr result = qdc_CsvChirpParseChannelErr_NONE;
    if (valueSize > _qdc_CHANNEL_NAME_MAX_CHARACTERS) {
        result = qdc_CsvChirpParseChannelErr_NAME_TOO_LONG_AUTOFIXED;
    }
    strncpy(ch->name, value, valueSize);
    ch->name[sizeof(ch->name) - sizeof(char)] = '\0';
    return result;
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParseFrequency(const char *value, size_t valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
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
qdc_CsvChirpParseChannelErr qdc_csvChirpParseDuplex(const char *value, size_t valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
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
qdc_CsvChirpParseChannelErr qdc_csvChirpParseOffset(const char *value, size_t valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
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
qdc_CsvChirpParseChannelErr qdc_csvChirpParseTone(const char *value, size_t valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
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
qdc_CsvChirpParseChannelErr qdc_csvChirpParseRToneFreq(const char *value, size_t valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
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
qdc_CsvChirpParseChannelErr qdc_csvChirpParseCToneFreq(const char *value, size_t valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
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
qdc_CsvChirpParseChannelErr qdc_csvChirpParseDtscCode(const char *value, size_t valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
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
qdc_CsvChirpParseChannelErr qdc_csvChirpParseDtscPolarity(const char *value, size_t valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(ch);
    if (valueSize < sizeof(char) * 2) { return qdc_CsvChirpParseChannelErr_DTCS_POLARITY_FIELD_TOO_LONG; }
    defer->isTxDtcsInverted = value[0] == 'R';
    defer->isRxDtcsInverted = value[1] == 'R';
    return qdc_CsvChirpParseChannelErr_NONE;
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParseRxDtscCode(const char *value, size_t valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
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
qdc_CsvChirpCrossMode qdc_csvChirpParseCrossModeMap(const char *value, size_t valueSize) {
    if (qdc_csvAreFieldsEqual(value, valueSize, "Tone")) {
        return qdc_CsvChirpCrossMode_TONE;
    } else if (qdc_csvAreFieldsEqual(value, valueSize, "DTCS")) {
        return qdc_CsvChirpCrossMode_DTCS;
    } else {
        return qdc_CsvChirpCrossMode_NONE;
    }
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParseCrossMode(const char *value, size_t valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(ch);
    defer->crossModeTx = qdc_csvChirpParseCrossModeMap(value, valueSize);
    const char *rxPart = qdc_strFindNext(value, valueSize, '>');
    defer->crossModeRx = qdc_csvChirpParseCrossModeMap(rxPart, valueSize - (rxPart - value));
    return qdc_CsvChirpParseChannelErr_NONE;
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParseMode(const char *value, size_t valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
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
qdc_CsvChirpParseChannelErr qdc_csvChirpParseTStep(const char *value, size_t valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
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
qdc_CsvChirpParseChannelErr qdc_csvChirpParseSkip(const char *value, size_t valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
    qdc_unused(defer);
    ch->includeInScanList1 = !qdc_csvAreFieldsEqual(value, valueSize, "S");
    ch->includeInScanList2 = ch->includeInScanList1;
    return qdc_CsvChirpParseChannelErr_NONE;
}
qdc_CsvChirpParseChannelErr qdc_csvChirpParsePower(const char *value, size_t valueSize, qdc_Channel *ch, qdc_CsvChirpParserDefer *defer) {
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


qdc_Code qdc_parseCtcss(float tone) {
    if (fabs(tone - 67.0f) < qdc_EPSILON) return qdc_Code_CTCSS_67;
    else if (fabs(tone - 69.3f) < qdc_EPSILON) return qdc_Code_CTCSS_69_3;
    else if (fabs(tone - 71.9f) < qdc_EPSILON) return qdc_Code_CTCSS_71_9;
    else if (fabs(tone - 74.4f) < qdc_EPSILON) return qdc_Code_CTCSS_74_4;
    else if (fabs(tone - 77.0f) < qdc_EPSILON) return qdc_Code_CTCSS_77;
    else if (fabs(tone - 79.7f) < qdc_EPSILON) return qdc_Code_CTCSS_79_7;
    else if (fabs(tone - 82.5f) < qdc_EPSILON) return qdc_Code_CTCSS_82_5;
    else if (fabs(tone - 85.4f) < qdc_EPSILON) return qdc_Code_CTCSS_85_4;
    else if (fabs(tone - 88.5f) < qdc_EPSILON) return qdc_Code_CTCSS_88_5;
    else if (fabs(tone - 91.5f) < qdc_EPSILON) return qdc_Code_CTCSS_91_5;
    else if (fabs(tone - 94.8f) < qdc_EPSILON) return qdc_Code_CTCSS_94_8;
    else if (fabs(tone - 97.4f) < qdc_EPSILON) return qdc_Code_CTCSS_97_4;
    else if (fabs(tone - 100.0f) < qdc_EPSILON) return qdc_Code_CTCSS_100;
    else if (fabs(tone - 103.5f) < qdc_EPSILON) return qdc_Code_CTCSS_103_5;
    else if (fabs(tone - 107.2f) < qdc_EPSILON) return qdc_Code_CTCSS_107_2;
    else if (fabs(tone - 110.9f) < qdc_EPSILON) return qdc_Code_CTCSS_110_9;
    else if (fabs(tone - 114.8f) < qdc_EPSILON) return qdc_Code_CTCSS_114_8;
    else if (fabs(tone - 118.8f) < qdc_EPSILON) return qdc_Code_CTCSS_118_8;
    else if (fabs(tone - 123.0f) < qdc_EPSILON) return qdc_Code_CTCSS_123;
    else if (fabs(tone - 127.3f) < qdc_EPSILON) return qdc_Code_CTCSS_127_3;
    else if (fabs(tone - 131.8f) < qdc_EPSILON) return qdc_Code_CTCSS_131_8;
    else if (fabs(tone - 136.5f) < qdc_EPSILON) return qdc_Code_CTCSS_136_5;
    else if (fabs(tone - 141.3f) < qdc_EPSILON) return qdc_Code_CTCSS_141_3;
    else if (fabs(tone - 146.2f) < qdc_EPSILON) return qdc_Code_CTCSS_146_2;
    else if (fabs(tone - 151.4f) < qdc_EPSILON) return qdc_Code_CTCSS_151_4;
    else if (fabs(tone - 156.7f) < qdc_EPSILON) return qdc_Code_CTCSS_156_7;
    else if (fabs(tone - 159.8f) < qdc_EPSILON) return qdc_Code_CTCSS_159_8;
    else if (fabs(tone - 162.2f) < qdc_EPSILON) return qdc_Code_CTCSS_162_2;
    else if (fabs(tone - 165.5f) < qdc_EPSILON) return qdc_Code_CTCSS_165_5;
    else if (fabs(tone - 167.9f) < qdc_EPSILON) return qdc_Code_CTCSS_167_9;
    else if (fabs(tone - 171.3f) < qdc_EPSILON) return qdc_Code_CTCSS_171_3;
    else if (fabs(tone - 173.8f) < qdc_EPSILON) return qdc_Code_CTCSS_173_8;
    else if (fabs(tone - 177.3f) < qdc_EPSILON) return qdc_Code_CTCSS_177_3;
    else if (fabs(tone - 179.9f) < qdc_EPSILON) return qdc_Code_CTCSS_179_9;
    else if (fabs(tone - 183.5f) < qdc_EPSILON) return qdc_Code_CTCSS_183_5;
    else if (fabs(tone - 186.2f) < qdc_EPSILON) return qdc_Code_CTCSS_186_2;
    else if (fabs(tone - 189.9f) < qdc_EPSILON) return qdc_Code_CTCSS_189_9;
    else if (fabs(tone - 192.8f) < qdc_EPSILON) return qdc_Code_CTCSS_192_8;
    else if (fabs(tone - 196.6f) < qdc_EPSILON) return qdc_Code_CTCSS_196_6;
    else if (fabs(tone - 199.5f) < qdc_EPSILON) return qdc_Code_CTCSS_199_5;
    else if (fabs(tone - 203.5f) < qdc_EPSILON) return qdc_Code_CTCSS_203_5;
    else if (fabs(tone - 206.5f) < qdc_EPSILON) return qdc_Code_CTCSS_206_5;
    else if (fabs(tone - 210.7f) < qdc_EPSILON) return qdc_Code_CTCSS_210_7;
    else if (fabs(tone - 218.1f) < qdc_EPSILON) return qdc_Code_CTCSS_218_1;
    else if (fabs(tone - 225.7f) < qdc_EPSILON) return qdc_Code_CTCSS_225_7;
    else if (fabs(tone - 229.1f) < qdc_EPSILON) return qdc_Code_CTCSS_229_1;
    else if (fabs(tone - 233.6f) < qdc_EPSILON) return qdc_Code_CTCSS_233_6;
    else if (fabs(tone - 241.8f) < qdc_EPSILON) return qdc_Code_CTCSS_241_8;
    else if (fabs(tone - 250.3f) < qdc_EPSILON) return qdc_Code_CTCSS_250_3;
    else if (fabs(tone - 254.1f) < qdc_EPSILON) return qdc_Code_CTCSS_254_1;
    return qdc_Code_NONE;
}

unsigned int qdc_parseDcsWithoutType(unsigned int code) {
    switch (code) {
        case 23: return _qdc_Code_NOTYPE_DCS_23;
        case 25: return _qdc_Code_NOTYPE_DCS_25;
        case 26: return _qdc_Code_NOTYPE_DCS_26;
        case 31: return _qdc_Code_NOTYPE_DCS_31;
        case 32: return _qdc_Code_NOTYPE_DCS_32;
        case 36: return _qdc_Code_NOTYPE_DCS_36;
        case 43: return _qdc_Code_NOTYPE_DCS_43;
        case 47: return _qdc_Code_NOTYPE_DCS_47;
        case 51: return _qdc_Code_NOTYPE_DCS_51;
        case 53: return _qdc_Code_NOTYPE_DCS_53;
        case 54: return _qdc_Code_NOTYPE_DCS_54;
        case 65: return _qdc_Code_NOTYPE_DCS_65;
        case 71: return _qdc_Code_NOTYPE_DCS_71;
        case 72: return _qdc_Code_NOTYPE_DCS_72;
        case 73: return _qdc_Code_NOTYPE_DCS_73;
        case 74: return _qdc_Code_NOTYPE_DCS_74;
        case 114: return _qdc_Code_NOTYPE_DCS_114;
        case 115: return _qdc_Code_NOTYPE_DCS_115;
        case 116: return _qdc_Code_NOTYPE_DCS_116;
        case 122: return _qdc_Code_NOTYPE_DCS_122;
        case 125: return _qdc_Code_NOTYPE_DCS_125;
        case 131: return _qdc_Code_NOTYPE_DCS_131;
        case 132: return _qdc_Code_NOTYPE_DCS_132;
        case 134: return _qdc_Code_NOTYPE_DCS_134;
        case 143: return _qdc_Code_NOTYPE_DCS_143;
        case 145: return _qdc_Code_NOTYPE_DCS_145;
        case 152: return _qdc_Code_NOTYPE_DCS_152;
        case 155: return _qdc_Code_NOTYPE_DCS_155;
        case 156: return _qdc_Code_NOTYPE_DCS_156;
        case 162: return _qdc_Code_NOTYPE_DCS_162;
        case 165: return _qdc_Code_NOTYPE_DCS_165;
        case 172: return _qdc_Code_NOTYPE_DCS_172;
        case 174: return _qdc_Code_NOTYPE_DCS_174;
        case 205: return _qdc_Code_NOTYPE_DCS_205;
        case 212: return _qdc_Code_NOTYPE_DCS_212;
        case 223: return _qdc_Code_NOTYPE_DCS_223;
        case 225: return _qdc_Code_NOTYPE_DCS_225;
        case 226: return _qdc_Code_NOTYPE_DCS_226;
        case 243: return _qdc_Code_NOTYPE_DCS_243;
        case 244: return _qdc_Code_NOTYPE_DCS_244;
        case 245: return _qdc_Code_NOTYPE_DCS_245;
        case 246: return _qdc_Code_NOTYPE_DCS_246;
        case 251: return _qdc_Code_NOTYPE_DCS_251;
        case 252: return _qdc_Code_NOTYPE_DCS_252;
        case 255: return _qdc_Code_NOTYPE_DCS_255;
        case 261: return _qdc_Code_NOTYPE_DCS_261;
        case 263: return _qdc_Code_NOTYPE_DCS_263;
        case 265: return _qdc_Code_NOTYPE_DCS_265;
        case 266: return _qdc_Code_NOTYPE_DCS_266;
        case 271: return _qdc_Code_NOTYPE_DCS_271;
        case 274: return _qdc_Code_NOTYPE_DCS_274;
        case 306: return _qdc_Code_NOTYPE_DCS_306;
        case 311: return _qdc_Code_NOTYPE_DCS_311;
        case 315: return _qdc_Code_NOTYPE_DCS_315;
        case 325: return _qdc_Code_NOTYPE_DCS_325;
        case 331: return _qdc_Code_NOTYPE_DCS_331;
        case 332: return _qdc_Code_NOTYPE_DCS_332;
        case 343: return _qdc_Code_NOTYPE_DCS_343;
        case 346: return _qdc_Code_NOTYPE_DCS_346;
        case 351: return _qdc_Code_NOTYPE_DCS_351;
        case 356: return _qdc_Code_NOTYPE_DCS_356;
        case 364: return _qdc_Code_NOTYPE_DCS_364;
        case 365: return _qdc_Code_NOTYPE_DCS_365;
        case 371: return _qdc_Code_NOTYPE_DCS_371;
        case 411: return _qdc_Code_NOTYPE_DCS_411;
        case 412: return _qdc_Code_NOTYPE_DCS_412;
        case 413: return _qdc_Code_NOTYPE_DCS_413;
        case 423: return _qdc_Code_NOTYPE_DCS_423;
        case 431: return _qdc_Code_NOTYPE_DCS_431;
        case 432: return _qdc_Code_NOTYPE_DCS_432;
        case 445: return _qdc_Code_NOTYPE_DCS_445;
        case 446: return _qdc_Code_NOTYPE_DCS_446;
        case 452: return _qdc_Code_NOTYPE_DCS_452;
        case 454: return _qdc_Code_NOTYPE_DCS_454;
        case 455: return _qdc_Code_NOTYPE_DCS_455;
        case 462: return _qdc_Code_NOTYPE_DCS_462;
        case 464: return _qdc_Code_NOTYPE_DCS_464;
        case 465: return _qdc_Code_NOTYPE_DCS_465;
        case 466: return _qdc_Code_NOTYPE_DCS_466;
        case 503: return _qdc_Code_NOTYPE_DCS_503;
        case 506: return _qdc_Code_NOTYPE_DCS_506;
        case 516: return _qdc_Code_NOTYPE_DCS_516;
        case 523: return _qdc_Code_NOTYPE_DCS_523;
        case 526: return _qdc_Code_NOTYPE_DCS_526;
        case 532: return _qdc_Code_NOTYPE_DCS_532;
        case 546: return _qdc_Code_NOTYPE_DCS_546;
        case 565: return _qdc_Code_NOTYPE_DCS_565;
        case 606: return _qdc_Code_NOTYPE_DCS_606;
        case 612: return _qdc_Code_NOTYPE_DCS_612;
        case 624: return _qdc_Code_NOTYPE_DCS_624;
        case 627: return _qdc_Code_NOTYPE_DCS_627;
        case 631: return _qdc_Code_NOTYPE_DCS_631;
        case 632: return _qdc_Code_NOTYPE_DCS_632;
        case 654: return _qdc_Code_NOTYPE_DCS_654;
        case 662: return _qdc_Code_NOTYPE_DCS_662;
        case 664: return _qdc_Code_NOTYPE_DCS_664;
        case 703: return _qdc_Code_NOTYPE_DCS_703;
        case 712: return _qdc_Code_NOTYPE_DCS_712;
        case 723: return _qdc_Code_NOTYPE_DCS_723;
        case 731: return _qdc_Code_NOTYPE_DCS_731;
        case 732: return _qdc_Code_NOTYPE_DCS_732;
        case 734: return _qdc_Code_NOTYPE_DCS_734;
        case 743: return _qdc_Code_NOTYPE_DCS_743;
        case 754: return _qdc_Code_NOTYPE_DCS_754;
        default: return qdc_Code_NONE;
    }
}

qdc_Code qdc_parseDcs(unsigned int code, bool isInverted) {
    return isInverted ? _qdc_Code_TYPE_DCS_INVERTED : _qdc_Code_TYPE_DCS | qdc_parseDcsWithoutType(code);
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


size_t qdc_csvGetColumnCount(const char *csvLine, size_t csvLineLen) {
    int columnCount = 0;
    const char *current = csvLine;
    while (current != NULL) {
        columnCount++;
        current = qdc_strFindNext(current, csvLineLen - (current - csvLine), ',');
    }
    return columnCount;
}

qdc_CsvChirpParseHeaderErr qdc_csvChirpParseHeader(const char *csvHeaderLine, size_t csvHeaderLen, void **parsers, size_t columnCount) {
    qdc_CsvChirpParserFn **columnParsers = (qdc_CsvChirpParserFn**)parsers;
    const char *fieldStart = csvHeaderLine;
    const char *fieldEnd = qdc_strFindNext(fieldStart, csvHeaderLen - (fieldStart - csvHeaderLine), ',');
    unsigned int index = 0;
    do {
        if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "Location")) {
            columnParsers[index] = qdc_csvChirpParseIgnore;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "Name")) {
            columnParsers[index] = qdc_csvChirpParseName;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "Frequency")) {
            columnParsers[index] = qdc_csvChirpParseFrequency;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "Duplex")) {
            columnParsers[index] = qdc_csvChirpParseDuplex;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "Offset")) {
            columnParsers[index] = qdc_csvChirpParseOffset;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "Tone")) {
            columnParsers[index] = qdc_csvChirpParseTone;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "rToneFreq")) {
            columnParsers[index] = qdc_csvChirpParseRToneFreq;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "cToneFreq")) {
            columnParsers[index] = qdc_csvChirpParseCToneFreq;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "DtcsCode")) {
            columnParsers[index] = qdc_csvChirpParseDtscCode;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "DtcsPolarity")) {
            columnParsers[index] = qdc_csvChirpParseDtscPolarity;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "RxDtcsCode")) {
            columnParsers[index] = qdc_csvChirpParseRxDtscCode;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "CrossMode")) {
            columnParsers[index] = qdc_csvChirpParseCrossMode;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "Mode")) {
            columnParsers[index] = qdc_csvChirpParseMode;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "TStep")) {
            columnParsers[index] = qdc_csvChirpParseTStep;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "Skip")) {
            columnParsers[index] = qdc_csvChirpParseSkip;
        } else if (qdc_csvAreFieldsEqual(fieldStart, fieldEnd - fieldStart, "Power")) {
            columnParsers[index] = qdc_csvChirpParsePower;
        } else {
            columnParsers[index] = qdc_csvChirpParseIgnore;
        }
        fieldStart = fieldEnd;
        fieldEnd = qdc_strFindNext(fieldEnd, csvHeaderLen - (fieldStart - csvHeaderLine), ',');
        index++;
    } while (fieldEnd != NULL);
    if (index != columnCount - 1) return qdc_CsvChirpParseHeaderErr_MISSING_COLUMNS;
    return qdc_CsvChirpParseHeaderErr_NONE;
}

qdc_CsvChirpParseChannelErr qdc_csvChirpParseChannel(const char *csvDataLine, size_t csvLineLen, qdc_Channel *ch, const void **parsers) {
    qdc_CsvChirpParserFn **columnParsers = (qdc_CsvChirpParserFn**)parsers;
    const char *fieldStart = csvDataLine;
    const char *fieldEnd = qdc_strFindNext(fieldStart, csvLineLen, ',');
    if (fieldEnd == NULL) { fieldEnd = csvDataLine + csvLineLen; }
    unsigned int index = 0;
    qdc_CsvChirpParserDefer defer = {0};
    qdc_CsvChirpParseChannelErr result = qdc_CsvChirpParseChannelErr_NONE;

    do {
        result = result | columnParsers[index](fieldStart, fieldEnd - fieldStart, ch, &defer);
        fieldStart = fieldEnd;
        fieldEnd = qdc_strFindNext(fieldEnd, fieldEnd - fieldStart, ',');
        index++;
    } while (fieldEnd != NULL);

    qdc_csvChirpParseDefered(&defer, ch);
    return result;
}
