#include <stdlib.h>
#include "fbtext.h"
#include "datetime.h"
#include "gpio.h"
#include "page.h"
#include "p_info.h"
#include "laser.h"
#include "fbtimer.h"
#include "clockutil.h"
#include "celestial.h"
#include "flarelist.h"
#include "gps.h"

extern CLaser* laser;

void
CInfoPage::Init()
{
    long now = time();
    UpdateSunRiseSet(now, my_latitude, my_longitude);
}


void
CInfoPage::Display()
{
    long now = time();
    long modfactor = now % 86400;
    if (modfactor == 0) {
	UpdateSunRiseSet(now, my_latitude, my_longitude);
    }
    CDateTime dt_rise(sunrise_time);
    CDateTime dt_set(sunset_time);
    char risebuf[32];
    char setbuf[32];
    dt_rise.GetString(risebuf, sizeof(risebuf));
    dt_set.GetString(setbuf, sizeof(setbuf));

    txtClear();
    txtPrintf("%s\n\n", GetTitle());

    txtPrintf("Lat :%8.4f  Lon :%8.4f\n\n", my_latitude, my_longitude);

    txtPrintf("Sunrise: %s\n", risebuf);
    txtPrintf("Sunset:  %s\n\n", setbuf);
}


void
CInfoPage::HandleKeys(short key)
{
    laser->SlewByKeys();
}


