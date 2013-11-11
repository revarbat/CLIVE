#ifndef P_CONFIG_LONG_H
#define P_CONFIG_LONG_H

#include "page.h"

class CConfigLongPage : public CPage {
public:
    CConfigLongPage(const char* title, long* intvar, long minval, long maxval);
    virtual ~CConfigLongPage();

    void Init();
    void Display();
    void HandleKeys(short key);

private:
    long* value_variable;
    long minimum_value;
    long maximum_value;
    long previous_value;
    CPage* previous_page;
};

#endif

