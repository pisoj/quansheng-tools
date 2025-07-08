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

    ChanChannel channel = {
        .rxFrequency = 145750000,
        .txOffset = 600,
        .txCode = ChanCode_CTCSS_254_1,
        .rxCode = ChanCode_NONE,
        .modulation = ChanModulation_FM,
        .offsetDirection = ChanOffsetDirection_MINUS,
        .preventTxWhileReceiving = true,
        .outputPower = ChanOutputPower_MEDIUM,
        .bandwidth = ChanBandwidth_WIDE,
        .swapRxAndTxFrequencies = false,
        .pttIdMode = ChanPttId_BEGIN_END,
        .enableDtmf = true,
        .frequencyStep = ChanFrequencyStep_25,
        .scrambleMode = ChanScramble_NONE,
        .name = " ššs",
        .includeInScanList1 = false,
        .includeInScanList2 = true,
        .enableRxCompander = false,
        .enableTxCompander = true,
    };
    Chan chan = {0};
    chanSetChannel(&chan, 1, &channel);

    fwrite(&chan, sizeof(chan), 1, csv);

    fclose(csv);

    return 0;
}
