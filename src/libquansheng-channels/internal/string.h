#ifndef _LIBQUANSHANG_CHANNELS_INTERNAL_STRING_H
#define _LIBQUANSHANG_CHANNELS_INTERNAL_STRING_H

#include <stddef.h>
#include "minmax.h"

typedef enum qdc_strToNumberErr {
    qdc_strToNumberErr_NONE = 0,
    qdc_strToNumberErr_INVALID_NUMBER_FORMAT = 1,
} qdc_strToNumberErr;

/**
 * As oppose to strncmp takes lengths of both strings into account.
*/
int qdc_strCmpSafe(const char *str1, size_t str1size, const char *str2, size_t str2size);

/**
 * If string only contains spaces or null characters it is considered empty.
*/
_Bool qdc_strIsEmpty(const char *str, size_t len);

/**
 * Adds the given character after the string to satisfy specified length.
*/
void qdc_strMakeLengthFixed(char *str, size_t len, char filler);

/**
 * @returns How many characters have been replaced.
*/
int qdc_strReplaceNonPrintableASCII(char *str, size_t len, char replacement);

/**
 * @return Pointer to the first character after the target or NULL pointer if the target character cannot be found or if it is the last character.
*/
const char *qdc_strFindNext(const char *str, size_t len, char target);

qdc_strToNumberErr qdc_strToLong(const char *str, size_t len, long *destination);

qdc_strToNumberErr qdc_strToFloat(const char *str, size_t len, float *destination);

qdc_strToNumberErr qdc_strToDouble(const char *str, size_t len, double *destination);

#endif
