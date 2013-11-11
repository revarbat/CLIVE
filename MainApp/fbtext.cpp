#include "fbtext.h"

#include <stdlib.h>
#include <stdio.h>

CTextDisp::CTextDisp()
{
}


CTextDisp::~CTextDisp()
{
}


void CTextDisp::Clear()
{
    printf("\033[H\033[2J");
}


void CTextDisp::AttrReset()
{
    printf("\033[0m");
}


void CTextDisp::AttrBold()
{
    printf("\033[1m");
}


void CTextDisp::AttrDim()
{
    printf("\033[2m");
}


void CTextDisp::AttrUnderline()
{
    printf("\033[4m");
}


void CTextDisp::AttrBlink()
{
    printf("\033[5m");
}


void CTextDisp::AttrInverse()
{
    printf("\033[7m");
}


void CTextDisp::SetPosition(short x, short y)
{
    printf("\033[%d;%df", y, x);
}


void CTextDisp::Print(const char* str)
{
    printf("%s", str);
}


void CTextDisp::Printf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
}


CTextDisp *textdisp = new CTextDisp();

void txtReInit()
{
    delete textdisp;
    textdisp = new CTextDisp();
    textdisp->Clear();
}


