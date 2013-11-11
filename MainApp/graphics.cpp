#include <stdlib.h>
#include "fbtext.h"
#include "graphics.h"




void GrInit()
{
    //dispcnt_store = GBA_REG_DISPCNT;
    //GBA_REG_DISPCNT = (GBA_BG2_ENABLE << 8) | GBA_BG_MODE_3;
}

void GrRelease()
{
    //GBA_REG_DISPCNT = dispcnt_store;
    txtReInit();
}


void GrClearScreen()
{
    //color_t* videoBase = GBA_BASE_VRAM;
    //for (int i = 0; i < 240*160; i++) {
    //    videoBase[i] = 0;
    //}
}


void GrPlotPixel(int x,int y, color_t c)
{
    //if (x >= 0 && x < 240 && y >= 0 && y < 160) {
    //    color_t* videoBase = GBA_BASE_VRAM;
    //    videoBase[y*240 + x] = c;
    //}
}


void GrPlotLine(short x1, short y1, short x2, short y2, color_t color)
{
    //color_t* VideoBase = GBA_BASE_VRAM;

    //int pos = y1 * 240 + x1; 
    //int i; 
    //int lambda = 0;
    //int stepx = 1;
    //int stepy = 240;

    //int dx = x2 - x1;
    //int dy = y2 - y1;

    //if (dx < 0) {
    //    dx = -dx;
    //    stepx = -stepx;
    //}
    //if (dy < 0) {
    //    dy = -dy;
    //    stepy = -stepy;
    //}

    //if (dx > dy) {
    //    for (i = 0; i <= dx; i++) {
    //        VideoBase[pos] = color;
    //        pos += stepx;
    //        lambda += dy;
    //        if (lambda > dx) {
    //            lambda -= dx;
    //            pos += stepy;
    //        }
    //    }
    //} else {
    //    for (i = 0; i <= dy; i++) { 
    //        VideoBase[pos] = color;
    //        pos += stepy;
    //        lambda += dx;
    //        if (lambda > dy) {
    //            lambda -= dy;
    //            pos += stepx; 
    //        }
    //    }
    //}
}


