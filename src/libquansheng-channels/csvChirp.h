#ifndef _LIBQUANSHANG_CHANNELS_CSV_CHIRP_H
#define _LIBQUANSHANG_CHANNELS_CSV_CHIRP_H

#include "channel.h"

typedef struct qdc_CsvChirp qdc_CsvChirp;

/**
 * @param csvHeader String containing the header (usually the first row) of the CSV. It contains column names as opposed to channel data. Should **not be NULL.*
 * @returns Pointer to new qdc_CsvChirp object that **should be destroied** using _qdc_csvChirpParseChannel_ when no more operations
 * are to be performed. **NULL pointer** ca be returned if the allocation fails.
*/
qdc_CsvChirp *qdc_csvChirpNew(const char *csvHeader, unsigned long headerLen);

/**
 * To determin if an error ocured use bitwise and (&) to compare a vaariable to the error. For example:
 * ```c
 *
 * qdc_CsvChirpParseChannelErr error = qdc_csvChirpParseChannel(...);
 * if (error == qdc_CsvChirpParseChannelErr_NONE) {
 *         Everything went successfully!
 * } else if (error & qdc_CsvChirpParseChannelErr_FREQUENCY_MUST_BE_DECIMAL) {
 *         Handle the error here...
 * } else if (error & qdc_CsvChirpParseChannelErr_NAME_TOO_LONG_AUTOFIXED) {
 *         Handle the autofixed error here...
 * } else {
 *         Some other error ocured...
 * }
 * ```
 *
 * **Note:** If an error ends with `AUTOFIXED` it can be considered more of an informational signal than an error.
 * It is here to let you know something happened, but that it wasn't anything critical and the program can continue working corectly.
*/
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

/**
 * @param csv Pointer to an object retreved from qdc_csvChirpNew
 * @param chIndex Pointer to where the index of the channel as specified in the CSV's location column should be saved, should **not be NULL**
 * @param ch Pointer to where the parsed channel information should be saved, should **not be NULL**
 */
qdc_CsvChirpParseChannelErr qdc_csvChirpParseChannel(const qdc_CsvChirp *csv, const char *csvLine, unsigned long csvLineLen, unsigned long *chIndex, qdc_Channel *ch);

void qdc_csvChirpDestroy(qdc_CsvChirp *csv);

#endif
