#ifndef CELESTIAL_H
#define CELESTIAL_H

#include "orbitdata.h"
#include "matrixmath.h"

#define DmsToDeg(hours, minutes, seconds) ((hours < 0.0)? (hours - (minutes/60.0) - (seconds/3600.0)) : (hours + (minutes/60.0) + (seconds/3600.0)))
#define HmsToDeg(hours, minutes, seconds) (DmsToDeg(hours,minutes,seconds)*15.0)

extern long sunrise_time;
extern long sunset_time;

void AltAzToRADec(long when, float longitude, float latitude, float altitude, float azimuth, float* rightasc, float* declination);
void RADecToAltAz(long when, float longitude, float latitude, float rightasc, float declination, float* altitude, float* azimuth);
float AngularDistance(float ra1, float dec1, float ra2, float dec2);

float NormalizeRadians(float angle);
void OrbitToXYZ(const OrbitData& orbit, long when, float* x, float* y, float* z);
void SatelliteOrbitToEquatorial(const OrbitData& orbit, long when, float longitude, float latitude, float* right_ascension, float* declination);
void SolarOrbitToEquatorial(const OrbitData& orbit, const OrbitData& obs_orbit, long when, float* right_ascension, float* declination);
void SolarRADec(const OrbitData& obs_orbit, long when, float* right_ascension, float* declination);
void RiseTransitSet(long when, float lat, float lon, float ra, float dec, long* rise, long* transit, long* set);
void UpdateSunRiseSet(long when, float lat, float lon);

CMatrix TwoStarAlignMatrix(float real_alt_1, float real_az_1, float obs_alt_1, float obs_az_1, float real_alt_2, float real_az_2, float obs_alt_2, float obs_az_2);

CMatrix ThreeStarAlignMatrix(float real_alt_1, float real_az_1, float obs_alt_1, float obs_az_1, float real_alt_2, float real_az_2, float obs_alt_2, float obs_az_2, float real_alt_3, float real_az_3, float obs_alt_3, float obs_az_3);

void CompensateAltAz(const CMatrix& mat, float real_alt, float real_az, float* corr_alt, float* corr_az);



#endif

