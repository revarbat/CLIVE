#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "celestial.h"
#include "planet.h"
#include "planetlist.h"
#include "star.h"
#include "starlist.h"

//float my_latitude = 37.387883;
//float my_longitude = -122.0602;
float my_latitude = 52;
float my_longitude = -5;

long time()
{
    return time(NULL);
}


void
PrintRA(float ra)
{
    if (ra < 0.0) {
        ra += 360.0;
    }
    if (ra > 360.0) {
        ra = fmod(ra, 360.0); 
    }
    ra /= 15.0;
    short hours = (short)ra;
    ra -= hours;
    if (ra < 0.0) {
        ra = -ra;
    }
    ra *= 60.0;
    short minutes = (short)ra;
    ra -= minutes;
    ra *= 60.0;
    short seconds = (short)ra;
    printf("%4dh %2dm %2ds", hours, minutes, seconds);
}


void
PrintDec(float ra)
{
    short degs;
    if (ra < 0.0) {
        ra += 360.0;
    }
    if (ra > 360.0) {
        ra = fmod(ra, 360.0); 
    }
    if (ra > 180.0) {
        ra -= 360.0;
    }
    degs = (short)ra;
    ra -= degs;
    if (ra < 0.0) {
        ra = -ra;
    }
    ra *= 60.0;
    short minutes = (short)ra;
    ra -= minutes;
    ra *= 60.0;
    short seconds = (short)ra;
    printf("%4d˚ %2d' %2d\"", degs, minutes, seconds);
}


void
PrintAlt(float ra)
{
    short degs;
    degs = (short)ra;
    ra -= degs;
    if (ra < 0.0) {
        ra = -ra;
    }
    ra *= 60.0;
    short minutes = (short)ra;
    ra -= minutes;
    ra *= 60.0;
    short seconds = (short)ra;
    printf("%4d˚ %2d' %2d\"", degs, minutes, seconds);
}


void
PrintAz(float ra)
{
    short degs;
    while (ra < 0.0) {
        ra += 360.0;
    }
    if (ra > 360.0) {
        ra = fmod(ra, 360.0); 
    }
    degs = (short)ra;
    ra -= degs;
    if (ra < 0.0) {
        ra = -ra;
    }
    ra *= 60.0;
    short minutes = (short)ra;
    ra -= minutes;
    ra *= 60.0;
    short seconds = (short)ra;
    printf("%4d˚ %2d' %2d\"", degs, minutes, seconds);
}


void PrintSolarInfo(long when, CPlanet* earth) {
    time_t lt;
    struct tm *time_tm;
    char timebuf[1024];
    char risebuf[1024];
    char setbuf[1024];
    float sun_ra, sun_dec;
    float sun_az, sun_alt;

    SolarRADec(*earth->GetOrbit(), when, &sun_ra, &sun_dec);
    RADecToAltAz(when, my_longitude, my_latitude, sun_ra, sun_dec, &sun_alt, &sun_az);

    UpdateSunRiseSet(when, my_latitude, my_longitude);

    lt = when;
    time_tm = localtime(&lt);
    strftime(timebuf, sizeof(timebuf), "%c %Z", time_tm);

    lt = sunrise_time;
    time_tm = localtime(&lt);
    strftime(risebuf, sizeof(risebuf), "%c %Z", time_tm);

    lt = sunset_time;
    time_tm = localtime(&lt);
    strftime(setbuf, sizeof(setbuf), "%c %Z", time_tm);

    printf("Sun (today, %s):\n", timebuf);
    printf("  ra  = ");  PrintRA(sun_ra);    printf("\n");
    printf("  dec = ");  PrintDec(sun_dec);  printf("\n");
    printf("  az  = ");  PrintAz(sun_az);    printf("\n");
    printf("  alt = ");  PrintAlt(sun_alt);  printf("\n");
    printf("  next rise  = %s\n", risebuf);
    printf("  next set   = %s\n", setbuf);
    printf("\n");
}


void PrintStarInfo(long when, const CStar* star) {
    time_t lt;
    struct tm *time_tm;
    long rise, transit, set;
    float ra, dec;
    float alt, az;
    char timebuf[1024];
    char risebuf[1024];
    char transbuf[1024];
    char setbuf[1024];

    star->GetAltAz(when, my_longitude, my_latitude, &alt, &az);
    ra = star->right_ascension;
    dec = star->declination;
    RiseTransitSet(when, my_latitude, my_longitude, ra, dec, &rise, &transit, &set);

    lt = when;
    time_tm = localtime(&lt);
    strftime(timebuf, sizeof(timebuf), "%c %Z", time_tm);

    lt = rise;
    time_tm = localtime(&lt);
    strftime(risebuf, sizeof(risebuf), "%c", time_tm);

    lt = transit;
    time_tm = localtime(&lt);
    strftime(transbuf, sizeof(transbuf), "%c", time_tm);

    lt = set;
    time_tm = localtime(&lt);
    strftime(setbuf, sizeof(setbuf), "%c", time_tm);

    printf("%s: (at %s)\n", star->GetName(), timebuf);
    printf("  alt   = ");  PrintAlt(alt);  printf("\n");
    printf("  az    = ");  PrintAz(az);    printf("\n");
    printf("  rise  = %s\n", risebuf);
    printf("  trans = %s\n", transbuf);
    printf("  set   = %s\n", setbuf);
    printf("\n");
}


void PrintOrbitInfo(long when, const OrbitData* orbit) {
    float a, e, i, argperi, AN, M0, n, M;

    a = orbit->SemiMajorAxis(when);
    e = orbit->Eccentricity(when);
    i = orbit->OrbitalInclination(when);
    AN = orbit->LongitudeOfAscendingNode(when);
    argperi = orbit->ArgumentOfPeriapsis(when);
    M0 = orbit->MeanAnomaly0(when);
    n = orbit->MeanDailyMotion(when);
    M = orbit->MeanAnomaly(when);

    printf("  a     = %g\n", a);
    printf("  e     = %g\n", e);
    printf("  i     = %g\n", i);
    printf("  aperi = %g\n", argperi);
    printf("  AN    = %g\n", AN);
    printf("  M0    = %g\n", M0);
    printf("  n     = %g\n", n);
    printf("  M     = %g\n", M);
}



void PrintPlanetInfo(long when, const CPlanet* planet) {
    time_t lt;
    struct tm *time_tm;
    long rise, transit, set;
    float ra, dec;
    float alt, az;
    char timebuf[1024];
    char risebuf[1024];
    char transbuf[1024];
    char setbuf[1024];
    CPlanet* earth = FindPlanet("Earth");

    SolarOrbitToEquatorial(*planet->GetOrbit(), *earth->GetOrbit(), when, &ra, &dec);
    planet->GetAltAz(when, *earth->GetOrbit(), my_longitude, my_latitude, &alt, &az);
    RiseTransitSet(when, my_latitude, my_longitude, ra, dec, &rise, &transit, &set);

    lt = when;
    time_tm = localtime(&lt);
    strftime(timebuf, sizeof(timebuf), "%c %Z", time_tm);

    lt = rise;
    time_tm = localtime(&lt);
    strftime(risebuf, sizeof(risebuf), "%c", time_tm);

    lt = transit;
    time_tm = localtime(&lt);
    strftime(transbuf, sizeof(transbuf), "%c", time_tm);

    lt = set;
    time_tm = localtime(&lt);
    strftime(setbuf, sizeof(setbuf), "%c", time_tm);

    printf("%s: (at %s)\n", planet->GetName(), timebuf);
    //PrintOrbitInfo(when, planet->GetOrbit());
    printf("  ra    = ");  PrintRA(ra);    printf("\n");
    printf("  dec   = ");  PrintDec(dec);  printf("\n");
    printf("  alt   = ");  PrintAlt(alt);  printf("\n");
    printf("  az    = ");  PrintAz(az);    printf("\n");
    printf("  rise  = %s\n", risebuf);
    printf("  trans = %s\n", transbuf);
    printf("  set   = %s\n", setbuf);
    printf("\n");
}



void
AlignTest2(float alt1, float az1, float oalt1, float oaz1, float alt2, float az2, float oalt2, float oaz2, float star_alt, float star_az)
{
    float alt, az;
    CMatrix T(3,3);

    T = TwoStarAlignMatrix(alt1, az1, oalt1, oaz1, alt2, az2, oalt2, oaz2);
    CompensateAltAz(T, star_alt, star_az, &alt, &az);
    printf("Corr:\n");
    printf("  star1 alt = %4.1f    az = %4.1f\n", alt1, az1);
    printf("  obs1  alt = %4.1f    az = %4.1f\n", oalt1, oaz1);
    printf("  star2 alt = %4.1f    az = %4.1f\n", alt2, az2);
    printf("  obs2  alt = %4.1f    az = %4.1f\n\n", oalt2, oaz2);

    printf("  init  alt = ");  PrintDec(star_alt);
    printf("    az = ");  PrintDec(star_az);  printf("\n");
    printf("  corr  alt = ");  PrintDec(alt);
    printf("    az = ");  PrintDec(az);  printf("\n");
    printf("\n");
}

void
AlignTest3(float alt1, float az1, float oalt1, float oaz1, float alt2, float az2, float oalt2, float oaz2, float alt3, float az3, float oalt3, float oaz3, float star_alt, float star_az)
{
    float alt, az;
    CMatrix T(3,3);

    T = ThreeStarAlignMatrix(alt1, az1, oalt1, oaz1, alt2, az2, oalt2, oaz2, alt3, az3, oalt3, oaz3);
    CompensateAltAz(T, star_alt, star_az, &alt, &az);
    printf("Corr:\n");
    printf("  star1 alt = %4.1f    az = %4.1f\n", alt1, az1);
    printf("  obs1  alt = %4.1f    az = %4.1f\n", oalt1, oaz1);
    printf("  star2 alt = %4.1f    az = %4.1f\n", alt2, az2);
    printf("  obs2  alt = %4.1f    az = %4.1f\n", oalt2, oaz2);
    printf("  star3 alt = %4.1f    az = %4.1f\n", alt3, az3);
    printf("  obs3  alt = %4.1f    az = %4.1f\n\n", oalt3, oaz3);

    printf("  init  alt = ");  PrintDec(star_alt);
    printf("    az = ");  PrintDec(star_az);  printf("\n");
    printf("  corr  alt = ");  PrintDec(alt);
    printf("    az = ");  PrintDec(az);  printf("\n");
    printf("\n");
}

int
main(int argc, char** argv)
{
    long when = ((2004-1970)*365 + 8) * 86400;

    CPlanet* mercury = FindPlanet("Mercury");
    CPlanet* venus = FindPlanet("Venus");
    CPlanet* earth = FindPlanet("Earth");
    CPlanet* mars = FindPlanet("Mars");
    CPlanet* jupiter = FindPlanet("Jupiter");
    CPlanet* saturn = FindPlanet("Saturn");
    CPlanet* uranus = FindPlanet("Uranus");
    CPlanet* neptune = FindPlanet("Neptune");
    CPlanet* pluto = FindPlanet("Pluto");
    CPlanet* sedna = FindPlanet("Sedna");

    const CStar* deneb = FindStar("Deneb");
    const CStar* arcturus = FindStar("Arcturus");
    const CStar* polaris = FindStar("Polaris");

    /*
    PrintPlanetInfo(when, mars);
    PrintPlanetInfo(when, jupiter);
    PrintStarInfo(when, deneb);
    PrintStarInfo(when, polaris);
    */



    /*
    AlignTest2( 0.0,  90.0,  0.0,  90.0,  0.0, 180.0,  0.0, 180.0, 45.0, 270.0);
    AlignTest2(45.0,  45.0, 45.0,  46.0, 13.0, 310.0, 13.0, 311.0, 58.0, 180.0);
    AlignTest2(60.0, 180.0, 59.0, 180.0,  0.0,  90.0,  0.0,  90.0, 45.0,  90.0);
    AlignTest2(60.0, 180.0, 61.0, 180.0,  0.0,  90.0,  0.0,  90.0, 45.0,  90.0);
    AlignTest2(45.0,   0.0, 46.0,   0.0,  0.0,  90.0,  0.0,  90.0, 45.0, 270.0);
    AlignTest2(45.0,   0.0, 46.0,   0.0, 45.0, 180.0, 44.0, 180.0, 45.0, 270.0);
    AlignTest2(45.0,  60.0, 46.0,  60.0, 45.0, 300.0, 44.0, 300.0, 45.0, 180.0);
    AlignTest2(45.0,  90.0, 46.0,  90.0, 45.0, 270.0, 44.0, 270.0, 45.0, 180.0);
    AlignTest2(23.0,   0.3, 23.0,  30.3, 85.0, 193.0, 85.0, 223.0, 58.0, 123.0);
    AlignTest3(23.0,   0.3, 23.0,  30.3, 85.0, 193.0, 85.0, 223.0, 19.0, 304.0, 19.0, 334.0, 58.0, 123.0);
    AlignTest2(23.0,   0.3, 23.0,   0.3, 85.0, 193.0, 86.0, 193.0, 58.0, 123.0);
    AlignTest2(23.0,   0.3, 22.0,   0.3, 84.0, 193.0, 85.0, 193.0, 58.0, 123.0);
    AlignTest3(23.0,   0.3, 22.0,   0.3, 84.0, 193.0, 85.0, 193.0, 19.0, 304.0, 18.0, 304.0, 58.0, 123.0);
    AlignTest2(38.0,   0.3, 38.6,   0.3, 42.2, 102.0, 42.1, 101.1, 10.9, 272.9);
    */



    when = time(NULL);
    //when = 1083369600;
    my_latitude = 37.236496;
    my_longitude = -121.897709;

    printf("my lat   = ");  PrintDec(my_latitude);  printf("\n");
    printf("my lon   = ");  PrintDec(my_longitude);  printf("\n\n");

    CDateTime dt(when);
    float hourangle = dt.GetLocalSidereal(my_longitude);
    printf("hour angle = ");  PrintAz(hourangle);    printf("\n");
    printf("hour angle = ");  PrintRA(hourangle);    printf("\n");

    PrintSolarInfo(when, earth);

    PrintPlanetInfo(when, mercury);
    PrintPlanetInfo(when, venus);
    PrintPlanetInfo(when, mars);
    PrintPlanetInfo(when, jupiter);
    PrintPlanetInfo(when, saturn);
    PrintPlanetInfo(when, uranus);
    PrintPlanetInfo(when, neptune);
    PrintPlanetInfo(when, pluto);
    PrintPlanetInfo(when, sedna);

    PrintStarInfo(when, polaris);
    PrintStarInfo(when, deneb);
    PrintStarInfo(when, arcturus);

    return 0;
}

