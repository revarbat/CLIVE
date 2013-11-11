#include <stdlib.h>
#include "fbtext.h"
#include "page.h"
#include "p_config_bool.h"
#include "keysin.h"


CConfigBoolPage::CConfigBoolPage(char* title, bool* boolvar) :
    CPage(title),
    value_variable(boolvar),
    previous_value(0),
    previous_page(NULL)
{
}


CConfigBoolPage::~CConfigBoolPage()
{
}


void CConfigBoolPage::Init()
{
    previous_value = *value_variable;
    previous_page = current_page;
}


void CConfigBoolPage::Display()
{
    txtClear();
    txtPrintf("%s\n\n", GetTitle());

    txtPrint("Curr value: ");
    txtAttrBold();
    if (*value_variable) {
        txtPrint("True ");
    } else {
        txtPrint("False");
    }
    txtAttrReset();
}



void CConfigBoolPage::HandleKeys(short key)
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
        *value_variable = !*value_variable;
    }

    if (key == KI_DOWN || key == KI_NEXT) {
        *value_variable = !*value_variable;
    }

    UpdateDisplay();
}


