#ifndef P_INFO_H
#define P_INFO_H

#include "page.h"

class CInfoPage : public CPage {
public:
    CInfoPage(const char* title) : CPage(title) { }
    void Init();
    void Display();
    void HandleKeys(short key);
};

#endif

