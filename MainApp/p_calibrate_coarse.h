#ifndef P_CALIBRATE_COARSE_H
#define P_CALIBRATE_COARSE_H

#include "page.h"

class CCalibrateCoarsePage : public CPage {
public:
    CCalibrateCoarsePage(const char* title) : CPage(title) { }
    void Init();
    void Display();
    void HandleKeys(short key);
};

#endif

