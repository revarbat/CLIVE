#ifndef SIGNALS_H
#define SIGNALS_H

#include <signal.h>

void setSignalHandler(int signo, void (*sighandler) (int));

#endif

