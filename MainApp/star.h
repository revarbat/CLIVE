#ifndef STAR_H
#define STAR_H

#include <stdlib.h>
#include <string.h>
#include "datetime.h"
#include "celestial.h"

struct CStar {
public:
    char* star_name;
    char* constellation;
    float visual_mag;
    float right_ascension;
    float declination;

    const char* GetName() const { return star_name; }
    void GetAltAz(long when, float longitude, float latitude, float* alt, float* az) const;
};


int star_sort_cmp(const void* a, const void* b);


#endif
