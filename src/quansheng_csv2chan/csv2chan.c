#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include <libquansheng-channels/libquansheng-channels.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Convert a CSV from CHIRP to a .chan file used by Quanshang Dock.\n\tUsage: csv2chan <input.csv> <output.chan>\n");
        return 1;
    }

    FILE *csv = fopen(argv[1], "wb");

    qdc_Channel channel = {
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
    };
    qdc_Chan chan = {0};
    qdc_chanSetChannel(&chan, 1, &channel);

    fwrite(&chan, sizeof(chan), 1, csv);

    fclose(csv);

    return 0;
}
