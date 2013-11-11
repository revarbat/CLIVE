#ifndef FLARELIST_H
#define FLARELIST_H

#include "flare.h"
#include "laser.h"

extern const CFlare* FlareList[];

extern const CFlare* nextflare;
extern CDateTime NextFlareTime;

long TimeUntilNextFlare();
const CFlare* GetNextFlareAfter(long when);
void CheckForNextFlare(CLaser *laser);
void UpdateTimeLeftClockDisplay(bool alsoprint);

#endif

