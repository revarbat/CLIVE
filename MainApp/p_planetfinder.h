#ifndef P_PLANETFINDER_H
#define P_PLANETFINDER_H

#include "page.h"

class CPlanetFinderPage : public CPage {
public:
    CPlanetFinderPage(const char* title) : CPage(title) { }
    void Init();
    void Display();
    void HandleKeys(short key);
};

#endif

