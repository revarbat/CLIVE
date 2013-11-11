#ifndef P_DEBUG_H
#define P_DEBUG_H

#include "page.h"

class CDebugPage : public CPage {
public:
    CDebugPage(const char* title) : CPage(title) { }
    void Init();
    void Display();
    void HandleKeys(short key);
};

#endif

