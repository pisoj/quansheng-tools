#ifndef _LIBQUANSHANG_CHANNELS_CSV_CHIRP_H
#define _LIBQUANSHANG_CHANNELS_CSV_CHIRP_H

#include "channel.h"

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
    qdc_CsvChirpParseChannelErr_LOCATION_MUST_BE_INTEGER = 0b10000000000,
    qdc_CsvChirpParseChannelErr_LOCATION_MUST_BE_POSITIVE = 0b100000000000,
} qdc_CsvChirpParseChannelErr;

typedef struct qdc_CsvChirp qdc_CsvChirp;

/**
 * @returns Pointer to new qdc_CsvChirp object that **should be destroied** using _qdc_csvChirpParseChannel_ when no more operations
 * are to be performed. **NULL pointer** ca be returned if the allocation fails.
*/
qdc_CsvChirp *qdc_csvChirpNew(const char *csvHeader, unsigned long headerLen);

void qdc_csvChirpDestroy(qdc_CsvChirp *csv);

qdc_CsvChirpParseChannelErr qdc_csvChirpParseChannel(const qdc_CsvChirp *csv, const char *csvLine, unsigned long csvLineLen, unsigned long *chIndex, qdc_Channel *ch);

#endif
