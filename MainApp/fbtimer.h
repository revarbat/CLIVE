#ifndef FBTIMER_H
#define FBTIMER_H

#include "timer.h"
#include "fbinterrupt.h"
#include "clockutil.h"

// Clock uses Timer3.
#define FBTIMER_NUM 2
#define MAX_FBTIMER_EVENTS 8
//#define FBTIMER_POLL_FREQ 1024
#define FBTIMER_POLL_FREQ 128  /* polls per sec */

class FBTimer;

class FBTimerEvent {
public:
    FBTimerEvent(long secs, unsigned short millis);
    FBTimerEvent(long secs, unsigned short millis, long repeat_secs, unsigned short repeat_millis);
    virtual ~FBTimerEvent() {}

    virtual void Run(FBTimer* timer) = 0;

    inline bool IsAtOrBefore(long secs, unsigned short millis) const
    {
        if (event_time_secs < secs) {
            return true;
        } else if (event_time_secs > secs) {
            return false;
        } else {
            return event_time_millis <= millis;
        }
    }

    void GetNextScheduledTime(long* secs, unsigned short* millis) const;
    void SetNextScheduledTime(long secs, unsigned short millis);
    void GetRepeatPeriod(long* secs, unsigned short* millis) const;
    void SetRepeatPeriod(long secs, unsigned short millis);
    bool UpdateForRepeat();
    bool operator<(const FBTimerEvent& x) const;

protected:
    long event_time_secs;
    unsigned short event_time_millis;
    long event_repeat_secs;
    unsigned short event_repeat_millis;
};


// Clock is based off of millis, or 1/1024ths of a second, for efficiency.
class FBTimer {
private:
    long seconds;
    unsigned short millis;
    friend void FBTimerIrqHandler(void* ctx);
    FBTimerEvent* eventlist[MAX_FBTIMER_EVENTS];

public:
    static FBTimer* main_clock;
    FBTimer();

    void SetTime(const CDateTime &dt);
    void SetTime(long secs, unsigned short mils);
    void GetTime(long* secs, unsigned short* mils) const;
    void GetDateTime(CDateTime& dt) const;
    long GetTimeSeconds() const;
    float GetTimeDouble() const;
    void WaitUntilTime(long secs, unsigned short mils);
    void WaitMillis(long secs, unsigned short mils);
    void ScheduleEvent(FBTimerEvent* event);
    void ProcessEvents();
};


void FBTimerIrqHandler();

void set_time(long secs, unsigned short mils);
long time();
unsigned short time_millis();
void sleep(long secs);
void millisleep(unsigned long millis);

#endif

