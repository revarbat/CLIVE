#ifndef P_CALIBRATE_H
#define P_CALIBRATE_H

#include "page.h"
#include "star.h"

class CCalibrate3StarPage : public CPage {
public:
    CCalibrate3StarPage(const char* title) :
      CPage(title),
      target_alt(0.0),
      target_az(0.0),
      calib_stars(NULL),
      calib_star_count(0),
      curr_calib_star(0)
    {
    }

    void Init();
    void Display();
    void HandleKeys(short key);

private:
    void FindCalibrationStars();

    enum {FIRST_STAR, SECOND_STAR, THIRD_STAR, VERIFY, DONE} calibration_state;
    short best_stars[3];
    float expected_star_az[3];
    float expected_star_alt[3];
    float calibrated_star_az[3];
    float calibrated_star_alt[3];
    float target_alt, target_az;
    const CStar** calib_stars;
    long calib_star_count;
    long curr_calib_star;
};

#endif

