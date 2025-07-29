#include "string.h"
#include "minmax.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

int qdc_strCmpSafe(const char *str1, size_t str1size, const char *str2, size_t str2size) {
    return strncmp(str1, str2, qdc_min(str1size, str2size));
}

_Bool qdc_strIsEmpty(const char *str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (str[i] != ' ' && str[i] != '\0') { return false; }
    }
    return true;
}

void qdc_strMakeLengthFixed(char *str, size_t len, char filler) {
    for (size_t i = 0; i < len - 1; i++) {
        if (str[i] != '\0') { continue; }
        str[i] = filler;
    }
    str[len - 1] = '\0';
}

int qdc_strReplaceNonPrintableASCII(char *str, size_t len, char replacement) {
    int replacedCount = 0;
    for (size_t i = 0; i < len; i++) {
        if ((str[i] >= 0x20 && str[i] <= 0x7E) || str[i] == '\0') { continue; }
        str[i] = replacement;
        replacedCount++;
    }
    return replacedCount;
}

const char *qdc_strFindNext(const char *str, size_t len, char target) {
    for (size_t i = 1; i < len; i++) {
        if (str[i - 1] != target) { continue; }
        return &str[i];
    }
    return NULL;
}

qdc_strToNumberErr qdc_strToLong(const char *str, size_t len, long *destination) {
    char strsafe[len + 1];
    strncpy(strsafe, str, len);
    strsafe[len] = '\0';

    char *startOfInvalid = NULL;
    long num = strtol(strsafe, &startOfInvalid, 10);
    if (startOfInvalid == strsafe) { return qdc_strToNumberErr_INVALID_NUMBER_FORMAT; }
    *destination = num;
    return qdc_strToNumberErr_NONE;
}

qdc_strToNumberErr qdc_strToFloat(const char *str, size_t len, float *destination) {
    char strsafe[len + 1];
    strncpy(strsafe, str, len);
    strsafe[len] = '\0';

    char *startOfInvalid = NULL;
    float num = strtof(strsafe, &startOfInvalid);
    if (startOfInvalid == strsafe) { return qdc_strToNumberErr_INVALID_NUMBER_FORMAT; }
    *destination = num;
    return qdc_strToNumberErr_NONE;
}

qdc_strToNumberErr qdc_strToDouble(const char *str, size_t len, double *destination) {
    char strsafe[len + 1];
    strncpy(strsafe, str, len);
    strsafe[len] = '\0';

    char *startOfInvalid = NULL;
    double num = strtod(strsafe, &startOfInvalid);
    if (startOfInvalid == strsafe) { return qdc_strToNumberErr_INVALID_NUMBER_FORMAT; }
    *destination = num;
    return qdc_strToNumberErr_NONE;
}
