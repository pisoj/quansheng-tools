#ifndef _LIBQUANSHANG_CHANNELS_INTERNAL_FILE_H
#define _LIBQUANSHANG_CHANNELS_INTERNAL_FILE_H

#include <stdio.h>

typedef enum qdc_FileReadNextLineErr {
    qdc_FileReadNextLineErr_NONE = 0,
    qdc_FileReadNextLineErr_FAILED_TO_ALLOCATE_MEMORY = 1,
    qdc_FileReadNextLineErr_END_OF_FILE_ALREADY_REACHED = 2,
} qdc_FileReadNextLineErr;

/**
 * The char pointer - *line - will be set to the start of the line string if succesfull, or to NULL if not.
 * Caller should free the *line if it is not NULL.
 */
qdc_FileReadNextLineErr qdc_fileReadNextLine(FILE *file, char **line);

#endif
