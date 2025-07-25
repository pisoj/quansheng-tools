#ifndef _LIBQUANSHANG_CHANNELS_CSV_H
#define _LIBQUANSHANG_CHANNELS_CSV_H

#include "channel.h"
#include <stddef.h>

typedef enum qdc_CsvChirpParseChannelErr {
    qdc_CsvChirpParseChannelErr_NONE = 0b0,
    qdc_CsvChirpParseChannelErr_NAME_TOO_LONG_AUTOFIXED = 0b1,
    qdc_CsvChirpParseChannelErr_FREQUENCY_MUST_BE_DECIMAL = 0b10,
    qdc_CsvChirpParseChannelErr_OFFSET_MUST_BE_DECIMAL = 0b100,
    qdc_CsvChirpParseChannelErr_TONE_MUST_BE_DECIMAL = 0b1000,
    qdc_CsvChirpParseChannelErr_TONE_SQUELCH_MUST_BE_DECIMAL = 0b10000,
    qdc_CsvChirpParseChannelErr_DTCS_MUST_BE_INTEGER = 0b100000,
    qdc_CsvChirpParseChannelErr_RX_DTCS_MUST_BE_INTEGER = 0b1000000,
    qdc_CsvChirpParseChannelErr_DTCS_POLARITY_FIELD_TOO_LONG = 0b10000000,
    qdc_CsvChirpParseChannelErr_UNSUPPORTED_FREQUENCY_STEP_AUTOFIXED = 0b100000000,
    qdc_CsvChirpParseChannelErr_POWER_MUST_BE_DECIMAL = 0b1000000000,
} qdc_CsvChirpParseChannelErr;

typedef enum qdc_CsvChirpParseHeaderErr {
    qdc_CsvChirpParseHeaderErr_NONE = 0x0,
    qdc_CsvChirpParseHeaderErr_MISSING_COLUMNS = 0x1,
} qdc_CsvChirpParseHeaderErr;

size_t qdc_csvGetColumnCount(const char *csvLine, size_t csvLineLen);

/**
 * @param csvHeaderLine must be null terminated string of exactly one CSV line, usually the first one.
 * @param destionation empty, null initialized [qdc_CsvChirpHeaderMap] struct.
 */
qdc_CsvChirpParseHeaderErr qdc_csvChirpParseHeader(const char *csvHeaderLine, size_t csvHeaderLen, void **parsers, size_t columnCount);

qdc_CsvChirpParseChannelErr qdc_csvChirpParseChannel(const char *csvDataLine, size_t csvLineLen, qdc_Channel *ch, const void **parsers);

#endif
