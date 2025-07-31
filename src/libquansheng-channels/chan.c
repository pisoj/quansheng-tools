#include "chan.h"
#include <string.h>
#include "internal/string.h"

qdc_ChanSetChannelErr qdc_chanSetChannel(qdc_Chan *chan, unsigned int channelNumber, const qdc_Channel *ch) {
    if (channelNumber > 199) {
        return qdc_ChanSetChannelErr_MAX_CHANNEL_NUMBER_IS_199;
    }
    if (ch->rxFrequency > 1300000000) {
        return qdc_ChanSetChannelErr_MAX_RX_FREQUENCY_IS_1300000000;
    }
    if (ch->txOffset > ch->rxFrequency) {
        return qdc_ChanSetChannelErr_TX_OFFSET_BIGGER_THAN_RX_FREQUENCY;
    }

    char name[sizeof(ch->name)];
    strncpy(name, ch->name, sizeof(name));
    int replacedCharactersCount = qdc_strReplaceNonPrintableASCII(name, sizeof(name), ' ');
    if(qdc_strIsEmpty(name, sizeof(name))) {
        return qdc_ChanSetChannelErr_INVALID_CHANNEL_NAME;
    }

    qdc_ChanSetChannelErr result = qdc_ChanSetChannelErr_NONE;
    if (replacedCharactersCount > 0) {
        result = qdc_ChanSetChannelErr_NON_ASCII_CHANNEL_NAME_AUTOFIXED;
    }
    memcpy(chan->_name[channelNumber], name, _qdc_CHANNEL_NAME_MAX_CHARACTERS);
    qdc_strMakeLengthFixed(chan->_name[channelNumber], sizeof(name), ' ');


    chan->_data[channelNumber] = (_qdc_ChanChannelData){
        .rxFrequency = ch->rxFrequency / 10,
        .txOffset = ch->txOffset / 10,
        .rxCode = ch->rxCode,
        .txCode = ch->txCode,
        .txCodeType_rxCodeType = (ch->txCode & 0xF00) >> 4 | ch->rxCode >> 8,
        .modulation_offsetDirection = ch->modulation << 4 | ch->offsetDirection,
        .busyLock_outputPower_bandwidth_reverse = ch->preventTxWhileReceiving << 4 | ch->outputPower << 2 | ch->bandwidth << 1 | ch->swapRxAndTxFrequencies,
        .pttId_dtmf = ch->pttIdMode << 1 | ch->enableDtmf,
        .step = ch->frequencyStep,
        .scramble = ch->scrambleMode,
    };

    unsigned int band;
    if (ch->rxFrequency == 0) {
        band = 15;
    } else if (ch->rxFrequency < 108000000) {
        band = 0;
    } else if (ch->rxFrequency < 137000000) {
        band = 1;
    } else if (ch->rxFrequency < 174000000) {
        band = 2;
    } else if (ch->rxFrequency < 350000000) {
        band = 3;
    } else if (ch->rxFrequency < 400000000) {
        band = 4;
    } else if (ch->rxFrequency < 470000000) {
        band = 5;
    } else {
        band = 6;
    }
    chan->_attr[channelNumber] = ch->includeInScanList1 << 7 | ch->includeInScanList2 << 6 | ch->enableRxCompander << 5 | ch->enableTxCompander << 4 | band;

    return result;
}
