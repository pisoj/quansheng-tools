#include "csv.h"

unsigned long qdc_csvGetColumnCount(const char *csvLine, unsigned long csvLineLen) {
    int columnCount = 0;
    const char *current = csvLine;
    while (current != NULL) {
        columnCount++;
        current = qdc_strFindNext(current, csvLineLen - (current - csvLine), ',');
    }
    return columnCount;
}
