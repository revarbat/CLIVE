#include <math.h>
#include <stdlib.h>
#include "laser.h"
#include "gpio.h"
#include "turret.h"
#include "flare.h"
#include "flarelist.h"
#include "keysin.h"
#include "celestial.h"


#define U8 unsigned char
#define U16 unsigned short

#define MICROSTEPS_PER_STEP 16

#define AZ_STEPS 115200L
#define ALT_STEPS 115200L


int turret_speed_low  =  10;
int turret_speed_high = 50;
long turret_azimuth_backlash = 0;
long turret_altitude_backlash = 0;

CLaser::CLaser() : align_data(3,3)
{
    azimuth_north_offset =   0.0;
    altitude_zero_offset =   0.0;
    altitude_vert_offset =  90.0;
    align_data = TwoStarAlignMatrix(10, 0, 10, 0, 80, 120, 80, 120);
    movement_speed = turret_speed_low;

    GpioSetDirOutbound(LASER_CNTL_PIN, true);   // Laser on/off

    turret = new CTurret(&turret_uart);
    turret->SetRotationLimits(0,-AZ_STEPS/2,AZ_STEPS/2);
    turret->SetRotationLimits(1,-ALT_STEPS/2,ALT_STEPS/2);

    turret_azimuth_backlash = 0;
    turret_altitude_backlash = 110;

    UpdateBacklash();
    RecalibrateHome();
}


CLaser::~CLaser()
{
    delete turret;
}


void CLaser::UpdateBacklash()
{
    turret->SetBacklash(0,turret_azimuth_backlash, turret_speed_low);
    turret->SetBacklash(1,turret_altitude_backlash, turret_speed_low);
}


float CLaser::GetAzimuthRaw()
{
    float azimuth = turret->GetCurrPos(0)*360.0/AZ_STEPS;
    while (azimuth < 0.0)
        azimuth += 360.0;
    while (azimuth > 360.0)
        azimuth -= 360.0;
    return azimuth;
}


float CLaser::GetAzimuth()
{
    float azimuth = GetAzimuthRaw() - azimuth_north_offset;
    while (azimuth < 0.0)
        azimuth += 360.0;
    while (azimuth > 360.0)
        azimuth -= 360.0;
    return azimuth;
}


void CLaser::SetAzimuth(float az)
{
    az += azimuth_north_offset;
    while (az > 180) {
        az -= 360;
    }
    while (az < -180) {
        az += 360;
    }
    long azpos = (long)(az*AZ_STEPS/360.0+0.5);
    UpdateBacklash();
    turret->RotateToPos(0, azpos, movement_speed);
}


float CLaser::GetAltitudeRaw()
{
    float altitude = turret->GetCurrPos(1)*360.0/ALT_STEPS;
    while (altitude > 360.0)
        altitude -= 360.0;
    return altitude;
}


float CLaser::GetAltitude()
{
    float alt = GetAltitudeRaw() - altitude_zero_offset;
    alt = alt*90.0/altitude_vert_offset;
    return alt;
}


void CLaser::SetAltitude(float alt)
{
    alt = alt*altitude_vert_offset/90.0;
    alt += altitude_zero_offset;
    long altpos = (long)(alt*ALT_STEPS/360.0+0.5);
    UpdateBacklash();
    turret->RotateToPos(1, altpos, movement_speed);
}


void CLaser::PointTo(float az, float alt)
{
    float newalt, newaz;
    float curralt, curraz;
    float dalt, daz;
    long spalt, spaz;

    curralt = GetAltitude();
    curraz  = GetAzimuth();
    CompensateAltAz(align_data, alt, az, &newalt, &newaz);
    dalt = newalt - curralt;
    daz  = newaz  - curraz;
    spalt = spaz = movement_speed;
    if (dalt > daz) {
        spaz  = spaz * (daz/dalt);
    } else {
        spalt  = spaz * (dalt/daz);
    }
    SetSpeed(spaz);
    SetAzimuth(newaz);
    SetSpeed(spalt);
    SetAltitude(newalt);
}


void CLaser::PointToFlare(const CFlare* flare)
{
    if (flare) {
        PointTo(flare->GetAzimuth(), flare->GetAltitude());
    }
}


void CLaser::PointToNextFlare()
{
    PointToFlare(nextflare);
}


void CLaser::RecalibrateHome()
{
    EnumBeamState state = GetBeamState();
    SetBeamState(BEAM_OFF);
    turret->RecalibrateHome(turret_speed_high);
    turret->SetZeroPos(0);
    turret->SetZeroPos(1);
    SetSpeed(turret_speed_high);
    SetBeamState(state);
}


void CLaser::CalibrateTwoStar(float exp_alt1, float exp_az1, float meas_alt1, float meas_az1, float exp_alt2, float exp_az2, float meas_alt2, float meas_az2)
{
    align_data = TwoStarAlignMatrix(
        exp_alt1, exp_az1, meas_alt1, meas_az1,
        exp_alt2, exp_az2, meas_alt2, meas_az2
        );
}


void CLaser::CalibrateThreeStar(float exp_alt1, float exp_az1, float meas_alt1, float meas_az1, float exp_alt2, float exp_az2, float meas_alt2, float meas_az2, float exp_alt3, float exp_az3, float meas_alt3, float meas_az3)
{
    align_data = ThreeStarAlignMatrix(
        exp_alt1, exp_az1, meas_alt1, meas_az1,
        exp_alt2, exp_az2, meas_alt2, meas_az2,
        exp_alt3, exp_az3, meas_alt3, meas_az3
        );
}


void CLaser::CalibrateAltitude(float expected1, float measured1, float expected2, float measured2)
{
    float slope = (measured1 - measured2) / (expected1 - expected2);
    float offset = measured1 - slope * expected1;
    altitude_zero_offset = offset;
    altitude_vert_offset = 90.0 * slope;
}


void CLaser::CalibrateAltitudeZero()
{
    altitude_zero_offset = GetAltitudeRaw();
}


void CLaser::CalibrateAltitudeToAngle(float angle)
{
    float curralt = GetAltitudeRaw() - altitude_zero_offset;
    altitude_vert_offset = curralt * 90.0 / angle;
}


void CLaser::CalibrateAzimuthToAngle(float angle)
{
    azimuth_north_offset = GetAzimuthRaw() - angle;
}


void CLaser::AdjustAzimuth(long by, short speed)
{
    long azpos = turret->GetCurrPos(0) + by;
    UpdateBacklash();
    if (azpos*2 > AZ_STEPS) {
        EnumBeamState state = GetBeamState();
        SetBeamState(BEAM_OFF);

        azpos -= AZ_STEPS;
        turret->RotateToPos(0, azpos, turret_speed_high);
        turret->WaitUntilStopped(0);

        SetBeamState(state);
    } else if (azpos*2 < -AZ_STEPS) {
        EnumBeamState state = GetBeamState();
        SetBeamState(BEAM_OFF);

        azpos += AZ_STEPS;
        turret->RotateToPos(0, azpos, turret_speed_high);
        turret->WaitUntilStopped(0);

        SetBeamState(state);
    } else {
        turret->RotateToPos(0, azpos, speed);
    }
}


void CLaser::AdjustAltitude(long by, short speed)
{
    if (altitude_vert_offset < 0) {
        by = - by;
    }
    long oldpos = turret->GetCurrPos(1);
    UpdateBacklash();
    turret->RotateToPos(1, oldpos+by, speed);
}


EnumBeamState CLaser::GetBeamState() const
{
    if (GpioGetLine(LASER_CNTL_PIN)) {
        return BEAM_ON;
    } else {
        return BEAM_OFF;
    }
}


void CLaser::SetBeamState(EnumBeamState state)
{
    GpioSetLine(LASER_CNTL_PIN, (state == BEAM_ON));
}


short CLaser::GetSpeed()
{
    return movement_speed;
}


void CLaser::SetSpeed(short speed)
{
    movement_speed = speed;
}


void CLaser::SlewByKeys()
{
    long amount = 320;
    short speed = turret_speed_low;

    if (IsKeyDown(KI_SLOW)) {
        amount = 32;
        speed = turret_speed_low;
    } else if (IsKeyDown(KI_FAST)) {
        amount = 3200;
        speed = turret_speed_high;
    }

    if (IsKeyDown(KI_LEFT)) {
        AdjustAzimuth(amount, speed);
    } else if (IsKeyDown(KI_RIGHT)) {
        AdjustAzimuth(-amount, speed);
    }

    if (IsKeyDown(KI_UP)) {
        AdjustAltitude(amount, speed);
    } else if (IsKeyDown(KI_DOWN)) {
        AdjustAltitude(-amount, speed);
    }
}


bool CLaser::GetActiveHolding()
{
    return turret->GetActiveHolding(0);
}


void CLaser::SetActiveHolding(bool onval)
{
    turret->SetActiveHolding(0, onval);
    turret->SetActiveHolding(1, onval);
}


