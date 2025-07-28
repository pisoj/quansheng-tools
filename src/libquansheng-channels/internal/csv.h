#ifndef _LIBQUANSHANG_CHANNELS_INTERNAL_CSV_H
#define _LIBQUANSHANG_CHANNELS_INTERNAL_CSV_H

#include "string.h"

#define qdc_csvAreFieldsEqual(csvStr, csvStrSize, strToCmp) !qdc_strCmpSafe(csvStr, csvStrSize, strToCmp, sizeof(strToCmp) - sizeof(char))

#define qdc_csvForEachColumn(csvLine, csvLineLen, columnCount, ...)                             \
        const char *fieldStart = csvLine;                                                       \
        const char *fieldEnd = qdc_strFindNext(fieldStart, csvLineLen, ',');                    \
        if (fieldEnd == NULL) { fieldEnd = csvLine + csvLineLen; }                              \
        for (unsigned long i = 0; i < columnCount; i++) {                                           \
            __VA_ARGS__                                                                         \
            fieldStart = fieldEnd;                                                              \
            fieldEnd = qdc_strFindNext(fieldStart, csvLineLen - (fieldStart - csvLine), ',');   \
            if (fieldEnd == NULL) {                                                             \
                fieldEnd = csvLine + csvLineLen;                                                \
            }                                                                                   \
            if (fieldStart == fieldEnd) {                                                       \
                break;                                                                          \
            }                                                                                   \
        }                                                                                       \

unsigned long qdc_csvGetColumnCount(const char *csvLine, unsigned long csvLineLen);

#endif
