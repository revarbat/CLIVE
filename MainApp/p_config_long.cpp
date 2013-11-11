#include <stdlib.h>
#include "fbtext.h"
#include "page.h"
#include "p_config_long.h"
#include "laser.h"
#include "fbtimer.h"
#include "keysin.h"
#include "flare.h"
#include "flarelist.h"
#include "gps.h"
#include "skyplot.h"



CConfigLongPage::CConfigLongPage(
    char* title,
    long* intvar,
    long minval,
    long maxval
) :
    CPage(title),
    value_variable(intvar),
    minimum_value(minval),
    maximum_value(maxval),
    previous_value(0),
    previous_page(NULL)
{
}


CConfigLongPage::~CConfigLongPage()
{
}


void CConfigLongPage::Init()
{
    previous_value = *value_variable;
    previous_page = current_page;
}


void CConfigLongPage::Display()
{
    txtClear();
    txtPrintf("%s\n\n", GetTitle());

    txtPrint("Curr value: ");
    txtAttrBold();
    txtPrintf("%d", *value_variable);
    txtAttrReset();
}



void CConfigLongPage::HandleKeys(short key)
{
    if (!key) {
        return;
    }

    if (key == KI_BACK) {
        *value_variable = previous_value;
        current_page = previous_page;
    }

    if (key == KI_ENTER) {
        current_page = previous_page;
    }

    if (key == KI_UP || key == KI_PREV) {
        int delta = 1;
        if (IsKeyDown(KI_FAST)) {
            delta = 10;
        }
        if (*value_variable + delta < maximum_value) {
            *value_variable += delta;
        } else {
            *value_variable = maximum_value;
        }
    }

    if (key == KI_DOWN || key == KI_NEXT) {
        int delta = 1;
        if (IsKeyDown(KI_FAST)) {
            delta = 10;
        }
        if (*value_variable - delta > minimum_value) {
            *value_variable -= delta;
        } else {
            *value_variable = minimum_value;
        }
    }

    UpdateDisplay();
}


