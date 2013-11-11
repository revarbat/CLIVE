#include <math.h>
#include "fbtext.h"
#include "page.h"
#include "p_starfinder.h"
#include "laser.h"
#include "fbtimer.h"
#include "clockutil.h"
#include "keysin.h"
#include "star.h"
#include "starlist.h"
#include "flarelist.h"
#include "skyplot.h"
#include "gps.h"
#include "scroller.h"

long  current_star = 0;
bool star_tracking_on = false;
extern CLaser* laser;
long star_rise = 0;
long star_set = 0;
long prev_star_point_time = 0;

#define CIRCUMPOLAR  -1
#define NEVER_RISES  -2



void
UpdateStarInfo()
{
    long dummy;
    long when = time();
    float ra, dec;

    const CStar* star = &StarList[current_star];
    ra = star->right_ascension;
    dec = star->declination;
    if (my_latitude >= 0 && dec > 90 - my_latitude) {
        star_rise = star_set = CIRCUMPOLAR;
    } else if (my_latitude >= 0 && dec < -90 + my_latitude) {
        star_rise = star_set = NEVER_RISES;
    } else if (my_latitude < 0 && dec > 90 - my_latitude) {
        star_rise = star_set = NEVER_RISES;
    } else if (my_latitude < 0 && dec < -90 + my_latitude) {
        star_rise = star_set = CIRCUMPOLAR;
    } else {
        RiseTransitSet(when, my_latitude, my_longitude, ra, dec, &star_rise, &dummy, &star_set);
    }
}


CStarFinderPage::CStarFinderPage(char* title) :
    CPage(title)
{
    current_star = NextNamedVisibleBrightStar(-1, +1);
    UpdateStarInfo();
}


void
CStarFinderPage::Init()
{
    star_tracking_on = false;
}


void
CStarFinderPage::Display()
{
    float ra, dec, alt, az;
    const CStar* star = &StarList[current_star];
    long when = time();
    star->GetAltAz(when, my_longitude, my_latitude, &alt, &az);
    ra = star->right_ascension;
    dec = star->declination;

    char risebuf[32];
    char setbuf[32];

    if (star_rise > 0) {
        CDateTime dt_rise(star_rise);
        dt_rise.GetTimeString(risebuf, sizeof(risebuf));

        CDateTime dt_set(star_set);
        dt_set.GetTimeString(setbuf, sizeof(setbuf));
    }

    txtClear();

    txtPrint("Star Finder\n\n");

    txtPrintf("Star: %s\n", star->star_name);
    txtPrintf("Mag:  %5.2f\n\n", star->visual_mag);

    if (star_rise == CIRCUMPOLAR) {
        txtPrint("Star is Circumpolar at this\n");
        txtPrint("latitude, and never sets.\n\n");
    } else if (star_rise == NEVER_RISES) {
        txtPrint("Star never rises above the\n");
        txtPrint("horizon at this latitude.\n\n");
    } else if (star_rise > 0) {
        txtPrintf("Rise: %s\n", risebuf);
        txtPrintf("Set:  %s\n\n", setbuf);
    }

    txtPrintf("RA: %5.1f       Az: %5.1f\n", ra, az);
    txtPrintf("Dec:%5.1f       Alt:%5.1f\n", dec, alt);
    if (alt < 0.0) {
        txtPrint("BELOW HORIZON\n");
    } else if (alt < 9.0) {
        txtPrint("BELOW LASER RANGE\n");
    } else {
        txtPrint("                 \n");
    }

    txtPrintf("\nTracking is %s \n", star_tracking_on? "ON" : "off");

    if (star_tracking_on) {
        if (abs(when - prev_star_point_time) >= 5) {
            if (alt > 0) {
                laser->PointTo(az, alt);
            } else {
                laser->PointTo(az, 0.0);
            }
            prev_star_point_time = when;
        }
    }
}




void
CStarFinderPage::HandleKeys(short key)
{
    laser->SlewByKeys();

    if (!key) {
        return;
    }

    if (key == KI_UP || key == KI_DOWN) {
        if (star_tracking_on) {
            scroller.PageWrite(1, 0, SC_COLOR_DIM_GREEN, "Manual Aim");
        }
        star_tracking_on = false;
    }
    if (key == KI_LEFT || key == KI_RIGHT) {
        if (star_tracking_on) {
            scroller.PageWrite(1, 0, SC_COLOR_DIM_GREEN, "Manual Aim");
        }
        star_tracking_on = false;
    }

    if (key == KI_PREV) {
        current_star = NextNamedVisibleBrightStar(current_star, -1);
        UpdateStarInfo();
        star_tracking_on = false;
    } else if (key == KI_NEXT) {
        current_star = NextNamedVisibleBrightStar(current_star, +1);
        UpdateStarInfo();
        star_tracking_on = false;
    }

    if (key == KI_PLOT) {
        float alt, az;
        const CStar* star = &StarList[current_star];
        star->GetAltAz(time(), my_longitude, my_latitude, &alt, &az);
        PlotSky(time(), star->constellation, laser->GetAltitude(), laser->GetAzimuth(), alt, az);
        UpdateStarInfo();
    }

    if (key == KI_ENTER) {
        UpdateStarInfo();
        star_tracking_on = !star_tracking_on;
        prev_star_point_time = 0;
        if (star_tracking_on) {
            const CStar* star = &StarList[current_star];
            scroller.Target(1, 0, star->GetName());
        }
    }

    UpdateDisplay();
}




