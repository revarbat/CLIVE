#ifndef DATETIME_H
#define DATETIME_H

extern long tz_offset;

class CDateTime {
public:
    CDateTime();
    CDateTime(short year, short month, short day, short hour, short minute, short second);
    CDateTime(long when);

    void Set(short year, short month, short day, short hour, short minute, short second);
    void SetGMT(short year, short month, short day, short hour, short minute, short second);
    void Set(long when);
    void GetString(char* buf, long buflen) const;
    void GetTimeString(char* buf, long buflen) const;
    void RecalcFromSystime();
    void RecalcFromParts();

    long GetSystime() const { return dt_systime; }
    float GetDaysSinceJ2000() const;
    float GetLocalSidereal(float longitude) const;

    short dt_year, dt_month, dt_day, dt_hour, dt_minute, dt_second, dt_dayofweek;
    long dt_systime;
};

#endif

