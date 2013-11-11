#ifndef LASER_H
#define LASER_H

#include "turret.h"
#include "flare.h"
#include "celestial.h"


extern int turret_speed_low;
extern int turret_speed_high;
extern long turret_azimuth_backlash;
extern long turret_altitude_backlash;

enum EnumBeamState {
    BEAM_OFF,
    BEAM_ON
};


class CLaser {
public:
    CLaser();
    ~CLaser();

    float GetAzimuthRaw();
    float GetAzimuth();
    void SetAzimuth(float az);
    float GetAltitudeRaw();
    float GetAltitude();
    void SetAltitude(float alt);
    void PointTo(float az, float alt);
    void PointToFlare(const CFlare* flare);
    void PointToNextFlare();
    void RecalibrateHome();
    void CalibrateTwoStar(float exp_alt1, float exp_az1, float meas_alt1, float meas_az1, float exp_alt2, float exp_az2, float meas_alt2, float meas_az2);
    void CalibrateThreeStar(float exp_alt1, float exp_az1, float meas_alt1, float meas_az1, float exp_alt2, float exp_az2, float meas_alt2, float meas_az2, float exp_alt3, float exp_az3, float meas_alt3, float meas_az3);
    void CalibrateAltitude(float expected1, float measured1, float expected2, float measured2);
    void CalibrateAltitudeZero();
    void CalibrateAltitudeToAngle(float angle);
    void CalibrateAzimuthToAngle(float angle);
    void AdjustAltitude(long by, short speed);
    void AdjustAzimuth(long by, short speed);
    EnumBeamState GetBeamState() const;
    void SetBeamState(EnumBeamState state);
    void SlewByKeys();
    short GetSpeed();
    void SetSpeed(short speed);
    bool GetActiveHolding();
    void SetActiveHolding(bool onval);
    void UpdateBacklash();

    float azimuth_north_offset;
    float altitude_zero_offset;
    float altitude_vert_offset;

    CMatrix align_data;

private:
    CTurret* turret;
    short movement_speed;
};

#endif

