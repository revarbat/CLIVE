#ifndef ORBITDATA_H
#define ORBITDATA_H

#include <math.h>
// #include <stdio.h>

#define J2000 946728000
#define CENTURIES_PER_SEC (1.0/(100.0*365.25*86400))
#define DEGS_PER_ARCSEC (1.0/(100.0*365.25*86400))
#define DAYS_PER_SEC (1.0/86400.0)

// Epoch:      Epoch Time
// Semimaj:    Semimajor AU
// Eccent:     Eccentricity
// Incline:    Inclination
// LongAsc:      Longitude of Ascending Node
// | RAAsc:      Right Ascention of Ascending Node
// LongPeri:     Longitude of Perihelion/Perigee/Periapsis
// | ArgPeri:    Argument of Perihelion/Perigee/Periapsis
// MeanAnom0:    Initial Mean Anomaly
// | MeanMotion: Motion per day.
// | MeanLong:   Mean Longitude


// LongAsc  = RAAsc * 15
// LongPeri = LongAsc + ArgPeri
// MeanLong = MeanAnomaly0 + LongPeri


// Orbital Data Types ---------------------------------------------------------------------------------
// PlanetCode:   SemiMaj MeanAnom0  ArgPeri  LongAsc Inclination Eccentricity
// PlanetPDF:    SemiMaj MeanLong   LongPeri LongAsc Inclination Eccentricity
// SatelliteTLE: SemiMaj MeanAnom0  ArgPeri  RAAsc   Inclination Eccentricity   MeanMotion MeanMotionD1 MeanMotionD2
// WebPage:      SemiMaj MeanMotion LongPeri LongAsc Inclination Eccentricity


class OrbitData {
public:
    OrbitData(long epoch) {
        epoch_time = epoch;

        semimajor_axis = 0.0;
        mean_anomaly_at_epoch = 0.0;
        argument_of_periapsis = 0.0;
        angle_ascending_node = 0.0;
        inclination = 0.0;
        eccentricity = 0.0;
        rotational_tilt = 0.0;
        mean_motion = 0.0;

        cr_semimajor_axis = 0.0;
        cr_mean_anomaly_at_epoch = 0.0;
        cr_argument_of_periapsis = 0.0;
        cr_angle_ascending_node = 0.0;
        cr_inclination = 0.0;
        cr_eccentricity = 0.0;
        cr_mean_motion = 0.0;
    }



    OrbitData(
        float semimaj,
        float mean_anomaly0,
        float arg_periapsis,
        float node_angle,
        float incline,
        float eccent,
        float tilt,
        long epoch
    ) {
        semimajor_axis = semimaj;
        mean_anomaly_at_epoch = mean_anomaly0;
        argument_of_periapsis = arg_periapsis;
        angle_ascending_node = node_angle;
        inclination = incline;
        eccentricity = eccent;
        rotational_tilt = tilt;
        epoch_time = epoch;
        mean_motion = 1.0146024953523785 * semimajor_axis * sqrt(semimajor_axis);

        cr_semimajor_axis = 0.0;
        cr_mean_anomaly_at_epoch = 0.0;
        cr_argument_of_periapsis = 0.0;
        cr_angle_ascending_node = 0.0;
        cr_inclination = 0.0;
        cr_eccentricity = 0.0;
        cr_mean_motion = 0.0;
    }



    OrbitData(
        float semimaj,
        float mean_anomaly0,
        float arg_periapsis,
        float node_angle,
        float incline,
        float eccent,
        float tilt,
        long epoch,
        float cr_semimaj,
        float cr_mean_anomaly0,
        float cr_arg_periapsis,
        float cr_node_angle,
        float cr_incline,
        float cr_eccent,
        float cr_tilt
    ) {
        semimajor_axis = semimaj;
        mean_anomaly_at_epoch = mean_anomaly0;
        argument_of_periapsis = arg_periapsis;
        angle_ascending_node = node_angle;
        inclination = incline;
        eccentricity = eccent;
        rotational_tilt = tilt;
        epoch_time = epoch;
        mean_motion = 0.9856076686/(semimajor_axis * sqrt(semimajor_axis));

        cr_semimajor_axis = cr_semimaj;
        cr_mean_anomaly_at_epoch = cr_mean_anomaly0;
        cr_argument_of_periapsis = cr_arg_periapsis;
        cr_angle_ascending_node = cr_node_angle;
        cr_inclination = cr_incline;
        cr_eccentricity = cr_eccent;
        cr_mean_motion = 0.0;
    }


    float
    SemiMajorAxis(long when) const
    {
        return semimajor_axis + (cr_semimajor_axis * (when - epoch_time));
    }

    void
    SetSemiMajorAxis(float semimaj)
    {
        semimajor_axis = semimaj;
        cr_semimajor_axis = 0.0;
        SetMeanDailyMotion(0.9856076686 / (semimajor_axis * sqrt(semimajor_axis)));
    }

    void
    SetSemiMajorAxis(float semimaj, float delta_per_century)
    {
        SetSemiMajorAxis(semimaj);
        cr_semimajor_axis = CENTURIES_PER_SEC * delta_per_century;
    }



    float
    MeanAnomaly(long when) const
    {
        float days_since_epoch = (when - epoch_time) * DAYS_PER_SEC;
        float M = MeanAnomaly0(when) + MeanDailyMotion(when) * days_since_epoch;
        if (M > 360.0) {
            M = fmod(M, 360.0);
        }
        return M;
    }

    float
    MeanAnomaly0(long when) const
    {
        return mean_anomaly_at_epoch + (cr_mean_anomaly_at_epoch * (when - epoch_time));
    }

    void
    SetMeanAnomaly0(float meananom0)
    {
        mean_anomaly_at_epoch = meananom0;
        cr_mean_anomaly_at_epoch = 0.0;
    }

    void
    SetMeanAnomaly0(float meananom0, float delta_per_century)
    {
        mean_anomaly_at_epoch = meananom0;
        cr_mean_anomaly_at_epoch = CENTURIES_PER_SEC * delta_per_century;
    }



    float
    MeanLongitude(long when) const
    {
        return MeanAnomaly0(when) + LongitudeOfPeriapsis(when);
    }

    void
    SetMeanLongitude(float meanlong)
    {
        float longperi = argument_of_periapsis + angle_ascending_node;
        mean_anomaly_at_epoch = meanlong - longperi;
        cr_mean_anomaly_at_epoch = 0.0;
    }

    void
    SetMeanLongitude(float meanlong, float delta_per_century)
    {
        float longperi = argument_of_periapsis + angle_ascending_node;
        mean_anomaly_at_epoch = meanlong - longperi;
        cr_mean_anomaly_at_epoch = CENTURIES_PER_SEC * DEGS_PER_ARCSEC * delta_per_century;
    }



    float
    MeanDailyMotion(long when) const
    {
        return mean_motion + (cr_mean_motion * (when - epoch_time));
    }

    void
    SetMeanDailyMotion(float meanmo)
    {
        mean_motion = meanmo;
        cr_mean_motion = 0.0;
    }

    void
    SetMeanDailyMotion(float meanmo, float delta_per_century)
    {
        mean_motion = meanmo;
        cr_mean_motion = CENTURIES_PER_SEC * delta_per_century;
    }



    float
    ArgumentOfPeriapsis(long when) const
    {
        return argument_of_periapsis + (cr_argument_of_periapsis * (when - epoch_time));
    }

    void
    SetArgumentOfPeriapsis(float argperi)
    {
        argument_of_periapsis = argperi;
        cr_argument_of_periapsis = 0.0;
    }

    void
    SetArgumentOfPeriapsis(float argperi, float delta_per_century)
    {
        argument_of_periapsis = argperi;
        cr_argument_of_periapsis = CENTURIES_PER_SEC * DEGS_PER_ARCSEC * delta_per_century;
    }



    float
    LongitudeOfPeriapsis(long when) const
    {
        long delttime = when - epoch_time;
        float argperi = argument_of_periapsis + (cr_argument_of_periapsis * delttime);
        float ascnode = angle_ascending_node + (cr_angle_ascending_node * delttime);
        return argperi + ascnode;
    }

    void
    SetLongitudeOfPeriapsis(float longperi)
    {
        argument_of_periapsis = longperi - angle_ascending_node;
        cr_argument_of_periapsis = 0.0;
    }

    void
    SetLongitudeOfPeriapsis(float longperi, float delta_per_century)
    {
        argument_of_periapsis = longperi - angle_ascending_node;
        cr_argument_of_periapsis = CENTURIES_PER_SEC * DEGS_PER_ARCSEC * delta_per_century;
    }



    float
    LongitudeOfAscendingNode(long when) const
    {
        return angle_ascending_node + (cr_angle_ascending_node * (when - epoch_time));
    }

    void
    SetLongitudeOfAscendingNode(float ascnode)
    {
        angle_ascending_node = ascnode;
        cr_angle_ascending_node = 0.0;
    }

    void
    SetLongitudeOfAscendingNode(float ascnode, float delta_per_century)
    {
        angle_ascending_node = ascnode;
        cr_angle_ascending_node = CENTURIES_PER_SEC * DEGS_PER_ARCSEC * delta_per_century;
    }



    float
    OrbitalInclination(long when) const
    {
        return inclination + (cr_inclination * (when - epoch_time));
    }

    void
    SetOrbitalInclination(float incl)
    {
        inclination = incl;
        cr_inclination = 0.0;
    }

    void
    SetOrbitalInclination(float incl, float delta_per_century)
    {
        inclination = incl;
        cr_inclination = CENTURIES_PER_SEC * DEGS_PER_ARCSEC * delta_per_century;
    }



    float
    Eccentricity(long when) const
    {
        return eccentricity + (cr_eccentricity * (when - epoch_time));
    }

    void
    SetEccentricity(float incl)
    {
        eccentricity = incl;
        cr_eccentricity = 0.0;
    }

    void
    SetEccentricity(float incl, float delta_per_century)
    {
        eccentricity = incl;
        cr_eccentricity = CENTURIES_PER_SEC * delta_per_century;
    }



    float
    RotationalTilt() const
    {
        return rotational_tilt;
    }

    void
    SetRotationalTilt(float tilt)
    {
        rotational_tilt = tilt;
    }



    float
    EpochTime() const
    {
        return epoch_time;
    }

    void
    SetEpochTime(long epoch)
    {
        epoch_time = epoch;
    }



private:
    long  epoch_time;                //

    float semimajor_axis;            //
    float mean_motion;               //
    float mean_anomaly_at_epoch;     //
    float argument_of_periapsis;     //
    float angle_ascending_node;      //
    float inclination;               //
    float eccentricity;              //

    float cr_semimajor_axis;         //
    float cr_mean_motion;            //
    float cr_mean_anomaly_at_epoch;  //
    float cr_argument_of_periapsis;  //
    float cr_angle_ascending_node;   //
    float cr_inclination;            //
    float cr_eccentricity;           //

    float rotational_tilt;           //
};

#endif

