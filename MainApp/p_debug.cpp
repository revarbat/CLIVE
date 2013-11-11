#include <stdlib.h>
#include "fbtext.h"
#include "datetime.h"
#include "gpio.h"
#include "page.h"
#include "p_debug.h"
#include "laser.h"
#include "fbtimer.h"
#include "clockutil.h"
#include "celestial.h"
#include "flarelist.h"
#include "gps.h"

extern CLaser* laser;

void
CDebugPage::Init()
{
}


void
CDebugPage::Display()
{
    long now = time();

    txtClear();
    txtPrintf("%s\n\n", GetTitle());

    txtPrintf("Alt0:%8.4f  AltV:%8.4f\n", laser->altitude_zero_offset, laser->altitude_vert_offset);
    txtPrintf("Az0 :%8.4f\n\n", laser->azimuth_north_offset);

    txtPrintf("Clock PWM: %d\n\n", (PWM_DATA(0)>>8));

    txtPrint("Last time synch: ");
    if (last_gps_sync_time == 0) {
        txtPrint("Never      \n");
    } else if (now >= last_gps_sync_time + 2) {
        txtPrintf("%ds ago\n", now - last_gps_sync_time);
    } else {
        txtPrintf("Current    \n");
    }
    txtPrintf("UART good: %d  bad: %d\n", good_uart_data, bad_uart_data);
    txtPrintf("%s\n", last_gps_data);

}


void
CDebugPage::HandleKeys(short key)
{
    laser->SlewByKeys();
}


