#include <libquansheng-channels/channel.h>
#include <libquansheng-channels/csv.h>
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
    size_t buffLen = 1024;
    char *buff = malloc(buffLen);
    buff[0] = '\0'; // If we are already at the end of the file buffer will remain unchanged
    if (buff == NULL) return qdc_FileReadNextLineErr_FAILED_TO_ALLOCATE_MEMORY;

    while (fgets(buff, buffLen, file) != NULL) {
        size_t len = strlen(buff);
        if ((len > 0 && buff[len - 1] == '\n') || feof(file)) {
            *line = buff;
            return qdc_FileReadNextLineErr_NONE;
        } else {
            // no newline and not EOF, line was too long
            buffLen *= 2;
            char *newbuff = realloc(buff, buffLen);
            if (newbuff == NULL) {
                free(buff);
                return qdc_FileReadNextLineErr_FAILED_TO_ALLOCATE_MEMORY;
            };
        }
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
    size_t csvColumnCount = qdc_csvGetColumnCount(line, lineLen);
    printf("Column count: %d\n", (int)csvColumnCount);
    void *parsers = malloc(sizeof(void*[csvColumnCount]));
    qdc_CsvChirpParseHeaderErr eer = qdc_csvChirpParseHeader(line, lineLen, parsers, csvColumnCount);
    printf("parse header error: %d\n", eer);
    free(line);

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
    lineLen = strlen(line);
    qdc_Channel ch;
    qdc_CsvChirpParseChannelErr perr = qdc_csvChirpParseChannel(line, lineLen, &ch, parsers);
    printf("Ch parser error: %b\n", perr);
    qdc_chanSetChannel(&chan, 2, &ch);

    free(parsers);

    fwrite(&chan ,sizeof(chan), 1, chanFile);

    fclose(chanFile);
    fclose(csvFile);

    return 0;
}
