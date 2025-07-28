#include "file.h"

#include <stdlib.h>
#include <string.h>

qdc_FileReadNextLineErr qdc_fileReadNextLine(FILE *file, char **line) {
    *line = NULL;
    size_t buffLen = 50;
    char *buff = malloc(buffLen);
    buff[0] = '\0'; // If we are already at the end of the file buffer will remain unchanged
    if (buff == NULL) return qdc_FileReadNextLineErr_FAILED_TO_ALLOCATE_MEMORY;

    size_t len = 0;
    while (fgets(buff + len, buffLen - len, file) != NULL) {
        len = strlen(buff);
        if ((len > 0 && buff[len - 1] == '\n') || feof(file)) {
            *line = buff;
            printf("Read:\n%s\n\n", buff);
            return qdc_FileReadNextLineErr_NONE;
        } else {
            // no newline and no EOF, line was too long
            buffLen *= 2;
            char *newbuff = realloc(buff, buffLen);
            if (newbuff == NULL) {
                free(buff);
                return qdc_FileReadNextLineErr_FAILED_TO_ALLOCATE_MEMORY;
            };
            buff = newbuff;
        }
    }
    if (buff[0] == '\0') return qdc_FileReadNextLineErr_END_OF_FILE_ALREADY_REACHED;

    return qdc_FileReadNextLineErr_NONE;
}
