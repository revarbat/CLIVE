#ifndef SKYPLOT_H
#define SKYPLOT_H

void PlotSky(long when, const char* constel, float laser_alt, float laser_az, float poi_alt, float poi_az);

void InitSkyCaches();
void UpdateSkyCaches();

#endif


