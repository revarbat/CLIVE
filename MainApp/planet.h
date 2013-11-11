#ifndef PLANET_H
#define PLANET_H

#include <stdlib.h>
#include <string.h>
#include "datetime.h"
#include "celestial.h"
#include "orbitdata.h"

class CPlanet {
public:
    CPlanet(const char* name, const OrbitData* orbit) {
        planet_name = (char*)malloc(strlen(name)+1);
        strcpy(planet_name, name);
        planet_orbit = orbit;
    }

    ~CPlanet() {
        free(planet_name);
        delete planet_orbit;
    }

    const char* GetName() const { return planet_name; }
    const OrbitData* GetOrbit() const { return planet_orbit; }

    void GetRaDecl(long when, const OrbitData& obs_orbit, float* ra, float* decl) const {
        SolarOrbitToEquatorial(*planet_orbit, obs_orbit, when, ra, decl);
    }

    void GetAltAz(
        long when,
        const OrbitData& obs_orbit,
        float longitude,
        float latitude,
        float* alt,
        float* az
        ) const
    {
        float ra, dec;
        GetRaDecl(when, obs_orbit, &ra, &dec);
        RADecToAltAz(when, longitude, latitude, ra, dec, alt, az);
    }

private:
    char* planet_name;
    const OrbitData* planet_orbit;

};

#endif
