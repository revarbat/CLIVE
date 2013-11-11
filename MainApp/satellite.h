#ifndef PLANET_H
#define PLANET_H

#include <stdlib.h>
#include <string.h>
#include "datetime.h"
#include "celestial.h"
#include "orbitdata.h"

class CSatellite {
public:
    CSatellite(const char* name, const OrbitData* orbit) {
        satellite_name = (char*)malloc(strlen(name)+1);
        strcpy(satellite_name, name);
        satellite_orbit = orbit;
    }

    ~CSatellite() {
        free(satellite_name);
        delete satellite_orbit;
    }

    const char* GetName() const { return satellite_name; }
    const OrbitData* GetOrbit() const { return satellite_orbit; }

    void GetRaDecl(long when, float* ra, float* decl) const {
        SolarOrbitToEquatorial(*satellite_orbit, obs_orbit, when, ra, decl);
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
    char* satellite_name;
    const OrbitData* satellite_orbit;

};

#endif
