#include <libquansheng-channels/chan.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <libquansheng-channels/libquansheng-channels.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Convert a CSV from CHIRP to a .chan file used by Quanshang Dock.\n\tUsage: csv2chan <input.csv> <output.chan>\n");
        return 1;
    }

    qdc_Chan chan = {0};

    qdc_CsvChirpFile *csvFile;
    switch (qdc_csvChirpFromFileNew(argv[1], &csvFile)) {
        case qdc_CsvChirpFromFileNewErr_FAILED_TO_ALLOCATE_MEMORY:
            fprintf(stderr, "Error: Failed to allocate memory.\n");
            return 2;
        case qdc_CsvChirpFromFileNewErr_FAILED_TO_OPEN_FILE:
            fprintf(stderr, "Error: Failed to open the csv file: %s", argv[1]);
            return 3;
        case qdc_CsvChirpFromFileNewErr_EMPTY_FILE:
            fprintf(stderr, "Error: The file %s is empty.\n", argv[1]);
            return 4;
        case qdc_CsvChirpFromFileNewErr_NONE:
            break;
    }

    qdc_CsvChirpFromFileParseNextChannelErr fileErr;
    unsigned long chIndex;
    qdc_Channel ch;
    qdc_CsvChirpParseChannelErr parserErr;
    while ((fileErr = qdc_CsvChirpFromFileParseNextChannel(csvFile, &chIndex, &ch, &parserErr)) == qdc_CsvChirpFromFileParseNextChannelErr_NONE) {
        if (parserErr == qdc_CsvChirpParseChannelErr_NONE) {
            // No errors, great!
        } else if (parserErr & qdc_CsvChirpParseChannelErr_NAME_TOO_LONG_AUTOFIXED) {
            fprintf(stderr, "Info: The name of the channel number %lu has been shrinked to %s in order to fit onto radio screen.\n", chIndex, ch.name);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_FREQUENCY_MUST_BE_DECIMAL) {
            fprintf(stderr, "Warning: Frequency of the channel #%lu %s must be decimal. Defaulting to 0 Hz.\n", chIndex, ch.name);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_OFFSET_MUST_BE_DECIMAL) {
            fprintf(stderr, "Warning: Tx offset of the channel #%lu %s must be decimal. Defaulting to 0 Hz.\n", chIndex, ch.name);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_TONE_MUST_BE_DECIMAL) {
            fprintf(stderr, "Warning: Tone field of the channel #%lu %s must be decimal. Defaulting to no tone.\n", chIndex, ch.name);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_TONE_SQUELCH_MUST_BE_DECIMAL) {
            fprintf(stderr, "Warning: Tone squelch field of the channel #%lu %s must be decimal. Defaulting to no tone squelch.\n", chIndex, ch.name);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_DTCS_MUST_BE_INTEGER) {
            fprintf(stderr, "Warning: DTCS field of the channel #%lu %s must be integer. Defaulting to no DCS.\n", chIndex, ch.name);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_RX_DTCS_MUST_BE_INTEGER) {
            fprintf(stderr, "Warning: Rx DTCS field of the channel #%lu %s must be integer. Defaulting to no Rx DCS.\n", chIndex, ch.name);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_DTCS_POLARITY_FIELD_TOO_LONG) {
            fprintf(stderr, "Warning: DTCS polarity field of the channel #%lu %s is too long. Defaulting to normal polarity.\n", chIndex, ch.name);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_UNSUPPORTED_FREQUENCY_STEP_AUTOFIXED) {
            fprintf(stderr, "Warning: Unsupported frequency step used by the channel #%lu %s. Defaulting to 5 kHz.\n", chIndex, ch.name);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_POWER_MUST_BE_DECIMAL) {
            fprintf(stderr, "Warning: Power field of the channel #%lu %s must be decimal. Defaulting to high power.\n", chIndex, ch.name);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_LOCATION_MUST_BE_INTEGER) {
            fprintf(stderr, "Warning: Location field of the channel %s must be an integer. Defaulting to channel number 0 (first channel).\n", ch.name);
        } else if (parserErr & qdc_CsvChirpParseChannelErr_LOCATION_MUST_BE_POSITIVE) {
            fprintf(stderr, "Warning: Location field of the channel %s must be positive. Defaulting to channel number 0 (first channel).\n", ch.name);
        } else {
            fprintf(stderr, "Error: An unknown error ocured while parding the channel #%lu %s. Tryign to porceed to the next channel...\n", chIndex, ch.name);
        }

        switch (qdc_chanSetChannel(&chan, chIndex, &ch)) {
            case qdc_ChanSetChannelErr_NONE:
                break;
            case qdc_ChanSetChannelErr_MAX_CHANNEL_NUMBER_IS_199:
                fprintf(stderr, "Error: Discarding channel #%lu %s because it does not fit into radio memory. Maximum number of channels is 200. Discarding channel.\n", chIndex, ch.name);
                break;
            case qdc_ChanSetChannelErr_MAX_RX_FREQUENCY_IS_1300000000:
                fprintf(stderr, "Error: Frequency of channel #%lu %s exceeds the limit of 1.3 GHz. Discarding channel.\n", chIndex, ch.name);
                break;
            case qdc_ChanSetChannelErr_TX_OFFSET_BIGGER_THAN_RX_FREQUENCY:
                fprintf(stderr, "Error: Tx offset of channel #%lu %s is bigger than rx frequency. Discarding channel.\n", chIndex, ch.name);
                break;
            case qdc_ChanSetChannelErr_INVALID_CHANNEL_NAME:
                fprintf(stderr, "Error: Name of the channel #%lu %s is invalid. Discarding channel.\n", chIndex, ch.name);
                break;
            case qdc_ChanSetChannelErr_NON_ASCII_CHANNEL_NAME_AUTOFIXED:
                fprintf(stderr, "Warning: Name of the channel #%lu %s contains non-ASCII characters. Discarding invalid characters.\n", chIndex, ch.name);
                break;
        }
    }

    qdc_csvChirpFromFileDestory(csvFile);

    FILE *chanFile = fopen(argv[2], "wb");
    if (chanFile == NULL) {
        fprintf(stderr, "Error: Failed to open the destination chan file.\n");
        return 5;
    }

    fwrite(&chan ,sizeof(chan), 1, chanFile);

    fclose(chanFile);

    return 0;
}
