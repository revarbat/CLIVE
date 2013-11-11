#ifndef P_CONFIG_INT_H
#define P_CONFIG_INT_H

#include "page.h"

class CConfigIntPage : public CPage {
public:
    CConfigIntPage(const char* title, int* intvar, int minval, int maxval);
    virtual ~CConfigIntPage();

    void Init();
    void Display();
    void HandleKeys(short key);

private:
    int* value_variable;
    int minimum_value;
    int maximum_value;
    int previous_value;
    CPage* previous_page;
};

#endif

