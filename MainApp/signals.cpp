#include "signals.h"


void
setSignalHandler(int signo, void (*sighandler) (int))
{
        struct sigaction act, oact;

        act.sa_handler = sighandler;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;

        /* Restart long system calls if a signal is caught. */
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;
#endif

        /* Make it so */
        sigaction(signo, &act, &oact);
}



void timerSignalHandler(int sig)
{
}


