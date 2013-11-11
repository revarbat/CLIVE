#ifndef P_TRACKER_H
#define P_TRACKER_H

#include "page.h"

class CTrackerPage : public CPage {
public:
    CTrackerPage(const char* title) : CPage(title) { }
    void Init();
    void Display();
    void HandleKeys(short key);
};

#endif

