#ifndef P_CALIBRATE_H
#define P_CALIBRATE_H

#include "page.h"

class CCalibrateFinePage : public CPage {
public:
    CCalibrateFinePage(const char* title) : CPage(title) { }
    void Init();
    void Display();
    void HandleKeys(short key);
};

#endif

