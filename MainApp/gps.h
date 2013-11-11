#ifndef GPS_H
#define GPS_H

#include "datetime.h"

extern long last_gps_sync_time;
extern float my_latitude;
extern float my_longitude;

extern long good_uart_data;
extern long bad_uart_data;
extern char last_gps_data[];

void GetGpsInfoFromSerial();
void UnparseGpsString(const char* in, int len);

#endif

