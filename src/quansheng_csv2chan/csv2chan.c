#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <libquansheng-channels/libquansheng-channels.h>

typedef enum qdc_FileReadNextLineErr {
    qdc_FileReadNextLineErr_NONE = 0,
    qdc_FileReadNextLineErr_FAILED_TO_ALLOCATE_MEMORY = 1,
    qdc_FileReadNextLineErr_END_OF_FILE_ALREADY_REACHED = 2,
} qdc_FileReadNextLineErr;

/**
 * The char pointer - *line - will be set to the start of the line string if succesfull, or to NULL if not.
 * Caller should free the *line if it is not NULL.
 */
qdc_FileReadNextLineErr qdc_fileReadNextLine(FILE *file, char **line) {
    *line = NULL;
    size_t buffLen = 5;
    char *buff = malloc(buffLen);
    buff[0] = '\0'; // If we are already at the end of the file buffer will remain unchanged
    if (buff == NULL) return qdc_FileReadNextLineErr_FAILED_TO_ALLOCATE_MEMORY;

    size_t len = 0;
    int i = 0;
    while (fgets(buff + len, buffLen - len, file) != NULL) {
        len = strlen(buff);
        if ((len > 0 && buff[len - 1] == '\n') || feof(file)) {
            *line = buff;
            printf("Read:\n%s\n\n", buff);
            return qdc_FileReadNextLineErr_NONE;
        } else {
            // no newline and not EOF, line was too long
            buffLen *= 2;
            printf("To Realloc: %d, index: %d\n", (int)buffLen, i);
            char *newbuff = realloc(buff, buffLen);
            if (newbuff == NULL) {
                free(buff);
                return qdc_FileReadNextLineErr_FAILED_TO_ALLOCATE_MEMORY;
            };
            buff = newbuff;
        }
        i++;
    }
    if (buff[0] == '\0') return qdc_FileReadNextLineErr_END_OF_FILE_ALREADY_REACHED;

    return qdc_FileReadNextLineErr_NONE;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Convert a CSV from CHIRP to a .chan file used by Quanshang Dock.\n\tUsage: csv2chan <input.csv> <output.chan>\n");
        return 1;
    }

    FILE *csvFile = fopen(argv[1], "r");
    FILE *chanFile = fopen(argv[2], "wb");

    /*qdc_Channel channel = {
        .rxFrequency = 145750000,
        .txOffset = 600,
        .txCode = qdc_Code_CTCSS_254_1,
        .rxCode = qdc_Code_NONE,
        .modulation = qdc_Modulation_FM,
        .offsetDirection = qdc_OffsetDirection_MINUS,
        .preventTxWhileReceiving = true,
        .outputPower = qdc_OutputPower_MEDIUM,
        .bandwidth = qdc_Bandwidth_WIDE,
        .swapRxAndTxFrequencies = false,
        .pttIdMode = qdc_PttId_BEGIN_END,
        .enableDtmf = true,
        .frequencyStep = qdc_FrequencyStep_25,
        .scrambleMode = qdc_Scramble_NONE,
        .name = " ššs",
        .includeInScanList1 = false,
        .includeInScanList2 = true,
        .enableRxCompander = false,
        .enableTxCompander = true,
    };*/
    qdc_Chan chan = {0};
    //qdc_chanSetChannel(&chan, 1, &channel);

    char *line;
    switch (qdc_fileReadNextLine(csvFile, &line)) {
        case qdc_FileReadNextLineErr_FAILED_TO_ALLOCATE_MEMORY:
            printf("qdc_FileReadNextLineErr_FAILED_TO_ALLOCATE_MEMORY\n");
            return 1;
        case qdc_FileReadNextLineErr_END_OF_FILE_ALREADY_REACHED:
            printf("qdc_FileReadNextLineErr_END_OF_FILE_ALREADY_REACHED\n");
            return 1;
        default:
            break;
    }
    size_t lineLen = strlen(line);

    qdc_CsvChirp *csv = qdc_csvChirpNew(line, lineLen);

    free(line);

    size_t index = 0;
    while (true) {
        qdc_FileReadNextLineErr error = qdc_fileReadNextLine(csvFile, &line);
        if (error != qdc_FileReadNextLineErr_NONE) {
            if (error == qdc_FileReadNextLineErr_END_OF_FILE_ALREADY_REACHED) {
                printf("qdc_FileReadNextLineErr_END_OF_FILE_ALREADY_REACHED\n");
                break;
            } else if (error == qdc_FileReadNextLineErr_FAILED_TO_ALLOCATE_MEMORY) {
                printf("qdc_FileReadNextLineErr_FAILED_TO_ALLOCATE_MEMORY\n");
                return 1;
            }
        }
        lineLen = strlen(line);
        qdc_Channel ch;
        unsigned long chIndex;
        qdc_CsvChirpParseChannelErr perr = qdc_csvChirpParseChannel(csv, line, lineLen, &chIndex, &ch);
        printf("Index: %d, chIndex %d, Ch parser result: %b\n", (int)index, (int)chIndex, perr);
        qdc_chanSetChannel(&chan, chIndex, &ch);
        index++;
    }

    qdc_csvChirpDestroy(csv);

    fwrite(&chan ,sizeof(chan), 1, chanFile);

    fclose(chanFile);
    fclose(csvFile);

    return 0;
}
