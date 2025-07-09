#ifndef _LIBQUANSHANG_CHANNELS_CHANN_H
#define _LIBQUANSHANG_CHANNELS_CHANN_H

#include <stdint.h>
#include "channel.h"

typedef struct _qdc_ChanChannelData {
    uint32_t rxFrequency;
    uint32_t txOffset;
    uint8_t rxCode;
    uint8_t txCode;
    uint8_t txCodeType_rxCodeType;
    uint8_t modulation_offsetDirection;
    uint8_t busyLock_outputPower_bandwidth_reverse;
    uint8_t pttId_dtmf;
    uint8_t step;
    uint8_t scramble;
} _qdc_ChanChannelData;

typedef struct qdc_Chan {
    _qdc_ChanChannelData _data[200];
    char _name[200][16];
    uint8_t _attr[200];
} qdc_Chan;

/**
 * *_AUTOFIXED can safely be ignored. It is only for informational purposes.
*/
typedef enum qdc_ChanSetChannelErr {
    qdc_ChanSetChannelErr_NONE = 0,
    qdc_ChanSetChannelErr_MAX_CHANNEL_NUMBER_IS_199 = 2,
    qdc_ChanSetChannelErr_MAX_RX_FREQUENCY_IS_1300000000 = 3,
    qdc_ChanSetChannelErr_TX_OFFSET_BIGGER_THAN_RX_FREQUENCY = 4,
    qdc_ChanSetChannelErr_INVALID_CHANNEL_NAME = 5,
    qdc_ChanSetChannelErr_NON_ASCII_CHANNEL_NAME_AUTOFIXED = 6,
} qdc_ChanSetChannelErr;
qdc_ChanSetChannelErr qdc_chanSetChannel(qdc_Chan *chan, unsigned int channelNumber, const qdc_Channel *ch);

#endif
