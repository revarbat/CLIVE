#ifndef GRAPHICS_H
#define GRAPHICS_H

typedef unsigned short color_t;

#define RGB(red,green,blue) (((blue & 0x1f) << 10) | ((green & 0x1f) << 5) | (red & 0x1f))

void GrInit();
void GrRelease();
void GrClearScreen();
void GrPlotPixel(int x,int y, color_t c);
void GrPlotLine(short x1, short y1, short x2, short y2, color_t color);

#endif

