#include <stdlib.h>
#include "fbtimer.h"
#include "flarelist.h"
#include "clockutil.h"
#include "gps.h"
#include "config.h"
#include "gpio.h"
#include "scroller.h"


#include "flarelist.inc"


const CFlare* nextflare = NULL;
CDateTime NextFlareTime;

enum EFlareScrollerState {
    NONE,
    PENDING,
    FLARE_NEAR,
    IN_PROGRESS
};

EFlareScrollerState flare_scroller_alert_state = PENDING;
EFlareScrollerState prev_scroller_alert_state = NONE;



const CFlare* GetNextFlareAfter(long when)
{
    const CFlare* nxtflare = NULL;
    for (int pos = 0; FlareList[pos]; pos++) {
        if (FlareList[pos]->GetTime() > when) {
            if (!nxtflare || FlareList[pos]->GetTime() < nxtflare->GetTime()) {
                nxtflare = FlareList[pos];
            }
        }
    }
    return nxtflare;
}


void
CheckForNextFlare(CLaser *laser)
{
    if (last_gps_sync_time > 0) {
        long now = time();
        if (!nextflare || nextflare->GetTime() + 30 < now) {
            nextflare = GetNextFlareAfter(now);
            if (nextflare) {
                NextFlareTime.Set(nextflare->GetTime());
                laser->PointToNextFlare();
                flare_scroller_alert_state = PENDING;
            }
        }
        if (nextflare) {
            long nexttime = nextflare->GetTime();
            if (now >= nexttime - FLARE_ALERT_SOUND_END) {
                GpioSetLine(KLAXON_PIN, false);
            } else if (now >= nexttime - FLARE_ALERT_SOUND_START) {
                GpioSetLine(KLAXON_PIN, true);
            }

            if (now >= nexttime - FLARE_ALERT_END) {
                GpioSetLine(FLARE_ALERT_PIN, false);
            } else if (now >= nexttime - FLARE_ALERT_START) {
                GpioSetLine(FLARE_ALERT_PIN, true);
            }

            if (now >= nexttime - FLARE_IN_PROGRESS_TIME) {
                flare_scroller_alert_state = IN_PROGRESS;
            } else if (now >= nexttime - FLARE_ALERT_START) {
                flare_scroller_alert_state = FLARE_NEAR;
            }
        } else {
            flare_scroller_alert_state = NONE;
        }
	switch (flare_scroller_alert_state) {
	    case NONE:
		scroller.Target(1, 0, "C.L.I.V.E.");
		break;
	    case PENDING:
		scroller.Target(1, 0, "Next Flare in");
		break;
	    case FLARE_NEAR:
		scroller.Warning(1, 0, "Flare Alert!");
		break;
	    case IN_PROGRESS:
		scroller.Warning(1, 0, "Flare in Progress!");
		break;
	}
    }
}


long
TimeUntilNextFlare()
{
    if (nextflare) {
        return (nextflare->GetTime() - time());
    }
    return 999999999;
}


void
UpdateTimeLeftClockDisplay(bool doprint)
{
    if (nextflare) {
        long timeleft = TimeUntilNextFlare();
        if (timeleft < 0) {
            if (-timeleft % 2) {
                timeleft = 0;
            } else {
                timeleft = -1;
            }
        }
        OutputTime(doprint, timeleft);
    } else {
        OutputTime(doprint, -1);
    }
}


