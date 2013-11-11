#include "keysin.h"
#include "keypad.h"
#include "strobesense.h"

CKeysIn* keyin;

CKeysIn::CKeysIn()
{
    keyin = this;
}


CKeysIn::~CKeysIn()
{
}


void CKeysIn::Flush()
{
    while (keybuf.Count() > 0) {
        keybuf.Get();
    }
}


short CKeysIn::GetKeyCount()
{
    return (keybuf.Count());
}


short CKeysIn::GetKey()
{
    if (keybuf.Count() <= 0) {
        return 0;
    }
    return keybuf.Get();
}


bool CKeysIn::IsKeyDown(short key)
{
    switch (key) {
        case KI_ENTER:
            return ((lastkeys & KEY_A) || (sslastkeys & SS_KEY_ENTER));
            break;
        case KI_BACK:
            return ((lastkeys & KEY_B) || (sslastkeys & SS_KEY_BACK));
            break;
        case KI_SLOW:
            return ((lastkeys & KEY_R) || (sslastkeys & SS_KEY_SLOW));
            break;
        case KI_FAST:
            return ((lastkeys & KEY_L) || (sslastkeys & SS_KEY_FAST));
            break;
        case KI_BEAM:
            return ((lastkeys & KEY_SELECT) || (sslastkeys & SS_KEY_BEAM));
            break;
        case KI_MENU:
            return ((lastkeys & KEY_START) || (sslastkeys & SS_KEY_MENU));
            break;
        case KI_UP:
            return ((lastkeys & KEY_UP) || (sslastkeys & SS_KEY_UP));
            break;
        case KI_DOWN:
            return ((lastkeys & KEY_DOWN) || (sslastkeys & SS_KEY_DOWN));
            break;
        case KI_LEFT:
            return ((lastkeys & KEY_LEFT) || (sslastkeys & SS_KEY_LEFT));
            break;
        case KI_RIGHT:
            return ((lastkeys & KEY_RIGHT) || (sslastkeys & SS_KEY_RIGHT));
            break;
        case KI_PREV:
            return (sslastkeys & SS_KEY_PREV);
            break;
        case KI_NEXT:
            return (sslastkeys & SS_KEY_NEXT);
            break;
        case KI_PLOT:
            return (sslastkeys & SS_KEY_PLOT);
            break;
        case KI_IDENT:
            return (sslastkeys & SS_KEY_IDENT);
            break;
        default:
            return false;
    }
}



static const short keymap[] = {KI_ENTER, KI_BACK, KI_BEAM, KI_MENU, KI_RIGHT, KI_LEFT, KI_UP, KI_DOWN, KI_FAST, KI_SLOW, 0, 0, 0, 0, 0, 0};
static const short sskeymap[] = {KI_PREV, KI_RIGHT, KI_NEXT, KI_UP, KI_ENTER, KI_DOWN, 0, KI_LEFT, KI_SLOW, KI_PLOT, KI_IDENT, KI_BACK, KI_FAST, KI_BEAM, KI_MENU};

void CKeysIn::Poll()
{
    short keys = ~(GBA_REG_KEYS);
    short sskeys = SSKeys();
    short keysdown = (keys & ~lastkeys);
    short sskeysdown = (sskeys & ~sslastkeys);
    lastkeys = keys;
    sslastkeys = sskeys;

    short i, bitval;

    if (keysdown) {
        i = 10;
        bitval = 1 << i;
        while (bitval) {
            if ((keysdown & bitval)) {
                if (keymap[i]) {
                    keybuf.Put(keymap[i]);
                }
            }
            bitval >>= 1;
            i--;
        }
    }

    if (sskeysdown) {
        i = 14;
        bitval = 1 << i;
        while (bitval) {
            if ((sskeysdown & bitval)) {
                if (sskeymap[i]) {
                    keybuf.Put(sskeymap[i]);
                }
            }
            bitval >>= 1;
            i--;
        }
    }
}


void CKeysIn::PollKeys(void *ctx)
{
    return keyin->Poll();
}




short GetKeyCount()
{
    return keyin->GetKeyCount();
}


short GetKey()
{
    return keyin->GetKey();
}


bool IsKeyDown(short key)
{
    return keyin->IsKeyDown(key);
}



