#ifndef CONSTLINES_H
#define CONSTLINES_H

struct ConstellationLine {
    char name[4];
    float ra1;
    float dec1;
    float ra2;
    float dec2;
};

extern const struct ConstellationLine ConstellationLineList[];
int GetConstellationLineCount();

#endif

