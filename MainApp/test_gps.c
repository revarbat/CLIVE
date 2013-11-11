#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

float my_latitude = 0;
float my_longitude = 0;
int initial_position_read = 0;

long good_uart_data = 0;
long bad_uart_data = 0;
char last_gps_data[128];


int ChecksumNmeaLine(const char* in)
{
    const char hexdigits[] = "0123456789ABCDEF";

    if (*in++ != '$') {
        return 0;
    }
    int val = 0;
    while (*in != '*' && *in != '\r') {
        val ^= *in++;
    }
    if (*in++ == '*') {
        if (*in++ != hexdigits[((val>>4) & 0xf)]) {
            return 0;
        }
        if (*in++ != hexdigits[(val & 0xf)]) {
            return 0;
        }
        return 1;
    }
    return 0;
}


void UnparseGpsString(int fd, const char* in, int len)
{
    if (in[0] == '@') {
        // Garmin TEXT data line.

        if (len != 56) {
            return;  // Wrong length means bad data.
            bad_uart_data++;
        }

        short year   = 2000 + (in[1] - '0') * 10 +  (in[2] - '0');
        short month  =        (in[3] - '0') * 10 +  (in[4] - '0');
        short day    =        (in[5] - '0') * 10 +  (in[6] - '0');
        short hour   =        (in[7] - '0') * 10 +  (in[8] - '0');
        short minute =        (in[9] - '0') * 10 + (in[10] - '0');
        short second =       (in[11] - '0') * 10 + (in[12] - '0');
	fprintf(stdout, "%04d-%02d-%02d %02d:%02d:%02d GMT   ", year, month, day, hour, minute, second);
	fprintf(stdout, "Lat: %11.6f   Lon: %11.6f\n", my_latitude, my_longitude);
        good_uart_data++;

        if (in[13] == 'N' || in[13] == 'S') {
            float lat =
                ((in[14] - '0') * 10.0) +
                 (in[15] - '0');

            float latmins =
                ((in[16] - '0') * 10.0) +
                 (in[17] - '0') +
                ((in[18] - '0') / 10.0) +
                ((in[19] - '0') / 100.0) +
                ((in[20] - '0') / 1000.0);

            if (in[13] == 'S') {
                my_latitude = -lat - (latmins / 60.0);
            } else {
                my_latitude = lat + (latmins / 60.0);
            }

        }
        if (in[21] == 'E' || in[21] == 'W') {
            float lon =
                ((in[22] - '0') * 100.0) +
                ((in[23] - '0') * 10.0) +
                 (in[24] - '0');

            float lonmins =
                ((in[25] - '0') * 10.0) +
                 (in[26] - '0') +
                ((in[27] - '0') / 10.0) +
                ((in[28] - '0') / 100.0) +
                ((in[21] - '0') / 1000.0);

            if (in[21] == 'W') {
                my_longitude = -lon - (lonmins / 60.0);
            } else {
                my_longitude = lon + (lonmins / 60.0);
            }
        }
    } else if (in[0] == '$') {
        // NMEA data line.

        if (!ChecksumNmeaLine(in)) {
            bad_uart_data++;
            return;
        }

        if (!strncmp(in, "$GPRMC", 6)) {

            //$GPRMC,time,  stat,lat,  NS,long     ,EW,speed,cours,date, magv, EW,mode
            //$GPRMC,hhmmss,A,ddmm.mmmm,N,dddmm.mmmm,W,nnn.n,nnn.n,ddmmyy,nnn.n,W,A*hh
            //$GPRMC,071458,A,3714.1938,N,12153.8619,W,0.0,241.0,070804,14.9,E,A*36

            char* timep = strchr(in+6,    ',');
            char* statp = strchr(timep+1, ',');
            char* latp  = strchr(statp+1, ',');
            char* latdp = strchr(latp+1,  ',');
            char* lonp  = strchr(latdp+1, ',');
            char* londp = strchr(lonp+1,  ',');
            char* spdp  = strchr(londp+1, ',');
            char* crsp  = strchr(spdp+1,  ',');
            char* datep = strchr(crsp+1,  ',');

            good_uart_data++;
            if (statp[1] == 'A' || statp[1] == 'V') {
                if (latdp[1] == 'N' || latdp[1] == 'S') {
                    char* decp  = strchr(latp+1, '.');

                    float lat;
                    if (decp - latp == 5) {
                        lat = ((latp[1] - '0') * 10.0) + (latp[2] - '0');
                        latp++;
                    } else {
                        lat = (latp[1] - '0');
                    }

                    float latmins =
                        ((latp[2] - '0') * 10.0) +
                         (latp[3] - '0') +
                        ((latp[5] - '0') / 10.0);

                    if (isdigit(latp[6])) {
                        latmins += ((latp[6] - '0') / 100.0);
                        if (isdigit(latp[7])) {
                            latmins += ((latp[7] - '0') / 1000.0);
                            if (isdigit(latp[8])) {
                                latmins += ((latp[8] - '0') / 10000.0);
                            }
                        }
                    }

                    if (latdp[1] == 'S') {
                        my_latitude = -lat - (latmins / 60.0);
                    } else {
                        my_latitude = lat + (latmins / 60.0);
                    }

                }
                if (londp[1] == 'E' || londp[1] == 'W') {
                    char* decp  = strchr(lonp+1, '.');

                    float lon = 0;
                    if (decp - lonp == 6) {
                        lon = lonp[1] - '0';
                        lonp++;
                    }
                    if (decp - lonp == 5) {
                        lon = (lon * 10) + (lonp[1] - '0');
                        lonp++;
                    }
                    if (decp - lonp == 4) {
                        lon = (lon * 10) + (lonp[1] - '0');
                        lonp++;
                    }

                    float lonmins =
                        ((lonp[1] - '0') * 10.0) +
                         (lonp[2] - '0') +
                        ((lonp[4] - '0') / 10.0);

                    if (isdigit(lonp[6])) {
                        lonmins += ((lonp[6] - '0') / 100.0);
                        if (isdigit(lonp[7])) {
                            lonmins += ((lonp[7] - '0') / 1000.0);
                            if (isdigit(lonp[8])) {
                                lonmins += ((lonp[8] - '0') / 10000.0);
                            }
                        }
                    }

                    if (londp[1] == 'W') {
                        my_longitude = -lon - (lonmins / 60.0);
                    } else {
                        my_longitude = lon + (lonmins / 60.0);
                    }
                }

                short year   = 2000 + (datep[5] - '0') * 10 +  (datep[6] - '0');
                short month  =        (datep[3] - '0') * 10 +  (datep[4] - '0');
                short day    =        (datep[1] - '0') * 10 +  (datep[2] - '0');
                short hour   =        (timep[1] - '0') * 10 +  (timep[2] - '0');
                short minute =        (timep[3] - '0') * 10 +  (timep[4] - '0');
                short second =        (timep[5] - '0') * 10 +  (timep[6] - '0');
		fprintf(stdout, "%04d-%02d-%02d %02d:%02d:%02d GMT   ", year, month, day, hour, minute, second);
		fprintf(stdout, "Lat: %11.6f   Lon: %11.6f\n", my_latitude, my_longitude);
            }
        } else {
            // If we get an unhandled NMEA line, ask Garmins
            // to only send us $GPRMC sentences.
            //SendNmeaLine("$PGRMO,,2");
            //SendNmeaLine("$PGRMO,GPRMC,1");
            good_uart_data++;
        }
    }
    if (!initial_position_read) {
        if (my_latitude != 0 && my_longitude != 0) {
            initial_position_read = 1;
        }
    }
}


void
GetGpsInfoFromSerial(int fd)
{
    char *in = last_gps_data;
    while (in < last_gps_data+sizeof(last_gps_data)) {
        if (!read(fd,in,1)) {
	    return;
	}
	if (*in == '\n') {
	    *in = '\0';
	    UnparseGpsString(fd, last_gps_data, sizeof(last_gps_data));
	    return;
	}
	in++;
    }
}


int main()
{
    int fd;
    struct termios options;

    fd = open("/dev/tty.usbserial-A602HTRY", O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd == -1) {
	perror("open_port: Unable to open:");
    } else {
	fcntl(fd, F_SETFL, 0);
	printf("Connected...\n");
    }

    tcgetattr(fd, &options);
    cfsetispeed(&options, B4800);
    cfsetospeed(&options, B4800);
    tcsetattr(fd, TCSANOW, &options);

    for (;;) {
	GetGpsInfoFromSerial(fd);
    }
}


