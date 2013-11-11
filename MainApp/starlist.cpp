#include <stdlib.h>
#include "starlist.h"
#include "celestial.h"
#include "gps.h"

#include "starlist.inc"

#ifdef TESTBENCH
extern long time();
#else
# include "fbtimer.h"
#endif

const CStar* FindStar(const char* name)
{
    const long max_star_slots = sizeof(StarList) / sizeof(CStar*);
    for (long pos = 0; pos < max_star_slots; pos++) {
        if (!StarList[pos].star_name) {
            break;
        }
        if (!strcmp(StarList[pos].star_name, name)) {
            return &StarList[pos];
        }
    }
    return NULL;
}


int cached_star_count = -1;
int GetStarCount()
{
    if (cached_star_count < 0) {
	const long max_star_slots = sizeof(StarList) / sizeof(CStar*);
	int count = 0;
	for (long pos = 0; pos < max_star_slots; pos++) {
	    if (!StarList[pos].star_name) {
		break;
	    }
	    count++;
	}
	cached_star_count = count;
    }
    return cached_star_count;
}


int NextNamedVisibleBrightStar(int pos, int dir)
{
    int limit = GetStarCount();
    do {
        pos += dir;
        if (pos >= GetStarCount()) {
            pos = 0;
        } else if (pos < 0) {
            pos = GetStarCount() - 1;
        }
        const CStar* star = &StarList[pos];
        if (strcmp(star->star_name,"")) {
            if (star->visual_mag <= 2.50) {
                float alt, az;
                star->GetAltAz(time(), my_longitude, my_latitude, &alt, &az);
                if (alt > 0.0) {
                    return pos;
                }
            }
        }
    } while(limit-->0);
    return 0;
}


// If no stars closer than 5 degrees, return null.
const CStar* FindClosestStar(float ra, float dec, float* delta)
{
    int limit = GetStarCount();
    float angdist, min_angdist = 180.0;
    const CStar* beststar = NULL;
    for (int i = 0; i < limit; i++) {
        const CStar* star = &StarList[i];
	if (fabs(star->declination - dec) < 5.0) {
	    if (dec > 80.0 || dec < -80.0 || fabs(ra - star->right_ascension) < 5.0) {
	        angdist = AngularDistance(ra, dec, star->right_ascension, star->declination);
	        if (angdist < 5.0 && angdist < min_angdist) {
		    min_angdist = angdist;
		    beststar = star;
		}
	    }
	}
    }
    if (beststar) {
        *delta = min_angdist;
    }
    return beststar;
}


