#ifndef _LIBQUANSHANG_CHANNELS_INTERNAL_UTILS_H
#define _LIBQUANSHANG_CHANNELS_INTERNAL_UTILS_H

#include <stddef.h>
#include <stdbool.h>

/**
 * If string only contains spaces or null characters it is considered empty.
*/
bool qdc_strIsEmpty(const char *str, size_t len);

/**
 * Adds the given character after the string to satisfy specified length.
*/
void qdc_strMakeLengthFixed(char *str, size_t len, char filler);

/**
 * @returns How many characters have been replaced.
*/
int qdc_strReplaceNonPrintableASCII(char *str, size_t len, char replacement);

#endif
