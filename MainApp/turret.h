#ifndef STEPPER_H
#define STEPPER_H

#include "uart.h"

class CTurret {
public:
    CTurret(CUart* uart);

    void SetRotationLimits(short axis, long minpos, long maxpos);
    void SetBacklash(short axis, long val, unsigned short speed);
    void SetZeroPos(short axis);

    long GetCurrPos(short axis);
    long GetStepsLeft(short axis);

    unsigned short GetSpeed(short axis);
    void SetSpeed(short axis, unsigned short speed);

    bool GetActiveHolding(short axis);
    void SetActiveHolding(short axis, bool onval);

    bool GetLimitSwitch(short axis);
    void WatchLimitSwitch(short axis, short level, short enable);

    void RotateSteps(short axis, long steps, unsigned short speed);
    void RotateToPos(short axis, long pos, unsigned short speed);
    void Slew(short axis, short dir);
    void Stop(short axis);
    void WaitUntilStopped(short axis);

    void RecalibrateHome(unsigned short speed);
    friend void TurretPoll();

private:
    void WriteAxis(short axis);
    void WriteCommand(long val, char cmd);
    void WriteCommand(const char* cmd);
    void WriteCommand(short axis, long val, char cmd);
    void WriteCommand(short axis, const char* cmd);
    void SendCommand();

    CUart* uart;
    CRingBuf<char,80> command_buf;
    long stepper_last_cmd_time;
    short stepper_outstanding_cmds;
    short previous_axis;

    short limit_switches;

    long target_step[2];
    long zero_position[2];
    long position_minimum[2];
    long position_maximum[2];
    long backlash[2];
    bool backlash_engaged[2];
    long stepper_pos[2];
    long stepper_speed[2];
    long stepper_previous_speed[2];
    bool active_holding[2];

    short turret_limits_control_val;
    short turret_mutex;
    CTurret* next;
};

extern CTurret* cturret_list;

void TurretPoll();

#endif

