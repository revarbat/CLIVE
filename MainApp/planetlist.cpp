#include <stdlib.h>
#include "starlist.h"
#include "planetlist.h"

OrbitData*
NewPlanetOrbit(
	long epoch,
        float semimaj,
	float eccent,
	float incl,
	float longasc,
	float longperi,
	float meanlong,
        float cr_semimaj,
	float cr_eccent,
	float cr_incl,
	float cr_longasc,
	float cr_longperi,
	float cr_meanlong,
	float tilt
)
{
    OrbitData* orbit = new OrbitData(epoch);
    orbit->SetSemiMajorAxis(semimaj, cr_semimaj);
    orbit->SetEccentricity(eccent, cr_eccent);
    orbit->SetOrbitalInclination(incl, cr_incl);
    orbit->SetLongitudeOfAscendingNode(longasc, cr_longasc);
    orbit->SetLongitudeOfPeriapsis(longperi, cr_longperi);
    orbit->SetMeanLongitude(meanlong, cr_meanlong);
    orbit->SetRotationalTilt(tilt);
    return orbit;
}


#define JDT(x) (long)((x-2440587.5)*86400)
#define JD22SEP2006 JDT(2454000.5)
#define JD18AUG2005 JDT(2453600.5)
#define JD22OCT2004 JDT(2453600.5)

#if defined(TESTBENCH) || defined(NO_GBA)
# define PLANETSECT
#else
# define PLANETSECT __attribute__ ((section(".iwram")))
#endif

CPlanet* PlanetList[] PLANETSECT = {
    //          Name                      Epoch         Semimaj      Eccent.    Incline   AscNode     LongPeri  MeanLong    SemiMajCR     EccentCR    InclCR   AscNodeCR  LPeriCR  MeanLongCR     Tilt
    //                                    secs          a.u.         no units   degrees   degrees     degrees   degrees     deg/cent      1/cent     "/cent    "/cent     "/cent   "/cent         degs
    new CPlanet("Mercury", NewPlanetOrbit(J2000,         0.38709893, .20563069,  7.00487,  48.33167,  77.45645, 252.25084,  0.00000066,  0.00002527, -23.51,   -446.30,   573.57, 538101628.29,   0.01)),
    new CPlanet("Venus",   NewPlanetOrbit(J2000,         0.72333199, .00677323,  3.39471,  76.68069, 131.53298, 181.97973,  0.00000092, -0.00004938,  -2.68,   -996.89,  -108.80, 210664136.06, 177.36)),
    new CPlanet("Earth",   NewPlanetOrbit(J2000,         1.00000011, .01671022,  0.00005, 348.73936, 102.94719, 100.46435, -0.00000005, -0.00003804, -46.94, -18228.25,  1198.28, 129597740.63,  23.45)),
    new CPlanet("Mars",    NewPlanetOrbit(J2000,         1.52366231, .09341233,  1.85061,  49.57854, 336.04084, 355.45332, -0.00007221,  0.00011902, -25.47,  -1020.19,  1560.78,  68905103.78,  25.19)),
    new CPlanet("Jupiter", NewPlanetOrbit(J2000,         5.20336301, .04839266,  1.30530, 100.55615,  14.75385,  34.40438,  0.00060737, -0.00012880,  -4.15,   1217.17,   839.93,  10925078.35,   3.13)),
    new CPlanet("Saturn",  NewPlanetOrbit(J2000,         9.53707032, .05415060,  2.48446, 113.71504,  92.43194,  49.94432, -0.00301530, -0.00036762,   6.11,  -1591.05, -1948.89,   4401052.95,  26.73)),
    new CPlanet("Uranus",  NewPlanetOrbit(J2000,        19.19126393, .04716771,  0.76986,  74.22988, 170.96424, 313.23218,  0.00152025, -0.00019150,  -2.09,  -1681.40,  1312.56,   1542547.79,  97.77)),
    new CPlanet("Neptune", NewPlanetOrbit(J2000,        30.06896348, .00858587,  1.76917, 131.72169,  44.97135, 304.88003, -0.00125196,  0.00002510,  -3.64,   -151.25,  -844.43,    786449.21,  28.32)),
    new CPlanet("Pluto",   NewPlanetOrbit(J2000,        39.48168677, .24880766, 17.14175, 110.30347, 224.06676, 238.92881, -0.00076912,  0.00006465,  11.07,    -37.33,  -132.25,    522747.90, 122.53)),
    new CPlanet("Quaoar",  NewPlanetOrbit(JD22OCT2004,  43.373492,   .037457,    7.992,   188.791,   343.641,   273.737,    0.0,         0.0,          0.00,      0.0,      0.00,         0.00,   0.00)),
    new CPlanet("Xena",    NewPlanetOrbit(JD18AUG2005,  67.7091000,  .4416129,  44.17745,  35.87473, 187.21092,  24.71148,  0.0,         0.0,          0.00,      0.0,      0.00,         0.00,   0.00)),
    new CPlanet("Sedna",   NewPlanetOrbit(JD22SEP2006, 485.7446251,  .8432988,  11.92952, 144.42364,  95.94103,  93.59068,  0.0,         0.0,          0.00,      0.0,      0.00,         0.00,   0.00)),
    NULL
};


CPlanet* FindPlanet(const char* name)
{
    const long max_planet_slots = sizeof(PlanetList) / sizeof(CPlanet*);
    for (long pos = 0; pos < max_planet_slots; pos++) {
        if (!PlanetList[pos]) {
            break;
        }
        if (!strcmp(PlanetList[pos]->GetName(), name)) {
            return PlanetList[pos];
        }
    }
    return NULL;
}


int cached_planet_count = -1;
int GetPlanetCount()
{
    if (cached_planet_count < 0) {
        const long max_planet_slots = sizeof(PlanetList) / sizeof(CPlanet*);
        int count = 0;
        for (long pos = 0; pos < max_planet_slots; pos++) {
            if (!PlanetList[pos]) {
                break;
            }
            count++;
        }
        cached_planet_count = count;
    }
    return cached_planet_count;
}


