#include <stdlib.h>
#include "fbtext.h"
#include "page.h"
#include "p_tracker.h"
#include "laser.h"
#include "fbtimer.h"
#include "keysin.h"
#include "flare.h"
#include "flarelist.h"
#include "gps.h"
#include "skyplot.h"
#include "config.h"
#include "scroller.h"

extern CLaser* laser;

void CTrackerPage::Init()
{
    laser->RecalibrateHome();
    laser->PointToNextFlare();
    scroller.Target(1, 0, "Next Flare in");
}


void CTrackerPage::Display()
{
    char datebuf[30];
    long now = time();
    CDateTime dt(now);
    dt.GetString(datebuf, sizeof(datebuf));

    txtClear();
    txtPrint("Iridium Flare Tracker\n\n");

    if (now < 10000000) {
        txtPrintf("Time: %d\n", now);
    } else {
        txtPrintf("Time: %s\n", datebuf);
    }

    if (nextflare) {
        NextFlareTime.GetString(datebuf, sizeof(datebuf));
        txtPrint("Next flare in ");
        UpdateTimeLeftClockDisplay(true);
        txtPrint("\n");
    }

    txtPrintf("\nTime               Mag Alt Az\n");
    int pos = 0;
    while (FlareList[pos] && FlareList[pos]->GetTime() < now) {
        pos++;
    }

    const CFlare* ptr = NULL;
    int limit = 11;
    while (limit-->0 && (ptr = FlareList[pos])) {
        txtPrintf("%s %2d %3d %3d\n", ptr->GetDateString(), ptr->GetMagnitude(), (short)ptr->GetAltitude(), (short)ptr->GetAzimuth());
        pos++;
    }

    if (nextflare) {
        if (TimeUntilNextFlare() <= FLARE_ALERT_START) {
            laser->PointToNextFlare();
        }
    }
}



void CTrackerPage::HandleKeys(short key)
{
    if (TimeUntilNextFlare() > 120) {
        laser->SlewByKeys();
    }

    if (!key) {
        return;
    }

    if (key == KI_BACK) {
        scroller.PageWrite(1, 0, SC_COLOR_DIM_GREEN, "Recalibrating");
        laser->RecalibrateHome();
    }

    if (key == KI_ENTER) {
        scroller.PageWrite(1, 0, SC_COLOR_DIM_GREEN, "Recalibrating");
        laser->RecalibrateHome();
        scroller.Target(1, 0, "Next Flare in");
        laser->PointToNextFlare();
    }

    if (key == KI_PLOT) {
        float alt, az;
        alt = 0.0;
        az = 0.0;
        if (nextflare) {
            alt = nextflare->GetAltitude();
            az = nextflare->GetAzimuth();
        }
        PlotSky(time(), NULL, laser->GetAltitude(), laser->GetAzimuth(), alt, az);
    }

    UpdateDisplay();
}


