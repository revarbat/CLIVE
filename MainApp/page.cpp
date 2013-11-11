#include <stdlib.h>
#include <string.h>
#include "fbtext.h"
#include "page.h"
#include "flarelist.h"
#include "keysin.h"
#include "laser.h"
#include "config.h"


extern CLaser* laser;
CPage *current_page;


CPage::CPage(const char* title) {
    this->title = new char[strlen(title)+1];
    strcpy(this->title, title);
}


CPage::~CPage()
{
    delete title;
}




CMenuPage* main_menu_page;

CMenuPage::CMenuPage(const char* title) : CPage(title)
{
    selected_item = 0;
    itemcount = 0;
    listsize = 10;
    previous_page = NULL;
    itemlist = (CPage**)malloc(listsize*sizeof(CPage*));
}


CMenuPage::~CMenuPage()
{
    free(itemlist);
}


void CMenuPage::AddItem(CPage* item)
{
    if (itemcount >= listsize) {
        listsize += 5;
        itemlist = (CPage**)realloc(itemlist, sizeof(CPage*)*listsize);
    }
    itemlist[itemcount++] = item;
}


void CMenuPage::Init()
{
    previous_page = current_page;
}


void CMenuPage::Display()
{
    txtClear();
    txtPrintf("%s:\n\n", GetTitle());
    short i = 0;
    for (; i < itemcount; i++) {
        if (i == selected_item) {
	    txtAttrFlash();
	    txtAttrInverse();
	}
        txtPrintf("  %-21s\n", itemlist[i]->GetTitle());
    }
    if (i == selected_item) {
	txtAttrFlash();
	txtAttrInverse();
    }
    txtPrintf("  %-21s", "Exit Menu");
    txtAttrReset();
}




void CMenuPage::HandleKeys(short key)
{
    if (key == KI_ENTER || key == KI_RIGHT) {
        if (selected_item == itemcount) {
            current_page = previous_page;
        } else {
            itemlist[selected_item]->Init();
            current_page = itemlist[selected_item];
        }

    } else if (key == KI_BACK || key == KI_LEFT) {
        current_page = previous_page;

    } else if (key == KI_UP || key == KI_PREV) {
        selected_item = selected_item - 1;
        if (selected_item < 0) {
            selected_item = itemcount;
        }

    } else if (key == KI_DOWN || key == KI_NEXT) {
        selected_item = (selected_item + 1) % (itemcount + 1);

    }

    if (key != 0) {
        UpdateDisplay();
    }
}



extern CPage* tracker_page;



void
UpdateDisplay()
{
    float azimuth =  laser->GetAzimuth();
    float altitude = laser->GetAltitude();
    EnumBeamState state = laser->GetBeamState();

    if (current_page != tracker_page) {
        if (nextflare && TimeUntilNextFlare() < FLARE_ALERT_START) {
            txtClear();
            txtPrint("          ");
	    txtAttrBold();
	    txtAttrFlash();
            txtPrint(" Flare imminent! ");
	    txtAttrReset();
            txtPrint("\n\n");
            txtPrint("  Switching to Flare Tracker!\n\n");
            sleep(3);
            current_page = tracker_page;
            return;
        }
    }

    current_page->Display();

    txtSetPosition(0,19);
    txtPrintf("Laser:%-3s  Alt %4.1f  Az %5.1f", ((state == BEAM_ON)? "ON" : "Off"), altitude, azimuth);
}


void
HandleKeys()
{
    short key = GetKey();

    if (key == KI_MENU) {
        main_menu_page->Init();
        current_page = main_menu_page;
        UpdateDisplay();
    }

    if (key == KI_BEAM) {
        if (laser->GetBeamState() == BEAM_ON) {
            laser->SetBeamState(BEAM_OFF);
        } else {
            laser->SetBeamState(BEAM_ON);
        }
        UpdateDisplay();
    }

    current_page->HandleKeys(key);
}


