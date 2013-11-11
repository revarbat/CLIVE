#include <stdlib.h>
#include "constlines.h"

#include "constlines.inc"

int cached_constline_count = -1;
int GetConstellationLineCount()
{
    if (cached_constline_count < 0) {
        const long max_constline_slots = sizeof(ConstellationLineList) / sizeof(struct ConstellationLine);
        int count = 0;
        for (long pos = 0; pos < max_constline_slots; pos++) {
            if (!*ConstellationLineList[pos].name) {
                break;
            }
            count++;
        }
        cached_constline_count = count;
    }
    return cached_constline_count;
}


