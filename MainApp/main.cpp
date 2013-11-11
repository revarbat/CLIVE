#include <sys/types.h>
#include <math.h>
#include "fbtext.h"
#include "fastmath.h"
#include "gps.h"
#include "page.h"
#include "laser.h"
#include "fbtimer.h"
#include "clockutil.h"
#include "celestial.h"
#include "gpio.h"
#include "keysin.h"
#include "uart.h"
#include "scroller.h"
#include "string.h"
#include "flare.h"
#include "flarelist.h"
#include "skyplot.h"
#include "p_config_int.h"
#include "p_config_long.h"
#include "p_config_bool.h"
#include "p_tracker.h"
#include "p_starfinder.h"
#include "p_planetfinder.h"
#include "p_info.h"
#include "p_debug.h"
#include "p_calibrate_coarse.h"
#include "p_calibrate_3star.h"

FBTimer fbt;
CLaser* laser;
CPage* tracker_page;
CKeysIn* ki;
bool stepper_active_holding = false;
bool debug_flare_imminent = false;

#ifdef NO_XPORT
unsigned short pwm_values[PWM_NUM/2];
unsigned short gpio_directions[(GPIO_NUM+15)/16];
unsigned short gpio_values[(GPIO_NUM+15)/16];
#endif


class DisplayUpdateTimerEvent : public FBTimerEvent {
public:
    DisplayUpdateTimerEvent() : FBTimerEvent(0, 0, 0, 512) {}
    virtual void Run(FBTimer* timer) {
        long secs = timer->GetTimeSeconds();
        long difftime = secs - event_time_secs;
        if (difftime > 1 || difftime < -1) {
            SetNextScheduledTime(secs + 1, 0);
        }
        UpdateTimeLeftClockDisplay(false);
    }
} display_updater;


class TurretUpdateTimerEvent : public FBTimerEvent {
public:
    TurretUpdateTimerEvent() : FBTimerEvent(0, 0, 0, 64) {}

    virtual void Run(FBTimer* timer) {
        long secs = timer->GetTimeSeconds();
        long difftime = secs - event_time_secs;
        if (difftime > 1 || difftime < -1) {
            SetNextScheduledTime(secs + 1, 0);
        }
        TurretPoll();
    }

} turret_updater;


extern "C"
int main(void)
{
    txtClear();

    // Create menus
    txtPrint("Creating menus.\n");

    main_menu_page = new CMenuPage("Main Menu");
    tracker_page = new CTrackerPage("Flare Tracker");
    CMenuPage* calib_menu_page = new CMenuPage("Calibration");
    CMenuPage* config_menu_page = new CMenuPage("Settings");
    CPage* calib_3star_page = new CCalibrate3StarPage("3-Star Calibration");

    main_menu_page->AddItem(tracker_page);
    main_menu_page->AddItem(new CStarFinderPage("Star Finder"));
    main_menu_page->AddItem(new CPlanetFinderPage("Planet Finder"));
    main_menu_page->AddItem(calib_menu_page);
    main_menu_page->AddItem(new CInfoPage("Info Page"));
    main_menu_page->AddItem(config_menu_page);
    main_menu_page->AddItem(new CDebugPage("Debug Page"));

    calib_menu_page->AddItem(new CCalibrateCoarsePage("Coarse Calibration"));
    calib_menu_page->AddItem(calib_3star_page);

    config_menu_page->AddItem(new CConfigIntPage("Day Clock Brightness", &daytime_clock_pwm, 0, 255));
    config_menu_page->AddItem(new CConfigIntPage("Night Clock Brightness", &nighttime_clock_pwm, 0, 255));
    config_menu_page->AddItem(new CConfigIntPage("Slow Laser Table Speed", &turret_speed_low, 1, 100));
    config_menu_page->AddItem(new CConfigIntPage("Fast Laser Table Speed", &turret_speed_high, 1, 100));
    config_menu_page->AddItem(new CConfigBoolPage("Active Stepper Holding", &stepper_active_holding));
    config_menu_page->AddItem(new CConfigBoolPage("Make Imminent Flare", &debug_flare_imminent));
    config_menu_page->AddItem(new CConfigLongPage("Laser Azimuth Backlash", &turret_azimuth_backlash, 1, 9999));
    config_menu_page->AddItem(new CConfigLongPage("Laser Altitude Backlash", &turret_altitude_backlash, 1, 9999));

    current_page = tracker_page;


    // set first 31 GPIO bits to output
    GPIO_DDR(0) = 0xffff; 
    GPIO_DDR(1) = 0x0fff; 
    GPIO_DDR(2) = 0x0f00; 
    GPIO_DATA(0) = 0x0000;
    GPIO_DATA(1) = 0x0000;
    GPIO_DATA(2) = 0x0000;
    GpioSetDirOutbound(FLARE_ALERT_PIN, true);
    GpioSetDirOutbound(KLAXON_PIN, true);

    // Clock brightness.
    PWM_DATA(0) = 0x0f00;

    fm_benchmark();

    txtPrint("Init GPS Serial link.\n");
    gps_uart.SetBaudRate(4800);
#ifdef NO_XPORT
    gps_uart.ReadBufStuff("$GPRMC,071458,A,3714.1938,N,12153.8619,W,0.0,241.0,070804,14.9,E,A*36\r\n");
#endif

    txtPrint("Init Turret Serial link.\n");
    turret_uart.SetBaudRate(9600);
    millisleep(128);
    turret_uart.WriteString("4!");
    millisleep(512);
    turret_uart.WriteString("0H");
    millisleep(128);
    turret_uart.WriteString("3O");
    millisleep(128);
    turret_uart.WriteString("B32000P");

    txtPrint("Init Scroller Serial link.\n");
    scroller_uart.SetBaudRate(9600);
    millisleep(128);
    scroller.PageWrite(1, 0, SC_COLOR_LIME, "C.L.I.V.E. v2.0<FI>Celestial<FI>Laser<FI>Identification /<FI>Visualisation<FI>Experiment<FP>");
    sleep(1);
    scroller.Init(1);
    sleep(3);

    txtPrint("Init keypads.\n");
    ki = new CKeysIn();

    txtPrint("Init sky object caches.\n");
    InitSkyCaches();

    txtPrint("Test clock digits.\n");
    scroller.PageWrite(1, 0, SC_COLOR_BRIGHT_GREEN, "Initialize Clock");
    sleep(1);
    TestClockDisplay();
    fbt.ScheduleEvent(&display_updater);

    txtPrint("Init laser table.\n");
    scroller.PageWrite(1, 0, SC_COLOR_BRIGHT_GREEN, "Initialize Laser");
    fbt.ScheduleEvent(&turret_updater);
    sleep(2);
    laser = new CLaser();
    laser->SetBeamState(BEAM_ON);

    scroller.PageWrite(1, 0, SC_COLOR_BRIGHT_GREEN, "Initialize GPS");
    sleep(3);
    txtClear();

    long seconds = 0;
    long lastseconds = 0;
    while (1) {
        GetGpsInfoFromSerial();
        UpdateSkyCaches();
        laser->SetActiveHolding(stepper_active_holding);
        if (debug_flare_imminent) {
            debug_flare_imminent = false;
            delete FlareList[0];
            FlareList[0] = new CFlare(seconds + 135, 0, 45.0, 45.0, 0);
            nextflare = NULL;
        }
        CheckForNextFlare(laser);
        ki->Poll();
        HandleKeys();
        seconds = time();
        if (seconds != lastseconds) {
            if ((seconds & 0x0fff) == 0) { // Every 1h 8m 16s
                UpdateSunRiseSet(seconds, my_latitude, my_longitude);
            }
            UpdateClockBrightness();
            UpdateDisplay();
            lastseconds = seconds;
        }
    }
}


