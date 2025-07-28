#ifndef _LIBQUANSHANG_CHANNELS_CHANNEL_H
#define _LIBQUANSHANG_CHANNELS_CHANNEL_H

#include "ctcss_dcs.h"

#define _qdc_CHANNEL_NAME_MAX_CHARACTERS sizeof(char) * 10 // More cannot fit on radio screen

typedef enum qdc_Modulation {
    qdc_Modulation_FM = 0x0,
    qdc_Modulation_AM = 0x1,
    qdc_Modulation_USB = 0x2,
} qdc_Modulation;

typedef enum qdc_OffsetDirection {
    qdc_OffsetDirection_NONE = 0x0,
    qdc_OffsetDirection_PLUS = 0x1,
    qdc_OffsetDirection_MINUS = 0x2,
} qdc_OffsetDirection;

typedef enum qdc_OutputPower {
    qdc_OutputPower_LOW = 0x0,
    qdc_OutputPower_MEDIUM = 0x1,
    qdc_OutputPower_HIGH = 0x2,
} qdc_OutputPower;

typedef enum qdc_Bandwidth {
    qdc_Bandwidth_WIDE = 0x0,
    qdc_Bandwidth_NARROW = 0x1,
} qdc_Bandwidth;

typedef enum qdc_PttIdMode {
    qdc_PttId_NONE = 0b000,
    qdc_PttId_BEGIN = 0b001,
    qdc_PttId_END = 0b010,
    qdc_PttId_BEGIN_END = 0b011,
    qdc_PttId_APOLLO_QUINDAR = 0b100,
} qdc_PttIdMode;

typedef enum qdc_FrequencyStep {
    qdc_FrequencyStep_0_01 = 0x07,
    qdc_FrequencyStep_0_05 = 0x08,
    qdc_FrequencyStep_0_1 = 0x09,
    qdc_FrequencyStep_0_25 = 0x0A,
    qdc_FrequencyStep_0_5 = 0x0B,
    qdc_FrequencyStep_1 = 0x0C,
    qdc_FrequencyStep_1_25 = 0x0D,
    qdc_FrequencyStep_2_5 = 0x00,
    qdc_FrequencyStep_5 = 0x01,
    qdc_FrequencyStep_6_25 = 0x02,
    qdc_FrequencyStep_8_33 = 0x06,
    qdc_FrequencyStep_10 = 0x03,
    qdc_FrequencyStep_12_5 = 0x04,
    qdc_FrequencyStep_15 = 0x0E,
    qdc_FrequencyStep_25 = 0x05,
    qdc_FrequencyStep_30 = 0x0F,
    qdc_FrequencyStep_50 = 0x10,
    qdc_FrequencyStep_100 = 0x11,
    qdc_FrequencyStep_125 = 0x12,
    qdc_FrequencyStep_250 = 0x13,
    qdc_FrequencyStep_500 = 0x14,
} qdc_FrequencyStep;

typedef enum qdc_ScrambleMode {
    qdc_Scramble_NONE = 0x00,
    qdc_Scramble_2600 = 0x01,
    qdc_Scramble_2700 = 0x02,
    qdc_Scramble_2800 = 0x03,
    qdc_Scramble_2900 = 0x04,
    qdc_Scramble_3000 = 0x05,
    qdc_Scramble_3100 = 0x06,
    qdc_Scramble_3200 = 0x07,
    qdc_Scramble_3300 = 0x08,
    qdc_Scramble_3400 = 0x09,
    qdc_Scramble_3500 = 0x0A,
} qdc_ScrambleMode;

typedef struct qdc_Channel {
    unsigned long rxFrequency;
    unsigned long txOffset;
    qdc_Code txCode;
    qdc_Code rxCode;
    qdc_Modulation modulation;
    qdc_OffsetDirection offsetDirection;
    _Bool preventTxWhileReceiving;
    qdc_OutputPower outputPower;
    qdc_Bandwidth bandwidth;
    _Bool swapRxAndTxFrequencies;
    qdc_PttIdMode pttIdMode;
    _Bool enableDtmf;
    qdc_FrequencyStep frequencyStep;
    qdc_ScrambleMode scrambleMode;
    char name[_qdc_CHANNEL_NAME_MAX_CHARACTERS + 1];
    _Bool includeInScanList1;
    _Bool includeInScanList2;
    _Bool enableTxCompander;
    _Bool enableRxCompander;
} qdc_Channel;

#endif
