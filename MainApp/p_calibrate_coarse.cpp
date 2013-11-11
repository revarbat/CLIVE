#include <math.h>
#include "fbtext.h"
#include "page.h"
#include "p_calibrate_coarse.h"
#include "laser.h"
#include "fbtimer.h"
#include "gpio.h"
#include "keysin.h"
#include "star.h"
#include "starlist.h"
#include "skyplot.h"
#include "flarelist.h"
#include "gps.h"

bool calibrate_zero_alt = true;
extern CLaser* laser;



void
CCalibrateCoarsePage::Init()
{
    laser->PointTo(0.0,0.0);
    calibrate_zero_alt = true;
}


void
CCalibrateCoarsePage::Display()
{
    txtSetPosition(0,0);

    txtPrint("Calibration\n\n");
    if (calibrate_zero_alt) {
        txtPrint("Aim laser at zero altitude  \n");
    } else {
        txtPrint("Aim laser at 90 degrees up  \n");
    }
    txtPrint("and press A or Enter.\n\n");
}


void
CCalibrateCoarsePage::HandleKeys(short key)
{
    laser->SlewByKeys();
    switch (key) {
        case KI_ENTER: {
            if (calibrate_zero_alt) {
                calibrate_zero_alt = false;
                laser->CalibrateAltitudeZero();
                laser->PointTo(0.0,90.0);
            } else {
                calibrate_zero_alt = true;
                laser->CalibrateAltitudeToAngle(90.0);
                laser->PointTo(0.0,0.0);
            }
            UpdateDisplay();
            break;
        }
        case KI_BACK: {
            if (calibrate_zero_alt) {
                laser->PointTo(0.0,0.0);
            } else {
                laser->PointTo(0.0,90.0);
            }
            UpdateDisplay();
            break;
        }
        case KI_PLOT: {
            float alt, az;
            alt = 0.0;
            az = 0.0;
            if (!calibrate_zero_alt) {
		alt = 90.0;
            }
            PlotSky(time(), NULL, laser->GetAltitudeRaw(), laser->GetAzimuthRaw(), alt, az);
            UpdateDisplay();
            break;
        }

        default: {
            break;
        }
    }
}

