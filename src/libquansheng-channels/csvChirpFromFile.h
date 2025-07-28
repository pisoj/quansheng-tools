#ifndef _LIBQUANSHANG_CHANNELS_CSV_CHIRP_FROM_FILE_H
#define _LIBQUANSHANG_CHANNELS_CSV_CHIRP_FROM_FILE_H

#include "csvChirp.h"

typedef enum qdc_CsvChirpFromFileNewErr {
    qdc_CsvChirpFromFileNewErr_NONE = 0,
    qdc_CsvChirpFromFileNewErr_FAILED_TO_ALLOCATE_MEMORY = 1,
    qdc_CsvChirpFromFileNewErr_FAILED_TO_OPEN_FILE = 2,
    qdc_CsvChirpFromFileNewErr_EMPTY_FILE = 3,
} qdc_CsvChirpFromFileNewErr;

typedef struct qdc_CsvChirpFile qdc_CsvChirpFile;

qdc_CsvChirpFromFileNewErr qdc_csvChirpFromFileNew(const char *path, qdc_CsvChirpFile **csvFile);

typedef enum qdc_CsvChirpFromFileParseNextChannelErr {
    qdc_CsvChirpFromFileParseNextChannelErr_NONE = 0,
    qdc_CsvChirpFromFileParseNextChannelErr_FAILED_TO_ALLOCATE_MEMORY = 1,
    qdc_CsvChirpFromFileParseNextChannelErr_NO_MORE_CHANNELS_TO_PARSE = 2,
} qdc_CsvChirpFromFileParseNextChannelErr;

/**
 * @param csvFile Pointer to an object retreived from qdc_csvChirpFromFileNew
 * @param chIndex Pointer to where the index of the channel as specified in the CSV's location column should be saved, should **not be NULL**
 * @param ch Pointer to where the parsed channel information should be saved, should **not be NULL**
 * @param parserErr can be NULL pointer, in which case the result of parsing won't be saved
*/
qdc_CsvChirpFromFileParseNextChannelErr qdc_CsvChirpFromFileParseNextChannel(const qdc_CsvChirpFile *csvFile, unsigned long *chIndex, qdc_Channel *destCh, qdc_CsvChirpParseChannelErr *parserErr);

void qdc_csvChirpFromFileDestory(qdc_CsvChirpFile *csvFile);

#endif
