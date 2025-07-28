#include "csvChirpFromFile.h"
#include "csvChirp.h"
#include "internal/file.h"
#include "internal/minmax.h"
#include "internal/string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct qdc_CsvChirpFile {
    FILE *file;
    qdc_CsvChirp *csv;
};

qdc_CsvChirpFromFileNewErr qdc_csvChirpFromFileNew(const char *path, qdc_CsvChirpFile **csvFile) {
    *csvFile = malloc(sizeof(qdc_CsvChirpFile));
    if (csvFile == NULL) return qdc_CsvChirpFromFileNewErr_FAILED_TO_ALLOCATE_MEMORY;
    (*csvFile)->file = fopen(path, "r");
    if ((*csvFile)->file == NULL) {
        free(*csvFile);
        return qdc_CsvChirpFromFileNewErr_FAILED_TO_OPEN_FILE;
    }

    char *line = NULL;
    unsigned long lineLen = 0;
    qdc_FileReadNextLineErr fileErr;
    while ((fileErr = qdc_fileReadNextLine((*csvFile)->file, &line)) == qdc_FileReadNextLineErr_NONE && qdc_strIsEmpty(line, qdc_max(1, (lineLen = strlen(line))) - 1)) {
        free(line);
        line = NULL;
        lineLen = 0;
    }
    switch (fileErr) {
        case qdc_FileReadNextLineErr_FAILED_TO_ALLOCATE_MEMORY:
            free(*csvFile);
            return qdc_CsvChirpFromFileNewErr_FAILED_TO_ALLOCATE_MEMORY;
        case qdc_FileReadNextLineErr_END_OF_FILE_ALREADY_REACHED:
            free(*csvFile);
            return qdc_CsvChirpFromFileNewErr_EMPTY_FILE;
        case qdc_FileReadNextLineErr_NONE:
            break;
    }

    (*csvFile)->csv = qdc_csvChirpNew(line, lineLen);
    if ((*csvFile)->csv == NULL) {
        free(*csvFile);
        return qdc_CsvChirpFromFileNewErr_FAILED_TO_ALLOCATE_MEMORY;
    }

    return qdc_CsvChirpFromFileNewErr_NONE;
}

qdc_CsvChirpFromFileParseNextChannelErr qdc_CsvChirpFromFileParseNextChannel(const qdc_CsvChirpFile *csvFile, unsigned long *chIndex, qdc_Channel *destCh, qdc_CsvChirpParseChannelErr *parserErr) {
    char *line;
    switch (qdc_fileReadNextLine((csvFile->file), &line)) {
        case qdc_FileReadNextLineErr_FAILED_TO_ALLOCATE_MEMORY:
            return qdc_CsvChirpFromFileParseNextChannelErr_FAILED_TO_ALLOCATE_MEMORY;
        case qdc_FileReadNextLineErr_END_OF_FILE_ALREADY_REACHED:
            return qdc_CsvChirpFromFileParseNextChannelErr_NO_MORE_CHANNELS_TO_PARSE;
        case qdc_FileReadNextLineErr_NONE:
            break;
    }

    qdc_CsvChirpParseChannelErr parserErrStage = qdc_csvChirpParseChannel(csvFile->csv, line, strlen(line), chIndex, destCh);
    if (parserErr != NULL) *parserErr = parserErrStage;

    return qdc_CsvChirpFromFileParseNextChannelErr_NONE;
}

void qdc_csvChirpFromFileDestory(qdc_CsvChirpFile *csvFile) {
    qdc_csvChirpDestroy(csvFile->csv);
    fclose(csvFile->file);
    free(csvFile);
}
