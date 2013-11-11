#ifndef PLANETLIST_H
#define PLANETLIST_H

#include "star.h"
#include "planet.h"

extern CPlanet* PlanetList[];

int GetPlanetCount();
CPlanet* FindPlanet(const char* name);

#endif

