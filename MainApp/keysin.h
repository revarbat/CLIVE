#ifndef KEYSIN_H
#define KEYSIN_H

#include "fbtimer.h"
#include "ringbuf.h"

#define KI_UP     1
#define KI_DOWN   2
#define KI_LEFT   3
#define KI_RIGHT  4
#define KI_ENTER  5
#define KI_BACK   6
#define KI_PLOT   7
#define KI_BEAM   8
#define KI_FAST   9
#define KI_MENU  10
#define KI_PREV  11
#define KI_NEXT  12
#define KI_IDENT 13
#define KI_SLOW  14

/*
5 x 3 keypad:

     R5    R4    R3    R2    R1

    Fast  Plot  Slow   Up   Prev   C1
    Beam  Idnt  Left  Entr  Rght   C2
    Menu  Back   --   Down  Next   C3


          Pins on keypad (in order)
    Rows: 5, 7, 8, 2, 1
    Cols: 3, 4, 6

    To Keypad (in order, from pin 1 (with arrow))
      Ylw Brn Blu Wht Org Blk Grn Red

    Cat5 header (tab down, pins away from you, left to right)
      Blu Org Blk Red Grn Ylw Brn Wht

4 x 4 keypad:
     --    Up   Prev  Menu
    Left  Entr  Rght  Back
    Slow  Down  Next  Beam
    Fast   --    --   Plot

Gameboy:
    A      = Enter
    B      = Back
    L      = Plot
    R      = Fast
    Select = Beam
    Start  = Menu
*/

class CKeysIn {
public:
    CKeysIn();
    ~CKeysIn();

    void Flush();
    bool IsKeyDown(short key);
    short GetKeyCount();
    short GetKey();

    void Poll();

private:
    static void PollKeys(void* ctx);

    short lastkeys;
    short sslastkeys;
    CRingBuf<short,16>keybuf;
};

extern CKeysIn* keyin;
short GetKeyCount();
short GetKey();
bool IsKeyDown(short key);

#endif

