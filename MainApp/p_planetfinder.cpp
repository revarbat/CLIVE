#include <math.h>
#include "fbtext.h"
#include "page.h"
#include "p_planetfinder.h"
#include "laser.h"
#include "fbtimer.h"
#include "clockutil.h"
#include "keysin.h"
#include "planet.h"
#include "planetlist.h"
#include "flarelist.h"
#include "skyplot.h"
#include "gps.h"
#include "scroller.h"

extern CLaser* laser;

int  current_planet = 0;
bool planet_tracking_on = false;
long prev_planet_point_time = 0;

float planet_ra = 0;
float planet_dec = 0;
long planet_rise = 0;
long planet_set = 0;



static void
UpdatePlanetInfo()
{
    long dummy;
    long when = time();

    CPlanet* earth = FindPlanet("Earth");
    CPlanet* planet = PlanetList[current_planet];
    planet->GetRaDecl(when, *earth->GetOrbit(), &planet_ra, &planet_dec);
    RiseTransitSet(when, my_latitude, my_longitude, planet_ra, planet_dec, &planet_rise, &dummy, &planet_set);
}


void
CPlanetFinderPage::Init()
{
    planet_tracking_on = false;
    UpdatePlanetInfo();
}


void
CPlanetFinderPage::Display()
{
    float alt, az;
    CPlanet* planet = PlanetList[current_planet];
    long when = time();
    RADecToAltAz(when, my_longitude, my_latitude, planet_ra, planet_dec, &alt, &az);

    char risebuf[32];
    CDateTime dt_rise(planet_rise);
    dt_rise.GetTimeString(risebuf, sizeof(risebuf));

    char setbuf[32];
    CDateTime dt_set(planet_set);
    dt_set.GetTimeString(setbuf, sizeof(setbuf));

    txtClear();

    txtPrint("Planet Finder\n\n");

    txtPrintf("Planet: %s\n\n", planet->GetName());

    txtPrintf("Rise: %s\n", risebuf);
    txtPrintf("Set:  %s\n\n", setbuf);

    txtPrintf("RA: %5.1f       Az: %5.1f\n", planet_ra, az);
    txtPrintf("Dec:%5.1f       Alt:%5.1f\n", planet_dec, alt);
    if (alt < 0.0) {
        txtPrint("BELOW HORIZON.\n");
    } else if (alt < 9.0) {
        txtPrint("BELOW LASER RANGE.\n");
    } else {
        txtPrint("                      \n");
    }

    txtPrintf("\nTracking is %s \n", planet_tracking_on? "ON" : "off");

    if (planet_tracking_on) {
        if (abs(when - prev_planet_point_time) >= 30) {
            if (alt > 0) {
                laser->PointTo(az, alt);
            } else {
                laser->PointTo(az, 0.0);
            }
            prev_planet_point_time = when;
        }
    }
}



void
CPlanetFinderPage::HandleKeys(short key)
{
    laser->SlewByKeys();

    if (!key) {
        return;
    }

    int planetcount = GetPlanetCount();

    if (key == KI_UP || key == KI_DOWN) {
        if (planet_tracking_on) {
            scroller.PageWrite(1, 0, SC_COLOR_DIM_GREEN, "Manual Aim");
        }
        planet_tracking_on = false;
    }
    if (key == KI_LEFT || key == KI_RIGHT) {
        if (planet_tracking_on) {
            scroller.PageWrite(1, 0, SC_COLOR_DIM_GREEN, "Manual Aim");
        }
        planet_tracking_on = false;
    }

    if (key == KI_PREV) {
        CPlanet* planet;
        CPlanet* obs_planet = FindPlanet("Earth");
        do {
            if (current_planet-- == 0) {
                current_planet = planetcount - 1;
            }
            planet = PlanetList[current_planet];
        } while (obs_planet == planet);
        planet_tracking_on = false;
        UpdatePlanetInfo();
    } else if (key == KI_NEXT) {
        CPlanet* planet;
        CPlanet* obs_planet = FindPlanet("Earth");
        do {
            if (current_planet++ == planetcount-1) {
                current_planet = 0;
            }
            planet = PlanetList[current_planet];
        } while (obs_planet == planet);
        planet_tracking_on = false;
        UpdatePlanetInfo();
    }

    if (key == KI_PLOT) {
        float alt, az;
        CPlanet* obs_planet = FindPlanet("Earth");
        CPlanet* planet = PlanetList[current_planet];
        planet->GetAltAz(time(), *obs_planet->GetOrbit(), my_longitude, my_latitude, &alt, &az);
        PlotSky(time(), NULL, laser->GetAltitude(), laser->GetAzimuth(), alt, az);
        UpdatePlanetInfo();
    }

    if (key == KI_ENTER) {
        UpdatePlanetInfo();
        planet_tracking_on = !planet_tracking_on;
        prev_planet_point_time = 0;
        if (planet_tracking_on) {
            CPlanet* planet = PlanetList[current_planet];
            scroller.Target(1, 0, SC_ICON_PLANET, planet->GetName());
        }
    }

    UpdateDisplay();
}



