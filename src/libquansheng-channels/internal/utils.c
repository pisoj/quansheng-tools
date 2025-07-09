#include "utils.h"
#include <stdbool.h>

bool qdc_strIsEmpty(const char *str, size_t len) {
    if (*str == '\0') { return true; }
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
