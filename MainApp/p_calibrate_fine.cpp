#include <stdlib.h>
#include <math.h>
#include "fbtext.h"
#include "page.h"
#include "p_calibrate_fine.h"
#include "laser.h"
#include "fbtimer.h"
#include "gpio.h"
#include "keysin.h"
#include "star.h"
#include "starlist.h"
#include "skyplot.h"
#include "flarelist.h"
#include "gps.h"

bool calibrate_second_star = false;
extern CLaser* laser;
const CStar** calib_stars = NULL;
long calib_star_count;
long curr_calib_star = 0;
const CStar* polaris;
float calibrated_polaris_alt;
float expected_polaris_alt;
float calibrated_polaris_az;
float expected_polaris_az;
static float target_alt, target_az;


void FindCalibrationStars()
{
    if (calib_stars) {
        free(calib_stars);
    }
    calib_stars = (const CStar**)malloc(sizeof(CStar*)*16);
    calib_star_count = 0;

    long now = time();
    int i = 0;
    int numstars = GetStarCount();
    float alt, az;
    for (i = 0; i < numstars; i++) {
        const CStar* star = &StarList[i];
        if (star->star_name[0]) {
            if (star->visual_mag < 2.0) {
                star->GetAltAz(now, my_longitude, my_latitude, &alt, &az);
                if (alt > 15.0) {
                    if ((calib_star_count & 0xf) == 15) {
                        calib_stars = (const CStar**)realloc(calib_stars, sizeof(CStar*) * (calib_star_count + 17));
                    }
                    calib_stars[calib_star_count++] = star;
                }
            }
        }
    }
}


void
CCalibrateFinePage::Init()
{
    calibrate_second_star = false;
    polaris = NULL;

    txtClear();
    txtPrint("Seeking Home Position\n\n");
    laser->RecalibrateHome();
}


void
CCalibrateFinePage::Display()
{
    float alt, az;

    txtClear();
    txtPrint("Calibration\n\n");

    if (!calib_stars) {
        txtPrint("Getting calibration stars\n");
        polaris = FindStar("Polaris");
        FindCalibrationStars();
        polaris->GetAltAz(time(), my_longitude, my_latitude, &alt, &az);
        target_alt = alt;
        target_az = az;

	txtPrint("Seeking Home Position\n\n");
        laser->RecalibrateHome();
        laser->PointTo(az, alt);
        return;
    }

    if (calibrate_second_star) {
        txtPrintf("Aim laser at %s\n", calib_stars[curr_calib_star]->star_name);
    } else {
        txtPrint("Aim laser at Polaris       \n");
    }
    txtPrint("and press A or Enter.\n\n");

    txtPrint("Press B or Back to point the\n");
    txtPrint("laser near the target star.\n\n");

    txtPrintf("Star  Alt: %4.1f  Az: %5.1f\n", target_alt, target_az);
}


void
CCalibrateFinePage::HandleKeys(short key)
{
    laser->SlewByKeys();

    if (!key) {
        return;
    }

    float pol_alt, pol_az;
    float sec_alt, sec_az;
    polaris->GetAltAz(time(), my_longitude, my_latitude, &pol_alt, &pol_az);
    calib_stars[curr_calib_star]->GetAltAz(time(), my_longitude, my_latitude, &sec_alt, &sec_az);

    if (key == KI_ENTER) {
        if (calibrate_second_star) {
            calibrate_second_star = false;
            laser->CalibrateTwoStar(
                expected_polaris_alt, expected_polaris_az,
		calibrated_polaris_alt, calibrated_polaris_az,
                sec_alt, sec_az,
		laser->GetAltitude(), laser->GetAzimuth()
                );

            laser->PointTo(pol_az, pol_alt);
            target_alt = pol_alt;
            target_az = pol_az;
        } else {
            calibrate_second_star = true;
            laser->CalibrateAzimuthToAngle(pol_az);
            expected_polaris_alt = pol_alt;
            expected_polaris_az = pol_az;
            calibrated_polaris_alt = laser->GetAltitude();
            calibrated_polaris_az = laser->GetAzimuth();

            laser->PointTo(sec_az, sec_alt);
            target_alt = sec_alt;
            target_az = sec_az;
        }
    }
    if (key == KI_BACK) {
        if (calibrate_second_star) {
            laser->PointTo(sec_az, sec_alt);
        } else {
            laser->PointTo(pol_az, pol_alt);
        }
    }
    if (key == KI_PLOT) {
        if (calibrate_second_star) {
            const CStar* second_star = calib_stars[curr_calib_star];
            PlotSky(time(), second_star->constellation, laser->GetAltitude(), laser->GetAzimuth(), sec_alt, sec_az);
        } else {
            PlotSky(time(), polaris->constellation, laser->GetAltitude(), laser->GetAzimuth(), pol_alt, pol_az);
        }
    }
    if (key == KI_PREV) {
        curr_calib_star--;
        if (curr_calib_star < 0) {
            curr_calib_star = calib_star_count-1;
        }
        if (calibrate_second_star) {
            calib_stars[curr_calib_star]->GetAltAz(time(), my_longitude, my_latitude, &sec_alt, &sec_az);
            target_alt = sec_alt;
            target_az = sec_az;
        }
    }
    if (key == KI_NEXT) {
        curr_calib_star++;
        if (curr_calib_star >= calib_star_count) {
            curr_calib_star = 0;
        }
        if (calibrate_second_star) {
            calib_stars[curr_calib_star]->GetAltAz(time(), my_longitude, my_latitude, &sec_alt, &sec_az);
            target_alt = sec_alt;
            target_az = sec_az;
        }
    }
}

