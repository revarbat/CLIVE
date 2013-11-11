#ifndef CLOCKUTIL_H
#define CLOCKUTIL_H

#include "datetime.h"

void OutputTime(bool alsoprint, long seconds);
void UpdateClockBrightness();
void TestClockDisplay();

extern int daytime_clock_pwm;
extern int nighttime_clock_pwm;

enum DayPartEnum {
    UNSET, DAWN, DAY, DUSK, NIGHT
};

extern DayPartEnum curr_day_part;

#endif

