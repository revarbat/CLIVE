#ifndef STARLIST_H
#define STARLIST_H

#include "star.h"

extern const CStar StarList[];

int GetStarCount();
const CStar* FindStar(const char* name);
int NextNamedVisibleBrightStar(int pos, int dir);
const CStar* FindClosestStar(float ra, float dec, float* delta);

#endif

