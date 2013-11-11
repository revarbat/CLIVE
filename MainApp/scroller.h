#ifndef SCROLLER_H
#define SCROLLER_H

#define SC_ICON_SATELLITE 0x0
#define SC_ICON_PLANET    0x1
#define SC_ICON_STAR      0x2

#define SC_FONT_NORMAL 0x0
#define SC_FONT_WIDE   0x1
#define SC_FONT_ITALIC 0x2
#define SC_FONT_FLASH  0x4

#define SC_FONTSIZE_5x7  0x0
#define SC_FONTSIZE_4x7  0x1

#define SC_COLOR_DIM_RED          1
#define SC_COLOR_RED              1
#define SC_COLOR_BRIGHT_RED       1
#define SC_COLOR_ORANGE           3
#define SC_COLOR_BRIGHT_ORANGE    3
#define SC_COLOR_LIGHT_YELLOW     3
#define SC_COLOR_YELLOW           3
#define SC_COLOR_BRIGHT_YELLOW    3
#define SC_COLOR_LIME             2
#define SC_COLOR_DIM_LIME         2
#define SC_COLOR_BRIGHT_LIME      2
#define SC_COLOR_BRIGHT_GREEN     6
#define SC_COLOR_GREEN            6
#define SC_COLOR_DIM_GREEN        6
#define SC_COLOR_YEL_GRN_RED      4
#define SC_COLOR_RAINBOW          4
#define SC_COLOR_RED_GRN_3D       4
#define SC_COLOR_RED_YEL_3D       4
#define SC_COLOR_GRN_RED_3D       4
#define SC_COLOR_GRN_YEL_3D       4
#define SC_COLOR_GREEN_ON_RED     2
#define SC_COLOR_RED_ON_GREEN     1
#define SC_COLOR_ORG_ON_GRN_3D    3
#define SC_COLOR_LIME_ON_RED_3D   2
#define SC_COLOR_GRN_ON_RED_3D    6
#define SC_COLOR_RED_ON_GRN_3D    1

#define SC_FUNC_AUTO              0
#define SC_FUNC_OPEN              1
#define SC_FUNC_COVER             2
#define SC_FUNC_DATE              3
#define SC_FUNC_CYCLING           4
#define SC_FUNC_CLOSE_RTOL        5
#define SC_FUNC_CLOSE_LTOR        6
#define SC_FUNC_CLOSE_TO_CENTER   7
#define SC_FUNC_SCROLL_UP         8
#define SC_FUNC_SCROLL_DOWN       9
#define SC_FUNC_OVERLAP          10
#define SC_FUNC_STACKING         11
#define SC_FUNC_COMIC_PACMAN     12
#define SC_FUNC_COMIC_CREATURE   13
#define SC_FUNC_BEEP             14
#define SC_FUNC_PAUSE            15
#define SC_FUNC_APPEAR           16
#define SC_FUNC_RANDOM           17
#define SC_FUNC_SHIFT_RTOL       18
#define SC_FUNC_TIME_DATE        19
#define SC_FUNC_MAGIC            20
#define SC_FUNC_THANKYOU         21
#define SC_FUNC_WELCOME          22
#define SC_FUNC_SPEED1           23
#define SC_FUNC_SPEED2           24
#define SC_FUNC_SPEED3           25

class CScroller {
public:
    void Init(short id);
    void SendIcon(short graphic);
    void SendIdCode(short id);
    void RunPage(short id, short page);
    void PageStart(short id, short page);
    void PageAppend(const char* msg);
    void PageDone();
    void SetColor(short color);
    void SetFont(short color);
    void SetFontSize(short size);
    void Transition(short color);
    void PageWrite(short id, short page, short color, const char* msg);
    void Target(short id, short page, const char* name);
    void Target(short id, short page, short icon, const char* name);
    void Warning(short id, short page, const char* name);

private:
    bool WasSentBefore(short id, short page, short color, short font, short fontsize, const char* msg);

    short last_id;
    short last_page;
    short last_color;
    short last_font;
    short last_fontsize;
    char  last_msg[128];

};

extern CScroller scroller;

#endif

