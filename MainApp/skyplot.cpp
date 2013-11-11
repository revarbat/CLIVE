#include <stdlib.h>
#include <math.h>
#include "graphics.h"
#include "page.h"
#include "flarelist.h"
#include "starlist.h"
#include "planetlist.h"
#include "constlines.h"
#include "celestial.h"
#include "keysin.h"
#include "fbtext.h"
#include "gps.h"


extern CPage* tracker_page;


//////////////////////////////////////////////////////////////////////
// Altitude/Azimuth to XY processing and plotting
//////////////////////////////////////////////////////////////////////

void AltAzToXYRaw(float alt, float az, short& x, short& y)
{
    alt = 79.0 * (90.0 - alt) / 90.0;
    x = -(short)(sin(az*M_PI/180.0) * alt);
    y = (short)(cos(az*M_PI/180.0) * alt);
}


void AltAzToXY(float alt, float az, short& x, short& y)
{
    if (alt >= 0.0) {
        alt = 79.0 * (90.0 - alt) / 90.0;
        x = 119 - (short)(sin(az*M_PI/180.0) * alt);
        y = 79 - (short)(cos(az*M_PI/180.0) * alt);
    } else {
        x = -1;
        y = -1;
    }
}


void PlotAltAzPixel(float alt, float az, color_t color)
{
    if (alt >= 0.0) {
        short x, y;
        AltAzToXY(alt, az, x, y);
        GrPlotPixel(x, y, color);
    }
}


void PlotAltAzLine(float alt1, float az1, float alt2, float az2, color_t color)
{
    float dalt, daz;

    if (alt1 < 0.0 && alt2 < 0.0) {
        return;
    }
    if (alt1 < 0.0) {
        dalt = alt1 - alt2;
        daz  = az1 - az2;
        az1 = (daz * alt2 / fabs(dalt)) + az1;
        alt1 = 0.0;
    }
    if (alt2 < 0.0) {
        dalt = alt1 - alt2;
        daz  = az1 - az2;
        az2 = (daz * alt1 / fabs(dalt)) + az2;
        alt2 = 0.0;
    }

    short x1, y1;
    AltAzToXY(alt1, az1, x1, y1);

    short x2, y2;
    AltAzToXY(alt2, az2, x2, y2);

    GrPlotLine(x1, y1, x2, y2, color);
}


void PlotAltAzSun(float alt, float az, color_t color)
{
    if (alt >= 0.0) {
        short x, y;
        AltAzToXY(alt, az, x, y);
	if (y >=  2 && y <= 155)
	    GrPlotLine(x-1, y-2, x+1, y-2, color);
	if (y >=  1 && y <= 156)
	    GrPlotLine(x-2, y-1, x+2, y-1, color);
	if (y >=  0 && y <= 157)
	    GrPlotLine(x-2, y  , x+2, y  , color);
	if (y >= -1 && y <= 158)
	    GrPlotLine(x-2, y+1, x+2, y+1, color);
	if (y >= -2 && y <= 159)
	    GrPlotLine(x-1, y+2, x+1, y+2, color);
    }
}


void PlotAltAzCrossHair(float alt, float az, color_t color)
{
    if (alt >= 0.0) {
        short x, y;
        AltAzToXY(alt, az, x, y);
        GrPlotPixel(x+2, y  , color);
        GrPlotPixel(x+1, y  , color);
        GrPlotPixel(x-1, y  , color);
        GrPlotPixel(x-2, y  , color);
        GrPlotPixel(x  , y-2, color);
        GrPlotPixel(x  , y-1, color);
        GrPlotPixel(x  , y+1, color);
        GrPlotPixel(x  , y+2, color);
    }
}


void PlotAltAzGrid(color_t color)
{
    short x, y;
    for (int a = 0; a < 90; a += 1) {
        AltAzToXYRaw(0, a, x, y);
        GrPlotPixel(119+x, 79+y, color);
        GrPlotPixel(119+x, 79-y, color);
        GrPlotPixel(119-x, 79+y, color);
        GrPlotPixel(119-x, 79-y, color);
        GrPlotPixel(119+y, 79+x, color);
        GrPlotPixel(119+y, 79-x, color);
        GrPlotPixel(119-y, 79+x, color);
        GrPlotPixel(119-y, 79-x, color);
    }
    for (int a = 0; a < 90; a += 3) {
        AltAzToXYRaw(45, a, x, y);
        GrPlotPixel(119+x, 79+y, color);
        GrPlotPixel(119+x, 79-y, color);
        GrPlotPixel(119-x, 79+y, color);
        GrPlotPixel(119-x, 79-y, color);
        GrPlotPixel(119+y, 79+x, color);
        GrPlotPixel(119+y, 79-x, color);
        GrPlotPixel(119-y, 79+x, color);
        GrPlotPixel(119-y, 79-x, color);
    }
    PlotAltAzLine(0.0,   0.0, 0.0, 180.0, color);
    PlotAltAzLine(0.0,  90.0, 0.0, 270.0, color);
}




//////////////////////////////////////////////////////////////////////
// Sky object plot position caching
//////////////////////////////////////////////////////////////////////


struct XY_t {
    short x, y;
    long when;
};


class CXYCache {
public:
    CXYCache(long count) {
        item_count = count;
        curr_item = -1;
        xycache = new XY_t[count];
    }

    ~CXYCache() {
        delete [] xycache;
    }

    bool IsCalculated(long when, long idx) {
        if (abs(when - xycache[idx].when) > 600) {
            return false;
        }
        return (xycache[idx].x != 0 || xycache[idx].y != 0);
    }

    bool IsCalculatedRecently(long when, long idx) {
        if (abs(when - xycache[idx].when) > 300) {
            return false;
        }
        return (xycache[idx].x != 0 || xycache[idx].y != 0);
    }

    bool PlotItem(long when, long idx, color_t color) {
        if (!IsCalculated(when, idx)) {
            return false;
        }
        GrPlotPixel(xycache[idx].x, xycache[idx].y, color);
        return true;
    }

    long GetNextItem() {
        if (++curr_item >= item_count) {
            curr_item = 0;
        }
        return curr_item;
    }

    void GetXY(long idx, short& x, short& y) {
        x = xycache[idx].x;
        y = xycache[idx].y;
    }

    void SetItem(long when, long idx, short x, short y) {
        xycache[idx].x = x;
        xycache[idx].y = y;
        xycache[idx].when = when;
    }

private:
    XY_t* xycache;
    long item_count;
    long curr_item;
};

CXYCache* starcache;
CXYCache* planetcache;
CXYCache* const1cache;
CXYCache* const2cache;


void InitSkyCaches()
{
    starcache = new CXYCache(GetStarCount());
    planetcache = new CXYCache(GetPlanetCount());
    const1cache = new CXYCache(GetConstellationLineCount());
    const2cache = new CXYCache(GetConstellationLineCount());
}


void UpdateStarCacheItem(long when, long idx)
{
    float alt, az;
    short x, y;

    const CStar* star = &StarList[idx];
    star->GetAltAz(when, my_longitude, my_latitude, &alt, &az);
    AltAzToXY(alt, az, x, y);
    starcache->SetItem(when,idx,x,y);
}


void UpdateConstellationCacheItem(long when, long idx)
{
    float alt1, az1;
    float alt2, az2;
    short x1, y1;
    short x2, y2;

    const struct ConstellationLine* cline = &ConstellationLineList[idx];

    RADecToAltAz(when, my_longitude, my_latitude, cline->ra1, cline->dec1, &alt1, &az1);

    RADecToAltAz(when, my_longitude, my_latitude, cline->ra2, cline->dec2, &alt2, &az2);

    if (alt1 > 0.0 && alt2 > 0.0) {
        if (alt1 < 0.0) {
            float dalt = alt1 - alt2;
            float daz  = az1 - az2;
            az1 = (daz * alt2 / fabs(dalt)) + az1;
            alt1 = 0.0;
        }
        if (alt2 < 0.0) {
            float dalt = alt1 - alt2;
            float daz  = az1 - az2;
            az2 = (daz * alt1 / fabs(dalt)) + az2;
            alt2 = 0.0;
        }

        AltAzToXY(alt1, az1, x1, y1);
        AltAzToXY(alt2, az2, x2, y2);
    } else {
        x1 = y1 = x2 = y2 = -1;
    }

    const1cache->SetItem(when,idx,x1,y1);
    const2cache->SetItem(when,idx,x2,y2);
}


void UpdateSkyCaches()
{
    if (last_gps_sync_time == 0) {
	// We don't know where we are yet.
	return;
    }
    long idx;
    long now = time();
    do {
	idx = starcache->GetNextItem();
    } while (idx > 0 && starcache->IsCalculatedRecently(now,idx));
    UpdateStarCacheItem(now, idx);

    do {
	idx = const1cache->GetNextItem();
	idx = const2cache->GetNextItem();
    } while (idx > 0 && const1cache->IsCalculatedRecently(now,idx));
    UpdateConstellationCacheItem(now, idx);
}



///////////////////////////////////////////////////////////////////////
// Plot sky map
///////////////////////////////////////////////////////////////////////


void PlotConstellation(long when, const char* abbrev, color_t color)
{
    int constcnt = GetConstellationLineCount();
    for (int i = 0; i < constcnt; i++) {
        const struct ConstellationLine* cline = &ConstellationLineList[i];
        if (!strcmp(cline->name, abbrev)) {
            short x1, y1, x2, y2;
            if (!const1cache->IsCalculated(when, i)) {
                UpdateConstellationCacheItem(when, i);
            }
            const1cache->GetXY(i, x1, y1);
            const2cache->GetXY(i, x2, y2);
            if (x1 > 0 && y1 > 0 && x2 > 0 && y2 > 0) {
                GrPlotLine(x1, y1, x2, y2, color);
            }
        }
        keyin->Poll();
        if ((i & 0x3) == 0) {
            if (nextflare && TimeUntilNextFlare() < 120) {
                break;
            }
            if (GetKeyCount() > 0) {
                break;
            }
        }
    }
    if (GetKeyCount() > 0) {
        GetKey();
    }
}


void PlotConstellations(long when, const char* abbrev, color_t color, color_t hicolor)
{
    int constcnt = GetConstellationLineCount();
    for (int i = 0; i < constcnt; i++) {
        short x1, y1, x2, y2;
        if (!const1cache->IsCalculated(when, i)) {
            UpdateConstellationCacheItem(when, i);
        }
        const1cache->GetXY(i, x1, y1);
        const2cache->GetXY(i, x2, y2);
        if (x1 > 0 && y1 > 0 && x2 > 0 && y2 > 0) {
            const struct ConstellationLine* cline = &ConstellationLineList[i];
            if (abbrev && !strcmp(cline->name, abbrev)) {
                GrPlotLine(x1, y1, x2, y2, hicolor);
            } else {
                GrPlotLine(x1, y1, x2, y2, color);
            }
        }
        keyin->Poll();
        if ((i & 0x3) == 0) {
            if (nextflare && TimeUntilNextFlare() < 120) {
                break;
            }
            if (GetKeyCount() > 0) {
                break;
            }
        }
    }
    if (GetKeyCount() > 0) {
        GetKey();
    }
}


void PlotStars(long when)
{
    int starcnt = GetStarCount();
    for (int i = 0; i < starcnt; i++) {
        const CStar* star = &StarList[i];
        short bright = 31 - ((star->visual_mag < 0.0)? 0 : (short)(3 * star->visual_mag));

        if (!starcache->IsCalculated(when, i)) {
            UpdateStarCacheItem(when, i);
        }
        starcache->PlotItem(when, i, RGB(bright,bright,bright));
        keyin->Poll();
        if ((i & 0x7) == 0) {
            if (nextflare && TimeUntilNextFlare() < 120) {
                break;
            }
            if (GetKeyCount() > 0) {
                break;
            }
        }
    }
    if (GetKeyCount() > 0) {
        GetKey();
    }
}


void PlotPlanets(long when, color_t color)
{
    float alt, az;
    CPlanet* earth = FindPlanet("Earth");
    int planetcnt = GetPlanetCount();
    for (int i = 0; i < planetcnt; i++) {
        CPlanet* planet = PlanetList[i];
        planet->GetAltAz(when, *earth->GetOrbit(), my_longitude, my_latitude, &alt, &az);
        PlotAltAzPixel(alt, az, color);
        keyin->Poll();
        if (GetKeyCount() > 0) {
            break;
        }
    }
}


void PlotSun(long when, color_t color)
{
    float alt, az, ra, dec;
    CPlanet* earth = FindPlanet("Earth");
    SolarRADec(*earth->GetOrbit(), when, &ra, &dec);
    RADecToAltAz(when, my_longitude, my_latitude, ra, dec, &alt, &az);
    PlotAltAzSun(alt, az, color);
}


void PlotSky(long when, const char* constel, float laser_alt, float laser_az, float poi_alt, float poi_az)
{
    GrInit();
    GrClearScreen();

    PlotAltAzGrid(color_t RGB(0,0,31));

    if (poi_alt > 0.0) {
        PlotAltAzCrossHair(poi_alt, poi_az, RGB(31,15,0));
    }
    if (laser_alt > 0.0) {
        PlotAltAzLine(90.0, 0.0, laser_alt, laser_az, RGB(7,31,7));
    }

    PlotConstellation(when, constel, RGB(15,15,15));
    PlotConstellations(when, constel, RGB(0,0,23), RGB(15,15,15));

    if (poi_alt > 0.0) {
        PlotAltAzCrossHair(poi_alt, poi_az, RGB(31,15,0));
    }
    if (laser_alt > 0.0) {
        PlotAltAzLine(90.0, 0.0, laser_alt, laser_az, RGB(7,31,7));
    }

    PlotStars(when);
    PlotPlanets(when, RGB(31,0,0));
    PlotSun(when, RGB(31,31,0));

    do {
        if (nextflare && TimeUntilNextFlare() < 120) {
            break;
        }
        keyin->Poll();
    } while (GetKeyCount() <= 0);
    GetKey();

    GrRelease();

    if (nextflare && TimeUntilNextFlare() < 120) {
        current_page = tracker_page;
        txtPrint("Flare in less than 2 minutes!\n\n");
        txtPrint("Switching to Flare Tracker!\n\n");
        sleep(3);
    }
}


