#include <stdlib.h>
#include <math.h>
#include "fbtext.h"
#include "page.h"
#include "p_calibrate_3star.h"
#include "laser.h"
#include "fbtimer.h"
#include "gpio.h"
#include "keysin.h"
#include "star.h"
#include "starlist.h"
#include "skyplot.h"
#include "flarelist.h"
#include "gps.h"

extern CLaser* laser;


void
CCalibrate3StarPage::FindCalibrationStars()
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
    // find all bright calibration star candidates, including Polaris, that are between 10 and 75 degrees altitude.
    for (i = 0; i < numstars; i++) {
        const CStar* star = &StarList[i];
        if (star->star_name[0]) {
            if (star->visual_mag <= 1.5 || !strcmp(star->star_name, "Polaris")) {
                star->GetAltAz(now, my_longitude, my_latitude, &alt, &az);
                if (alt > 10.0 && alt < 75.0) {
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
CCalibrate3StarPage::Init()
{
    calibration_state = FIRST_STAR;

    txtClear();
    txtPrint("Seeking Home Position\n\n");
    laser->RecalibrateHome();
}


void
CCalibrate3StarPage::Display()
{
    float alt, az;
    short polaris = -1;
    short best_star_count = sizeof(best_stars)/sizeof(short);

    txtClear();
    txtPrint("Calibration\n\n");

    if (!calib_stars) {
        txtPrint("Getting calibration stars\n");
        FindCalibrationStars();

	best_stars[0] = -1;
	best_stars[1] = -1;
	best_stars[2] = -1;

	// Find the three brightest calibration star candidates.
	for (short i = 0; i < calib_star_count; i++) {
	    const CStar* star = calib_stars[i];
	    if (polaris < 0 && !strcmp(star->star_name, "Polaris")) {
		polaris = i;
	    }
	    for (short j = best_star_count-1; j >= 0; j--) {
		if (best_stars[j] < 0 || star->visual_mag < calib_stars[best_stars[j]]->visual_mag) {
		    if (j < best_star_count-1) {
		        best_star[j+1] = best_star[j];
		    }
		    best_star[j] = i;
		}
	    }
	}
	if (polaris >= 0) {
	    // If Polaris was a candidate, and isn't already in the list of best stars, add it at the beginning.
	    if (calib_stars[best_stars[best_star_count-1]]->visual_mag > calib_stars[best_stars[polaris]]->visual_mag) {
		for (short j = best_star_count-2; j >= 0; j--) {
		    best_star[j+1] = best_star[j];
		}
		best_star[0] = polaris;
	    }
	}
	curr_calib_star = best_stars[0];

	txtPrint("Pointing to first star\n");
	calib_stars[curr_calib_star]->GetAltAz(time(), my_longitude, my_latitude, &alt, &az);
	target_alt = alt;
	target_az = az;
        laser->PointTo(az, alt);

        return;
    }

    switch (calibration_state) {
        case FIRST_STAR:
	    txtPrint("Point to First Star\n\n");
	    break;
        case SECOND_STAR:
	    txtPrint("Point to Second Star\n\n");
	    break;
        case THIRD_STAR:
	    txtPrint("Point to Third Star\n\n");
	    break;
	case VERIFY:
	    txtPrint("Calibration complete.\n\n");
	    txtPrint("Press A or Enter to save.\n");
	    txtPrint("Press B or Back to cancel.\n\n");
	    return;
	    break;
	case DONE:
	    txtPrint("Calibration has been saved.\n\n");
	    txtPrint("Press A or Enter to recalibrate.\n\n");
	    return;
	    break;
    }
    txtPrintf("Aim laser at %s\n", calib_stars[curr_calib_star]->star_name);
    txtPrint("and press A or Enter.\n\n");

    txtPrint("Press B or Back to point the\n");
    txtPrint("laser near the target star.\n\n");

    txtPrintf("Star  Alt: %4.1f  Az: %5.1f\n", target_alt, target_az);
}


void
CCalibrate3StarPage::HandleKeys(short key)
{
    laser->SlewByKeys();

    if (!key) {
        return;
    }

    float currstar_alt, currstar_az;
    const CStar* currstar = calib_stars[curr_calib_star];

    currstar->GetAltAz(time(), my_longitude, my_latitude, &currstar_alt, &currstar_az);

    if (key == KI_ENTER) {
	switch (calibration_state) {
	    case FIRST_STAR:
		calibration_state = SECOND_STAR;
		laser->CalibrateAzimuthToAngle(currstar_az);
		expected_star_alt[0] = currstar_alt;
		expected_star_az[0] = currstar_az;
		calibrated_star_alt[0] = laser->GetAltitude();
		calibrated_star_az[0] = laser->GetAzimuth();

		curr_calib_star = best_stars[1];
		calib_stars[curr_calib_star]->GetAltAz(time(), my_longitude, my_latitude, &currstar_alt, &currstar_az);

		laser->PointTo(currstar_az, currstar_alt);
		target_alt = currstar_alt;
		target_az = currstar_az;
		break;

	    case SECOND_STAR:
		calibration_state = THIRD_STAR;
		expected_star_alt[1] = currstar_alt;
		expected_star_az[1] = currstar_az;
		calibrated_star_alt[1] = laser->GetAltitudeRaw();
		calibrated_star_az[1] = laser->GetAzimuthRaw();

		curr_calib_star = best_stars[2];
		calib_stars[curr_calib_star]->GetAltAz(time(), my_longitude, my_latitude, &currstar_alt, &currstar_az);

		laser->PointTo(currstar_az, currstar_alt);
		target_alt = currstar_alt;
		target_az = currstar_az;
		break;

	    case THIRD_STAR:
		calibration_state = VERIFY;
		expected_star_alt[2] = currstar_alt;
		expected_star_az[2] = currstar_az;
		calibrated_star_alt[2] = laser->GetAltitudeRaw();
		calibrated_star_az[2] = laser->GetAzimuthRaw();
		break;

	    case VERIFY:
		calibration_state = DONE;
		laser->CalibrateThreeStar(
		    expected_star_alt[0], expected_star_az[0],
		    calibrated_star_alt[0], calibrated_star_az[0],
		    expected_star_alt[1], expected_star_az[1],
		    calibrated_star_alt[1], calibrated_star_az[1],
		    expected_star_alt[2], expected_star_az[2],
		    calibrated_star_alt[2], calibrated_star_az[2]
		    );
		free(calib_stars);
		calib_stars = NULL;
		calib_star_count = 0;
		break;

	    case DONE:
		calibration_state = FIRST_STAR;
		curr_calib_star = best_stars[0];
		calib_stars[curr_calib_star]->GetAltAz(time(), my_longitude, my_latitude, &currstar_alt, &currstar_az);

		laser->PointTo(currstar_az, currstar_alt);
		target_alt = currstar_alt;
		target_az = currstar_az;

		txtClear();
		txtPrint("Seeking Home Position\n\n");
		laser->RecalibrateHome();
		break;

	}
    }
    if (key == KI_BACK) {
	if (calibration_state == VERIFY || calibration_state == DONE) {
	    calibration_state = FIRST_STAR;
	    curr_calib_star = best_stars[0];
	    calib_stars[curr_calib_star]->GetAltAz(time(), my_longitude, my_latitude, &currstar_alt, &currstar_az);

	    target_alt = currstar_alt;
	    target_az = currstar_az;
	}
	laser->PointTo(currstar_az, currstar_alt);
    }
    if (key == KI_PLOT) {
	PlotSky(time(), currstar->constellation, laser->GetAltitude(), laser->GetAzimuth(), currstar_alt, currstar_az);
    }
    if (key == KI_PREV) {
        curr_calib_star--;
        if (curr_calib_star < 0) {
            curr_calib_star = calib_star_count-1;
        }
	calib_stars[curr_calib_star]->GetAltAz(time(), my_longitude, my_latitude, &currstar_alt, &currstar_az);
	target_alt = currstar_alt;
	target_az = currstar_az;
    }
    if (key == KI_NEXT) {
        curr_calib_star++;
        if (curr_calib_star >= calib_star_count) {
            curr_calib_star = 0;
        }
	calib_stars[curr_calib_star]->GetAltAz(time(), my_longitude, my_latitude, &currstar_alt, &currstar_az);
	target_alt = currstar_alt;
	target_az = currstar_az;
    }
}

