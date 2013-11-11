#include <stdlib.h>
#include "fbinterrupt.h"
#include "fbtimer.h"


FBTimerEvent::FBTimerEvent(long secs, unsigned short millis) {
    event_time_secs = secs;
    event_time_millis = millis;
    event_repeat_secs = 0;
    event_repeat_millis = 0;
}

FBTimerEvent::FBTimerEvent(long secs, unsigned short millis, long repeat_secs, unsigned short repeat_millis) {
    event_time_secs = secs;
    event_time_millis = millis;
    event_repeat_secs = repeat_secs;
    event_repeat_millis = repeat_millis;
}

void FBTimerEvent::GetNextScheduledTime(long* secs, unsigned short* millis) const {
    *secs = event_time_secs;
    *millis = event_time_millis;
}

void FBTimerEvent::SetNextScheduledTime(long secs, unsigned short millis) {
    event_time_secs = secs;
    event_time_millis = millis;
}

void FBTimerEvent::GetRepeatPeriod(long* secs, unsigned short* millis) const {
    *secs = event_repeat_secs;
    *millis = event_repeat_millis;
}

void FBTimerEvent::SetRepeatPeriod(long secs, unsigned short millis) {
    event_repeat_secs = secs;
    event_repeat_millis = millis;
}

bool FBTimerEvent::UpdateForRepeat() {
    if (!event_repeat_secs && ! event_repeat_millis) {
        return false;
    }
    if (!event_time_secs && ! event_time_millis) {
        event_time_secs = time();
    }
    event_time_secs += event_repeat_secs;
    event_time_millis += event_repeat_millis;
    if (event_time_millis >= 1024) {
        event_time_millis -= 1024;
        event_time_secs++;
    }
    return true;
}

bool FBTimerEvent::operator<(const FBTimerEvent& x) const {
    int cmp = this->event_time_secs - x.event_time_secs;
    if (cmp) {
        return (cmp < 0);
    }
    return (this->event_time_millis < x.event_time_millis);
}



void FBTimerIrqHandler(void* ctx);

FBTimer* FBTimer::main_clock = 0;

FBTimer::FBTimer()
{
    const unsigned long gba_speed_hz = 16777216;
    const unsigned long cycles_per_milli = gba_speed_hz / FBTIMER_POLL_FREQ;
    const unsigned short timer_int = (INT_TIMER0 << FBTIMER_NUM);

    unsigned short timer_count;
    unsigned short timer_freq;
    if (cycles_per_milli < 65536) {
        timer_count = (0x10000 - cycles_per_milli) & 0xffff;
        timer_freq = TIMER_FREQ_SYS;
    } else {
        timer_count = (0x10000 - (cycles_per_milli >> 6)) & 0xffff;
        timer_freq = TIMER_FREQ_64;
    }

    this->main_clock = this;
    this->seconds = 0;
    this->millis = 0;

    TIMER_DATA(FBTIMER_NUM) = timer_count;
    TIMER_CNTL(FBTIMER_NUM) = (TIMER_ENABLE | timer_freq | TIMER_IRQ_EN);

    FBRegisterInterrupt(timer_int, FBTimerIrqHandler, NULL);
    FBEnableInterrupts(timer_int);
}


void
FBTimer::SetTime(const CDateTime &dt)
{
    seconds = dt.dt_systime;
}


void
FBTimer::SetTime(long new_secs, unsigned short new_millis)
{
    seconds = new_secs;
    millis = new_millis;
}

void
FBTimer::GetDateTime(CDateTime& dt) const
{
    dt.Set(seconds);
}

void
FBTimer::GetTime(long* secs, unsigned short* mils) const
{
    *secs = seconds;
    *mils = millis;
}

long
FBTimer::GetTimeSeconds() const
{
    return seconds;
}

float
FBTimer::GetTimeDouble() const
{
    return (float)(seconds + (millis / 1024.0));
}

void
FBTimer::WaitUntilTime(long targ_secs, unsigned short targ_millis)
{
    volatile long* secptr = &seconds;
    volatile unsigned short* milptr = &millis;
    while (*secptr < targ_secs);
    while (*milptr < targ_millis);
}

void
FBTimer::WaitMillis(long secs, unsigned short mils)
{
    unsigned long targ_millis = millis + mils;
    unsigned long targ_secs = seconds + secs;
    while (targ_millis >= 1024) {
        targ_millis -= 1024;
        targ_secs++;
    }
    WaitUntilTime(targ_secs, targ_millis);
}


void
FBTimer::ScheduleEvent(FBTimerEvent* event)
{
    for (int i = 0; i < MAX_FBTIMER_EVENTS; i++) {
        if (!eventlist[i]) {
            eventlist[i] = event;
            break;
        }
    }
}


void
FBTimer::ProcessEvents()
{
    for (int i = 0; i < MAX_FBTIMER_EVENTS; i++) {
        FBTimerEvent* event = eventlist[i];
        if (event) {
            if (event->IsAtOrBefore(this->seconds, this->millis)) {
                event->Run(this);
                if (!event->UpdateForRepeat()) {
                    delete event;
                    eventlist[i] = NULL;
                }
            }
        }
    }
}


void
FBTimerIrqHandler(void* ctx)
{
    const short millis_per_poll = 1024 / FBTIMER_POLL_FREQ;
    FBTimer* fbt = FBTimer::main_clock;
    fbt->millis += millis_per_poll;
    if (fbt->millis >= 1024) {
        fbt->millis -= 1024;
        fbt->seconds++;
    }
    fbt->ProcessEvents();
}


void set_time(long secs, unsigned short mils)
{
    FBTimer::main_clock->SetTime(secs, mils);
}


long time()
{
    return FBTimer::main_clock->GetTimeSeconds();
}


unsigned short time_millis()
{
    long secs;
    unsigned short mils;
    FBTimer::main_clock->GetTime(&secs, &mils);
    return mils;
}


void sleep(long secs)
{
    FBTimer::main_clock->WaitMillis(secs, 0);
}


void millisleep(unsigned long millis)
{
    long secs = millis >> 10;
    unsigned short mils = millis & 1023;
    FBTimer::main_clock->WaitMillis(secs, mils);
}


