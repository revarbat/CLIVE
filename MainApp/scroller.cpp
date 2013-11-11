#include <string.h>
#include "scroller.h"
#include "datetime.h"
#include "uart.h"
#include "fbtimer.h"
#include "clockutil.h"
#include "scrollimgs.h"

CScroller scroller;

extern void int_to_str(long val, bool zeropad, int chars, char* buf, int buflen);

void CScroller::Init(short id)
{
}


void CScroller::SendIcon(short icon) {
}



void CScroller::SendIdCode(short id)
{
}


void CScroller::RunPage(short id, short page)
{
}


void CScroller::PageStart(short id, short page)
{
    txtSetPosition(0,20);
}

void CScroller::PageAppend(const char* msg)
{
    txtPrint(msg);
}


void CScroller::PageDone()
{
}


void CScroller::SetColor(short color)
{
    char buf[128] = "\033[1xc";
    buf[3] += color;
    txtPrint(buf);
}


void CScroller::SetFont(short font)
{
}


void CScroller::SetFontSize(short size)
{
}


void CScroller::Transition(short func)
{
}


bool CScroller::WasSentBefore(short id, short page, short color, short font, short fontsize, const char* msg)
{
    if (id == last_id &&
        page == last_page &&
        color == last_color &&
        font == last_font &&
        fontsize == last_fontsize &&
        !strcmp(msg, last_msg))
    {
        return true;
    }
    last_id = id;
    last_page = page;
    last_color = color;
    last_font = font;
    last_fontsize = fontsize;
    strcpy(last_msg, msg);
    return false;
}


void CScroller::PageWrite(short id, short page, short color, const char* msg)
{
    if (WasSentBefore(id, page, color, SC_FONT_NORMAL, SC_FONTSIZE_4x7, msg)) {
        return;
    }
    PageStart(id, page);
    Transition(SC_FUNC_APPEAR);
    SetColor(color);
    SetFont(SC_FONT_NORMAL);
    SetFontSize(SC_FONTSIZE_4x7);
    scroller_uart.WriteString(msg);
    Transition(SC_FUNC_PAUSE);
    PageDone();
}


void CScroller::Target(short id, short page, const char* name)
{
    short color;
    if (curr_day_part == NIGHT) {
        color = SC_COLOR_DIM_GREEN;
    } else if (curr_day_part == DAY) {
        color = SC_COLOR_BRIGHT_GREEN;
    } else {
        color = SC_COLOR_GREEN;
    }

    if (WasSentBefore(id, page, color, SC_FONT_NORMAL, SC_FONTSIZE_4x7, name)) {
        return;
    }

    PageStart(id, page);
    Transition(SC_FUNC_APPEAR);
    SetColor(color);
    SetFont(SC_FONT_NORMAL);
    SetFontSize(SC_FONTSIZE_4x7);
    PageAppend(name);
    Transition(SC_FUNC_PAUSE);
    Transition(SC_FUNC_PAUSE);
    PageDone();
}


void CScroller::Target(short id, short page, short icon, const char* name)
{
    short color;
    if (curr_day_part == NIGHT) {
        color = SC_COLOR_DIM_GREEN;
    } else if (curr_day_part == DAY) {
        color = SC_COLOR_BRIGHT_GREEN;
    } else {
        color = SC_COLOR_GREEN;
    }

    if (WasSentBefore(id, page, color, SC_FONT_NORMAL, SC_FONTSIZE_4x7, name)) {
        return;
    }

    PageStart(id, page);
    Transition(SC_FUNC_APPEAR);
    SetColor(color);
    SetFont(SC_FONT_NORMAL);
    SetFontSize(SC_FONTSIZE_4x7);
    SendIcon(icon);
    PageAppend(name);
    Transition(SC_FUNC_PAUSE);
    Transition(SC_FUNC_PAUSE);
    PageDone();
}


void CScroller::Warning(short id, short page, const char* msg)
{
    short color;
    if (curr_day_part == NIGHT) {
        color = SC_COLOR_DIM_RED;
    } else if (curr_day_part == DAY) {
        color = SC_COLOR_BRIGHT_RED;
    } else {
        color = SC_COLOR_RED;
    }

    if (WasSentBefore(id, page, color, SC_FONT_FLASH, SC_FONTSIZE_4x7, msg)) {
        return;
    }

    PageStart(id, page);
    Transition(SC_FUNC_APPEAR);
    SetColor(color);
    SetFont(SC_FONT_FLASH);
    SetFontSize(SC_FONTSIZE_4x7);
    PageAppend(msg);
    Transition(SC_FUNC_PAUSE);
    Transition(SC_FUNC_PAUSE);
    PageDone();
}



