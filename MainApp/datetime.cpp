#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datetime.h"
#include "fbtext.h"

long tz_offset = -7 * 3600;
const float dayspersec = 1.0 / 86400.0;
const float hourspersec = 1.0 / 3600.0;

bool YearIsLeapYear(short year)
{
    if ((year & 0x3) == 0) {
        return true;
    }
    return false;
}


short DaysInYear(short year)
{
    if (YearIsLeapYear(year)) {
        return 366;
    }
    return 365;
}


short DaysInMonth(short year, short month)
{
    switch (month) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
            break;

        case 2:
            if (YearIsLeapYear(year)) {
                return 29;
            } else {
                return 28;
            }
            break;

        default:
            return 30;
    }
}



CDateTime::CDateTime()
{
    Set(0);
}


CDateTime::CDateTime(short year, short month, short day, short hour, short minute, short second)
{
    Set(year, month, day, hour, minute, second);
}


CDateTime::CDateTime(long when)
{
    Set(when);
}


void CDateTime::RecalcFromParts()
{
    long days = 0;
    long systime = 0;
    short year, month, day, hour, minute, second;

    year   = dt_year;
    month  = dt_month;
    day    = dt_day;
    hour   = dt_hour;
    minute = dt_minute;
    second = dt_second;

    if (year < 100) {
        year += 2000;
    }
    if (year > 1970) {
        for (short i = year-1; i >= 1970; i--) {
            days += DaysInYear(i);
        }
        for (short i = month-1; i >= 1; i--) {
            days += DaysInMonth(year, i);
        }
        days += (day - 1);
        systime = days * 86400;
        systime += hour * 3600;
        systime += minute * 60;
        systime += second;
    } else {
        for (short i = year+1; i < 1970; i++) {
            days -= DaysInYear(i);
        }
        for (short i = month+1; i <= 12; i++) {
            days -= DaysInMonth(year, i);
        }
        days -= (day - 1);
        systime = days * 86400;
        systime += (23-hour) * 3600;
        systime += (59-minute) * 60;
        systime += (59-second);
    }
    dt_dayofweek = (((systime + tz_offset) / 86400) % 7) - 3;
    if (dt_dayofweek < 0) {
        dt_dayofweek += 7;
    }
    dt_systime = systime;
}

void CDateTime::Set(short year, short month, short day, short hour, short minute, short second)
{
    dt_year   = year;
    dt_month  = month;
    dt_day    = day;
    dt_hour   = hour;
    dt_minute = minute;
    dt_second = second;

    RecalcFromParts();
    dt_systime -= tz_offset;
    RecalcFromSystime();
}

void CDateTime::SetGMT(short year, short month, short day, short hour, short minute, short second)
{
    dt_year   = year;
    dt_month  = month;
    dt_day    = day;
    dt_hour   = hour;
    dt_minute = minute;
    dt_second = second;

    RecalcFromParts();
    RecalcFromSystime();
}



void CDateTime::RecalcFromSystime()
{
    const long secsinleapblock = ((8*365)+2) * 86400;
    long when = dt_systime + tz_offset;
    long secs;

    if (when >= 0) {
        dt_year = 1970;
        while (secsinleapblock <= when) {
            dt_year += 8;
            when -= secsinleapblock;
        }
        while ((secs = DaysInYear(dt_year) * 86400) <= when) {
            dt_year++;
            when -= secs;
        }
        dt_month = 1;
        while ((secs = DaysInMonth(dt_year, dt_month) * 86400) <= when) {
            dt_month++;
            when -= secs;
        }
        dt_day = 1;
        dt_day += (when / 86400);
        when %= 86400;
        dt_hour = (when / 3600);
        when %= 3600;
        dt_minute = (when / 60);
        when %= 60;
        dt_second = when;
    } else {
        dt_year = 1969;
        while (secsinleapblock <= -when) {
            dt_year -= 8;
            when += secsinleapblock;
        }
        while ((secs = DaysInYear(dt_year) * 86400) <= -when) {
            dt_year--;
            when += secs;
        }
        dt_month = 12;
        while ((secs = DaysInMonth(dt_year, dt_month) * 86400) <= -when) {
            dt_month--;
            when += secs;
        }
        dt_day = DaysInMonth(dt_year, dt_month);
        dt_day += (when / 86400);
        when %= 86400;
        dt_hour = (when / 3600);
        when %= 3600;
        dt_minute = (when / 60);
        when %= 600;
        dt_second = -when;
    }
    
    dt_dayofweek = (((dt_systime + tz_offset) / 86400) % 7) - 3;
    if (dt_dayofweek < 0) {
        dt_dayofweek += 7;
    }
}


void CDateTime::Set(long when)
{
    dt_systime = when;
    RecalcFromSystime();
}


float CDateTime::GetDaysSinceJ2000() const
{
    const long j2k_systime = (((30 * 365) + 7) * 86400) + 43200;
    float dsj2k = (dt_systime - j2k_systime) * dayspersec;
    return dsj2k;
}


float CDateTime::GetLocalSidereal(float longitude) const
{
    long days = (long)(dt_systime * dayspersec);
    long remsecs = dt_systime - (days * 86400);
    float uth = remsecs * hourspersec;
    float lst = 100.46 + (0.985647 * GetDaysSinceJ2000()) + longitude + (15 * uth);
    while (lst < 0.0) {
        lst += 360.0;
    }
    while (lst >= 360.0) {
        lst -= 360.0;
    }
    return lst;
}


void int_to_str(long val, bool zeropad, int chars, char* buf, int buflen)
{
    int digits, i;
    bool isneg = val < 0;
    val = abs(val);
    for (digits = 0, i = 1; i <= val; i *= 10) {
        digits++;
    }
    if (digits < 1) {
        digits = 1;
    }
    if (chars > digits) {
        digits = chars;
    }
    short pos = digits;
    if (isneg) {
        pos++;
    }
    if (pos < buflen) {
        buf[pos] = '\0';
    }
    while (pos-->0) {
        if (pos == buflen-1) {
            buf[pos] = '\0';
        } else if (pos < buflen-1) {
            if (val || pos == digits-1) {
                short digit = val % 10;
                buf[pos] = '0' + digit;
            } else {
                if (zeropad) {
                    buf[pos] = '0';
                } else {
                    buf[pos] = ' ';
                }
            }
        }
        val /= 10;
    }
    if (isneg) {
        buf[0] = '-';
    }
}


void CDateTime::GetTimeString(char* buf, long buflen) const
{
    bool ispm = false;
    if (dt_hour > 11) {
        ispm = true;
    }
    short hour = dt_hour;
    if (dt_hour == 0) {
        hour = 12;
    } else if (dt_hour > 12) {
        hour = dt_hour - 12;
    }
        
    int_to_str(hour, false, 2, buf, buflen);
    buf[2] = ':';
    buflen -= 3;
    int_to_str(abs(dt_minute), true, 2, buf+3, buflen);
    buf[5] = ':';
    buflen -= 3;
    int_to_str(abs(dt_second), true, 2, buf+6, buflen);
    buflen -= 3;
    strncpy(buf+8, (ispm? "p" : "a"), buflen);
}


void CDateTime::GetString(char* buf, long buflen) const
{
    const char *dayname[] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};
    const char *monthname[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    strncpy(buf, dayname[dt_dayofweek], buflen);
    buf[2] = ' ';
    buflen -= 3;
    strncpy(buf+3, monthname[dt_month-1], buflen);
    buflen -= 3;
    int_to_str(dt_day, false, 2, buf+6, buflen);
    buf[8] = ' ';
    buflen -= 3;

    GetTimeString(buf+9, buflen);
}



