#ifndef P_STARFINDER_H
#define P_STARFINDER_H

#include "page.h"

class CStarFinderPage : public CPage {
public:
    CStarFinderPage(const char* title);
    void Init();
    void Display();
    void HandleKeys(short key);
};

#endif

