#include "star.h"

void CStar::GetAltAz(long when, float longitude, float latitude, float* alt, float* az) const {
    RADecToAltAz(when, longitude, latitude, right_ascension, declination, alt, az);
}



int star_sort_cmp(const void* a, const void* b)
{
    const CStar* pa = (const CStar*)a;
    const CStar* pb = (const CStar*)b;
    int cmp = strcasecmp(pa->star_name, pb->star_name);
    if (cmp != 0)
        return cmp;
    return (pa->right_ascension > pb->right_ascension);
}


