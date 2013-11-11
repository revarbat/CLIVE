#include "clockutil.h"
#include "celestial.h"
#include "flarelist.h"
#include "fbtext.h"
#include "fbtimer.h"
#include "gpio.h"
#include "config.h"
#include "laser.h"
#include "gps.h"

int daytime_clock_pwm = 192;
int nighttime_clock_pwm = 4;

DayPartEnum curr_day_part;
extern CLaser* laser;


short CalcDigit(long &val, long div)
{
    short bitval = 8;
    short outval = 0;
    long  base = div << 3;
    while (bitval) {
        if (val >= base) {
            val -= base;
            outval += bitval;
        }
        bitval >>= 1;
        base >>= 1;
    }
    return outval;
}


void OutputTime(bool printalso, long seconds)
{
    if (seconds > 0) {
        unsigned short hours_tens, minutes_tens, seconds_tens;
        unsigned short hours_ones, minutes_ones;

        if (seconds < 360000) {
            hours_tens   = CalcDigit(seconds, 36000);
            hours_ones   = CalcDigit(seconds,  3600);
            minutes_tens = CalcDigit(seconds,   600);
            minutes_ones = CalcDigit(seconds,    60);
            seconds_tens = CalcDigit(seconds,    10);
        } else {
            hours_tens   = 9;
            hours_ones   = 9;
            minutes_tens = 9;
            minutes_ones = 9;
            seconds_tens = 9;
            seconds      = 9;
        }

        if (printalso) {
            txtPrintf("%d%d:%d%d:%d%d", hours_tens, hours_ones, minutes_tens, minutes_ones, seconds_tens, seconds);
        }

        GPIO_DATA(0) =
            (minutes_ones<<12) |
            (seconds_tens<<8) |
            (seconds<<4);
        GPIO_DATA(1) =
            (GPIO_DATA(1) & 0xf000) |
            (hours_tens<<8) |
            (hours_ones<<4) |
            minutes_tens;

    } else {
        if (printalso) {
            txtPrintf("        ");
        }
        GPIO_DATA(0) = 0xffff;
        GPIO_DATA(1) = 0x0fff;
    }
}


void UpdateClockBrightness()
{
    const long ramp_time = 1024;
    long now = time();
    short val;
    if (sunrise_time < now || sunset_time < now) {
        UpdateSunRiseSet(now+43200, my_latitude, my_longitude);
    }
    bool laseron = false;

    DayPartEnum new_day_part;
    long dv = daytime_clock_pwm - nighttime_clock_pwm;

    if (now < sunrise_time - ramp_time) {
        new_day_part = NIGHT;
        val = nighttime_clock_pwm;
        laseron = true;

    } else if (now < sunrise_time) {
        new_day_part = DAWN;
        long dt = now - sunrise_time;
        val = daytime_clock_pwm + ((dt * dv) >> 10);
        laseron = true;

    } else if (now < sunset_time) {
        new_day_part = DAY;
        val = daytime_clock_pwm;
        laseron = false;

    } else if (now < sunset_time + ramp_time) {
        new_day_part = DUSK;
        long dt = sunset_time - now;
        val = daytime_clock_pwm + ((dt * dv) >> 10);
        laseron = true;

    } else {
        new_day_part = NIGHT;
        val = nighttime_clock_pwm;
        laseron = true;
    }

    long time_left = TimeUntilNextFlare();
    if (time_left < FLARE_ALERT_END) {
        val = 1 + ((time_left * (val-1)) / FLARE_ALERT_END);
    }

    PWM_DATA(0) = val << 8;

    if (new_day_part != curr_day_part) {
        if (laseron) {
            laser->SetBeamState(BEAM_ON);
        } else {
            laser->SetBeamState(BEAM_OFF);
        }
        curr_day_part = new_day_part;
    }
}


void
TestClockDisplay()
{
    for (int i = 0; i <= 9; i++) {
        GPIO_DATA(0) = (i<<12) | (i<<8) | (i<<4);
        GPIO_DATA(1) = (i<<8) | (i<<4) | i;
        millisleep(512);
    }

    GPIO_DATA(1) = (1<<8) | (15<<4) | 15;
    GPIO_DATA(0) = (15<<12) | (15<<8) | (15<<4);
    millisleep(512);

    GPIO_DATA(1) = (15<<8) | (2<<4) | 15;
    GPIO_DATA(0) = (15<<12) | (15<<8) | (15<<4);
    millisleep(512);

    GPIO_DATA(1) = (15<<8) | (15<<4) | 3;
    GPIO_DATA(0) = (15<<12) | (15<<8) | (15<<4);
    millisleep(512);

    GPIO_DATA(1) = (15<<8) | (15<<4) | 15;
    GPIO_DATA(0) = (4<<12) | (15<<8) | (15<<4);
    millisleep(512);

    GPIO_DATA(1) = (15<<8) | (15<<4) | 15;
    GPIO_DATA(0) = (15<<12) | (5<<8) | (15<<4);
    millisleep(512);

    GPIO_DATA(1) = (15<<8) | (15<<4) | 15;
    GPIO_DATA(0) = (15<<12) | (15<<8) | (6<<4);
    millisleep(512);
}




