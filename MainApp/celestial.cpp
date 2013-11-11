#include <stdio.h>
#include <math.h>
#include "celestial.h"
#include "datetime.h"
#include "fasttrig.h"
#include "planet.h"
#include "planetlist.h"
#include "matrixmath.h"


long sunrise_time = 0;
long sunset_time = 0;


const float deg2rad = pi / 180.0;
const float rad2deg = 180.0 / pi;
const float rad2hour = rad2deg / 15.0;
const float sec2day = 1.0 / 86400.0;

#define PI 3.14159265358979323
#define THRESH 1.0e-6
#define CUBE_ROOT( X)  (expf( logf( X) / 3.))

static float
kepler(const float ecc, float mean_anom)
{
	float curr, err, thresh;
	int is_negative = 0, n_iter = 0;

	if( !mean_anom) {
		return( 0.);
	}

	if( ecc < .3) {   /* low-eccentricity formula from Meeus,  p. 195 */
		curr = atan2f( sinf( mean_anom), cosf( mean_anom) - ecc);
			/* one correction step,  and we're done */
		err = curr - ecc * sinf( curr) - mean_anom;
		curr -= err / (1. - ecc * cosf( curr));
	} else {
	    curr = mean_anom;
	}

	if( mean_anom < 0.) {
		mean_anom = -mean_anom;
		curr = - curr;
		is_negative = 1;
	}

	thresh = THRESH * fabsf( 1. - ecc);
	if( ecc > .8 && mean_anom < PI / 3. || ecc > 1.)    /* up to 60 degrees */ {
		float trial = mean_anom / fabsf( 1. - ecc);

		if( trial * trial > 6. * fabsf(1. - ecc))   /* cubic term is dominant */ {
			if( mean_anom < PI) {
				trial = CUBE_ROOT( 6. * mean_anom);
			} else {      /* hyperbolic w/ 5th & higher-order terms predominant */
				trial = asinhf( mean_anom / ecc);
			}
		}
		curr = trial;
	}

	if( ecc < 1.) {
		err = curr - ecc * sinf( curr) - mean_anom;
		while( fabsf( err) > thresh) {
			n_iter++;
			curr -= err / (1. - ecc * cosf( curr));
			err = curr - ecc * sinf( curr) - mean_anom;
		}
	} else {
		err = ecc * sinhf( curr) - curr - mean_anom;
		while( fabsf( err) > thresh) {
			n_iter++;
			curr -= err / (ecc * coshf( curr) - 1.);
			err = ecc * sinhf( curr) - curr - mean_anom;
		}
	}
	return( is_negative ? -curr : curr);
}


void
AltAzToRADec(long when, float lon, float lat, float alt, float az, float *ra, float *dec)
{
	float ha_rad;

	float lat_rad = lat * deg2rad;
	float alt_rad = alt * deg2rad;
	float az_rad  = az  * deg2rad;

	float sin_lat = sinf(lat_rad);
	float cos_lat = cosf(lat_rad);
	float sin_alt = sinf(alt_rad);
	float cos_alt = cosf(alt_rad);
	float sin_az  = sinf(az_rad);
	float cos_az  = cosf(az_rad);

	float sin_dec = sin_alt * sin_lat + cos_alt * cos_lat * cos_az;
	float dec_rad = asinf(sin_dec);
	float cos_dec = cosf(dec_rad);
	*dec = dec_rad * rad2deg;

	float x = (sin_alt - sin_lat * sin_dec) / (cos_lat * cos_dec);

	if (x < -1.0) {
		ha_rad = pi;
	} else if (x > 1.0) {
		ha_rad = 0.0;
	} else {
		ha_rad = acosf(x);
	}

	if (sin_az > 0.0) {
		ha_rad = (2.0 * pi) - ha_rad;
	}

	CDateTime dt(when);
	*ra = dt.GetLocalSidereal(lon) - (ha_rad * rad2deg);
	while (*ra < 0.0) {
		*ra += 360.0;
	}
	while (*ra >= 360.0) {
		*ra -= 360.0;
	}
}


void
RADecToAltAz(long when, float longitude, float latitude, float rightasc, float declination, float* altitude, float* azimuth)
{
	CDateTime dt(when);
	float hourangle = dt.GetLocalSidereal(longitude) - rightasc;

	while (hourangle < 0.0) {
		hourangle += 360.0;
	}

	longitude = longitude * deg2rad;
	latitude = latitude * deg2rad;
	declination = declination * deg2rad;
	hourangle = hourangle * deg2rad;

	float alt = asinf(sinf(latitude)*sinf(declination)+cosf(latitude)*cosf(declination)*cosf(hourangle)) * rad2deg;
	float Y = sinf(hourangle);
	float X = cosf(hourangle) * sinf(latitude) - tanf(declination) * cosf(latitude);
	float az = atan2f(Y,X) * rad2deg + 180.0;
	*altitude = alt;
	*azimuth = az;
}


float NormalizeRadians(float angle)
{
	angle = fmod(angle,2.0*pi);
	if (angle < 0.0) {
		angle += 2.0 * pi;
	}
	return angle;
}


void OrbitToXYZ(
	const OrbitData& orbit,
	long when,
	float* x,
	float* y,
	float* z
	)
{
	float true_anomaly;

	float inclination = orbit.OrbitalInclination(when) * deg2rad;
	float ascnode = orbit.LongitudeOfAscendingNode(when) * deg2rad;
	float peri = orbit.ArgumentOfPeriapsis(when) * deg2rad;
	float mean_anomaly = orbit.MeanAnomaly(when) * deg2rad;

	float semimajor = orbit.SemiMajorAxis(when);
	float eccentricity = orbit.Eccentricity(when);

	mean_anomaly = NormalizeRadians(mean_anomaly);
	float eccentric_anomaly = kepler(eccentricity, mean_anomaly);
	eccentric_anomaly = NormalizeRadians(eccentric_anomaly);
	true_anomaly = 2*atan2f(sqrtf((1+eccentricity)/(1-eccentricity)),1.0/tanf(eccentric_anomaly*0.5));

	float radius_orbit = semimajor * (1 - eccentricity * eccentricity) / (1 + eccentricity * cosf(true_anomaly));

	*x = radius_orbit * (cosf(ascnode) * cosf(peri+true_anomaly) - sinf(ascnode) * cosf(inclination) * sinf(peri+true_anomaly));
	*y = radius_orbit * (sinf(ascnode) * cosf(peri+true_anomaly) + cosf(ascnode) * cosf(inclination) * sinf(peri+true_anomaly));
	*z = radius_orbit * sinf(inclination) * sinf(peri+true_anomaly);
}


void SolarOrbitToEquatorial(
	const OrbitData& orbit,
	const OrbitData& obs_orbit,
	long when,
	float* right_ascension,
	float* declination
	)
{
	float tilt = obs_orbit.RotationalTilt() * deg2rad;
	float obs_x, obs_y, obs_z;
	float obj_x, obj_y, obj_z;

	OrbitToXYZ(    orbit, when, &obj_x, &obj_y, &obj_z);
	OrbitToXYZ(obs_orbit, when, &obs_x, &obs_y, &obs_z);

	float dx, dy, dz;
	dx = obj_x - obs_x;
	dy = obj_y - obs_y;
	dz = obj_z - obs_z;

	float delta = sqrtf((dx*dx)+(dy*dy)+(dz*dz));
	float geo_lon = atan2f(dy,dx);
	float geo_lat = asinf(dz/delta);

	*right_ascension = atan2f(sinf(geo_lon)*cosf(tilt) - tanf(geo_lat)*sinf(tilt), cosf(geo_lon)) * rad2deg;
	*declination = asinf(sinf(geo_lat)*cosf(tilt) + cosf(geo_lat)*sinf(tilt)*sinf(geo_lon)) * rad2deg;
}


// Returns apparent right ascension and declination, from the observer's point of view.
void SatelliteOrbitToEquatorial(
	const OrbitData& orbit,
	long when,
	float longitude, float latitude,
	float* right_ascension,
	float* declination
	)
{
	float obs_x, obs_y, obs_z;
	float obj_x, obj_y, obj_z;
	float longrad = longitude*deg2rad;
	float latrad = latitude*deg2rad;

	// TODO: Should probably implement SG4 algorithm for calculating
	// satellite positions.  More accurate.  Takes drag into account.
	OrbitToXYZ(    orbit, when, &obj_x, &obj_y, &obj_z);

	// Position of viewer at given latitude and longitude.
	// Unit of distance is Earth radiuses.
	obs_x = cosf(latrad) * cosf(longrad);
	obs_y = cosf(latrad) * sinf(longrad);
	obs_z = sinf(latrad);

	float dx, dy, dz;
	dx = obj_x - obs_x;
	dy = obj_y - obs_y;
	dz = obj_z - obs_z;

	float delta = sqrtf((dx*dx)+(dy*dy)+(dz*dz));
	float geo_lon = atan2f(dy,dx);
	float geo_lat = asinf(dz/delta);

	*right_ascension = geo_lon * rad2deg;
	*declination = geo_lat * rad2deg;
}


void SolarRADec(
	const OrbitData& obs_orbit,
	long when,
	float* right_ascension,
	float* declination
	)
{
	float tilt = obs_orbit.RotationalTilt() * deg2rad;
	float x, y, z;

	OrbitToXYZ(obs_orbit, when, &x, &y, &z);
	x = -x;
	y = -y;
	z = -z;

	float delta = sqrtf((x*x)+(y*y)+(z+z));
	float geo_lon = atan2f(y,x);
	float geo_lat = asinf(z/delta);

	float ra = atan2f(sinf(geo_lon)*cosf(tilt) - tanf(geo_lat)*sinf(tilt), cosf(geo_lon)) * rad2deg;
	float dec = asinf(sinf(geo_lat)*cosf(tilt) + cosf(geo_lat)*sinf(tilt)*sinf(geo_lon)) * rad2deg;

	if (ra < 0) {
		ra += 360.0;
	}

	*right_ascension = ra;
	*declination = dec;
}

#include "fbtext.h"

void RiseTransitSet(long when, float lat, float lon, float ra, float dec, long* rise, long* transit, long* set)
{
	long daybase = when - (when % 86400);
	CPlanet* earth = FindPlanet("Earth");
	float full_set_angle = -0.83 * deg2rad;
	float latrad = lat * deg2rad;
	float decrad = dec * deg2rad;
	const OrbitData* obs_orbit = earth->GetOrbit(); 

	float mean_anomaly = obs_orbit->MeanAnomaly(when);
	float transit_hour_utc = fmod((ra - lon - mean_anomaly),360.0)/15 + 17.1375;
	long transit_time = (long)(transit_hour_utc * 3600.0);

	float horizon_hour = acosf((sinf(full_set_angle) - sinf(latrad)*sinf(decrad))/(cosf(latrad)*cosf(decrad))) * rad2hour;
	long horizon_time = (long)(horizon_hour * 3600);

	transit_time += daybase;
	*rise = transit_time - horizon_time;
	*transit = transit_time;
	*set = transit_time + horizon_time;
}


void SunRiseSet(long when, float lat, float lon, long* out_rise, long* out_set, long* out_trans)
{
	float ra, dec;
	long dummy, rise, set, transit;
	CPlanet* earth = FindPlanet("Earth");

	SolarRADec(*earth->GetOrbit(), when, &ra, &dec);
	RiseTransitSet(when, lat, lon, ra, dec, &rise, &dummy, &set);

	SolarRADec(*earth->GetOrbit(), rise, &ra, &dec);
	RiseTransitSet(rise, lat, lon, ra, dec, &rise, &dummy, &dummy);

	SolarRADec(*earth->GetOrbit(), transit, &ra, &dec);
	RiseTransitSet(rise, lat, lon, ra, dec, &dummy, &transit, &dummy);

	SolarRADec(*earth->GetOrbit(), set, &ra, &dec);
	RiseTransitSet(set, lat, lon, ra, dec, &dummy, &dummy, &set);

	*out_rise = rise;
	*out_trans = transit;
	*out_set = set;
}


void UpdateSunRiseSet(long when, float lat, float lon)
{
	long rise, set, transit;
	SunRiseSet(when, lat, lon, &rise, &set, &transit);
	sunrise_time = rise;
	sunset_time = set;
}


float AngularDistance(float ra1, float dec1, float ra2, float dec2)
{
	float ra1_rad  = ra1  * deg2rad;
	float ra2_rad  = ra2  * deg2rad;
	float dec1_rad = dec1 * deg2rad;
	float dec2_rad = dec2 * deg2rad;

	float dra  = ra1_rad  - ra2_rad;
	float ddec = dec1_rad - dec2_rad;

	float sin_ra  = sinf(dra  * 0.5);
	float sin_dec = sinf(ddec * 0.5);

	float sqr_ra  = sin_ra  * sin_ra;
	float sqr_dec = sin_dec * sin_dec;

	float aux = sqr_dec + cosf(dec1_rad) * cosf(dec2_rad) * sqr_ra;
	return (2.0 * fabsf(asinf(sqrtf(aux)) * rad2deg));
}


float SmallAngle(float a, float b)
{
	float dif = fmod(fabsf(a - b), 2.0*pi);
	if (dif > pi) {
		dif = 2.0*pi - dif;
	}
	return dif;
}


void AltAzDistToXYZ(float alt, float az, float dist, float* X, float* Y, float* Z)
{
	*X = dist * cosf(alt) * cosf(az);
	*Y = dist * cosf(alt) * sinf(az);
	*Z = dist * sinf(alt);
}


void AltAzRadiansToXYZ(float alt, float az, float* X, float* Y, float* Z)
{
	*X = cosf(alt) * cosf(az);
	*Y = cosf(alt) * sinf(az);
	*Z = sinf(alt);
}


void VectorProduct(float x1, float y1, float z1, float x2, float y2, float z2, float* x3, float* y3, float* z3)
{
	float a = y1 * z2 - z1 * y2;
	float b = z1 * x2 - x1 * z2;
	float c = x1 * y2 - y1 * x2;
	float cp = 1.0 / sqrtf(a*a+b*b+c*c);
	*x3 = a * cp;
	*y3 = b * cp;
	*z3 = c * cp;
}


CMatrix TwoStarAlignMatrix(
	float real_alt_1,
	float real_az_1,
	float obs_alt_1,
	float obs_az_1,
	float real_alt_2,
	float real_az_2,
	float obs_alt_2,
	float obs_az_2
	)
{
	real_alt_1 *= deg2rad;
	real_az_1  *= deg2rad;
	real_alt_2 *= deg2rad;
	real_az_2  *= deg2rad;
	obs_alt_1  *= deg2rad;
	obs_az_1   *= deg2rad;
	obs_alt_2  *= deg2rad;
	obs_az_2   *= deg2rad;

	float x1, y1, z1;
	float x2, y2, z2;
	float x3, y3, z3;
	AltAzRadiansToXYZ(obs_alt_1, obs_az_1, &x1, &y1, &z1);
	AltAzRadiansToXYZ(obs_alt_2, obs_az_2, &x2, &y2, &z2);
	VectorProduct(x1, y1, z1, x2, y2, z2, &x3, &y3, &z3);

	float X1, Y1, Z1;
	float X2, Y2, Z2;
	float X3, Y3, Z3;
	AltAzRadiansToXYZ(real_alt_1, real_az_1, &X1, &Y1, &Z1);
	AltAzRadiansToXYZ(real_alt_2, real_az_2, &X2, &Y2, &Z2);
	VectorProduct(X1, Y1, Z1, X2, Y2, Z2, &X3, &Y3, &Z3);

	float AVals[] = {
		x1, x2, x3,
		y1, y2, y3,
		z1, z2, z3
	};
	CMatrix A(3, 3, AVals);

	float BVals[] = {
		X1, X2, X3,
		Y1, Y2, Y3,
		Z1, Z2, Z3
	};
	CMatrix B(3, 3, BVals);

	return (A * B.Inverse());
}


CMatrix ThreeStarAlignMatrix(
	float real_alt_1,
	float real_az_1,
	float obs_alt_1,
	float obs_az_1,
	float real_alt_2,
	float real_az_2,
	float obs_alt_2,
	float obs_az_2,
	float real_alt_3,
	float real_az_3,
	float obs_alt_3,
	float obs_az_3
	)
{
	real_alt_1 *= deg2rad;
	real_az_1  *= deg2rad;
	real_alt_2 *= deg2rad;
	real_az_2  *= deg2rad;
	real_alt_3 *= deg2rad;
	real_az_3  *= deg2rad;
	obs_alt_1  *= deg2rad;
	obs_az_1   *= deg2rad;
	obs_alt_2  *= deg2rad;
	obs_az_2   *= deg2rad;
	obs_alt_3  *= deg2rad;
	obs_az_3   *= deg2rad;

	float x1, y1, z1;
	float x2, y2, z2;
	float x3, y3, z3;
	AltAzRadiansToXYZ(obs_alt_1, obs_az_1, &x1, &y1, &z1);
	AltAzRadiansToXYZ(obs_alt_2, obs_az_2, &x2, &y2, &z2);
	AltAzRadiansToXYZ(obs_alt_3, obs_az_3, &x3, &y3, &z3);

	float X1, Y1, Z1;
	float X2, Y2, Z2;
	float X3, Y3, Z3;
	AltAzRadiansToXYZ(real_alt_1, real_az_1, &X1, &Y1, &Z1);
	AltAzRadiansToXYZ(real_alt_2, real_az_2, &X2, &Y2, &Z2);
	AltAzRadiansToXYZ(real_alt_3, real_az_3, &X3, &Y3, &Z3);

	float AVals[] = {
		x1, x2, x3,
		y1, y2, y3,
		z1, z2, z3
	};
	CMatrix A(3, 3, AVals);

	float BVals[] = {
		X1, X2, X3,
		Y1, Y2, Y3,
		Z1, Z2, Z3
	};
	CMatrix B(3, 3, BVals);

	return (A * B.Inverse());
}


void CompensateAltAz(
	const CMatrix& mat,
	float real_alt,
	float real_az,
	float* corr_alt,
	float* corr_az
	)
{
	float altrad = real_alt * deg2rad;
	float azrad = real_az * deg2rad;
	float x, y, z;
	AltAzRadiansToXYZ(altrad, azrad, &x, &y, &z);

	float AVals[] = {
		x,
		y,
		z
	};
	CMatrix A(1,3,AVals);
	CMatrix B(1,3);
	B = mat * A;

	float X = B.Get(0,0);
	float Y = B.Get(0,1);
	float Z = B.Get(0,2);
	*corr_az = NormalizeRadians(atan2f(Y, X)) * rad2deg;
	*corr_alt = atan2f(Z, sqrtf(X*X+Y*Y)) * rad2deg;
}



