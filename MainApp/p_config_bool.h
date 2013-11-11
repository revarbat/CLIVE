#ifndef P_CONFIG_BOOL_H
#define P_CONFIG_BOOL_H

#include "page.h"

class CConfigBoolPage : public CPage {
public:
    CConfigBoolPage(const char* title, bool* boolvar);
    virtual ~CConfigBoolPage();

    void Init();
    void Display();
    void HandleKeys(short key);

private:
    bool* value_variable;
    bool previous_value;
    CPage* previous_page;
};

#endif

