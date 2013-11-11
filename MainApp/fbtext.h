#ifndef FBTEXT_H
#define FBTEXT_H

#include <stdarg.h>


class CTextDisp {
public:
    CTextDisp(short option);
    ~CTextDisp();

    void Clear();
    void AttrReset();
    void AttrBold();
    void AttrDim();
    void AttrUnderline();
    void AttrBlink();
    void AttrInverse();
    void SetPosition(short x, short y);
    void Print(const char* str);
    void Printf(const char* fmt, ...);
};

extern CTextDisp *textdisp;
void txtReInit();

#define txtPrint(x)    textdisp->Print((const char *)x)
#define txtPrintf      textdisp->Printf
#define txtClear       textdisp->Clear
#define txtSetPosition textdisp->SetPosition
#define txtAttrReset   textdisp->AttrReset
#define txtAttrBold    textdisp->AttrBold
#define txtAttrULine   textdisp->AttrUnderline
#define txtAttrFlash   textdisp->AttrFlash
#define txtAttrInverse textdisp->AttrInverse

#endif

