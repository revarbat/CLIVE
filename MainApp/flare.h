#ifndef FLARE_H
#define FLARE_H

#include "datetime.h"
#include "clockutil.h"

class CFlare {
public:
    CFlare(long when, int mag, float alt, float az, int satnum)
        : flare_dt(when)
    {
        flare_time = when;
        flare_magnitude = mag;
        flare_altitude = alt;
        flare_azimuth = az;
        satellite_number = satnum;
        flare_dt.GetString(flare_dt_str, sizeof(flare_dt_str));
    }

    CFlare(const CDateTime &dt, int mag, float alt, float az, int satnum)
        : flare_dt(dt)
    {
        flare_time = dt.GetSystime();
        flare_magnitude = mag;
        flare_altitude = alt;
        flare_azimuth = az;
        satellite_number = satnum;
        flare_dt.GetString(flare_dt_str, sizeof(flare_dt_str));
    }

    const CDateTime& GetDateTime() const { return flare_dt; }
    const char* GetDateString() const { return flare_dt_str; }
    long  GetTime() const { return flare_time; }
    float GetAzimuth() const { return flare_azimuth; }
    float GetAltitude() const { return flare_altitude; }
    int   GetMagnitude() const { return flare_magnitude; }
    int   GetSatNum() const { return satellite_number; }

private:
    long  flare_time;
    long  flare_magnitude;
    float flare_altitude;
    float flare_azimuth;
    int   satellite_number;
    CDateTime flare_dt;
    char flare_dt_str[32];

};


#endif
