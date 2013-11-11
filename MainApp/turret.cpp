#include <stdlib.h>
#include <string.h>
#include "fbtimer.h"
#include "gpio.h"
#include "turret.h"
#include "uart.h"
#include "ringbuf.h"


#define MAX_SPEED 6400

CTurret* cturret_list = NULL;

CTurret::CTurret(CUart* uart)
{
    this->uart = uart;

    stepper_last_cmd_time = 0;
    stepper_outstanding_cmds = 0;

    previous_axis = 2;
    limit_switches = 15;
    turret_limits_control_val = 0;
    turret_mutex = 0;

    for (short axis = 0; axis < 2; axis++) {
	Stop(axis);
	SetRotationLimits(axis, -16384, 16384);
	backlash[axis] = 0;
	backlash_engaged[axis] = false;
	zero_position[axis] = 0;
	target_step[axis] = 0;
	stepper_pos[axis] = 0;
	stepper_speed[axis] = 0;
	stepper_previous_speed[axis] = 0;
	active_holding[axis] = false;
	zero_position[axis] = 0;
    }

    this->next = cturret_list;
    cturret_list = this;
}


void CTurret::SetRotationLimits(short axis, long minpos, long maxpos)
{
    position_minimum[axis] = minpos;
    position_maximum[axis] = maxpos;
}



void CTurret::SetBacklash(short axis, long val, unsigned short speed)
{
    if (val != backlash[axis]) {
        // Make sure we start out with backlash disengaged fully.
        if (backlash_engaged[axis]) {
            long maxbacklash = backlash[axis];
            if (val > maxbacklash) {
                maxbacklash = val;
            }
            RotateSteps(axis, maxbacklash, speed);
            WaitUntilStopped(axis);
        }
        backlash[axis] = val;
    }
}



void CTurret::SetZeroPos(short axis)
{
    zero_position[axis] = stepper_pos[axis];
    if (backlash_engaged[axis]) {
        zero_position[axis] += backlash[axis];
    }
    target_step[axis] = 0;
}


long CTurret::GetCurrPos(short axis)
{
    long pos = stepper_pos[axis];
    if (backlash_engaged[axis]) {
        pos += backlash[axis];
    }
    return (pos - zero_position[axis]);
}


long CTurret::GetStepsLeft(short axis)
{
    return (target_step[axis] - GetCurrPos(axis));
}


unsigned short CTurret::GetSpeed(short axis)
{
    unsigned long speed = stepper_speed[axis];
    speed = speed*100/MAX_SPEED;
    return speed;
}


void int_to_str(long val, bool zeropad, int chars, char* buf, int buflen);

void CTurret::SetSpeed(short axis, unsigned short speed)
{
    if (speed > 100)
        speed = 100;
    speed = speed*MAX_SPEED/100;
    if (stepper_previous_speed[axis] != speed) {
	if (speed > 0) {
	    WriteCommand(axis, speed, 'R');
	} else {
	    WriteCommand(axis, "Z");
	}
	stepper_previous_speed[axis] = speed;
    }
}



void CTurret::Stop(short axis)
{
    SetSpeed(axis, 0);
}


void CTurret::SetActiveHolding(short axis, bool onval)
{
    if (onval != active_holding[axis]) {
        WriteCommand(axis, (onval? 2 : 0), 'W');
        active_holding[axis] = onval;
    }
}


bool CTurret::GetActiveHolding(short axis)
{
    return active_holding[axis];
}


void CTurret::WaitUntilStopped(short axis)
{
    while (GetStepsLeft(axis));
}


void CTurret::RotateToPos(short axis, long pos, unsigned short speed)
{
    long currpos = GetCurrPos(axis);
    SetSpeed(axis, speed);
    target_step[axis] = pos;

    // Correct for mechanism backlash.
    if (currpos > pos) {
        backlash_engaged[axis] = true;
        pos -= backlash[axis];
    } else {
        backlash_engaged[axis] = false;
    }

    WriteCommand(axis, pos + zero_position[axis], 'G');
}


void CTurret::RotateSteps(short axis, long steps, unsigned short speed)
{
    long pos = GetCurrPos(axis) + steps;
    RotateToPos(axis, pos, speed);
}


void CTurret::Slew(short axis, short dir)
{
    if (dir == 0) {
        WriteCommand(axis, "Z");
    } else if (dir < 0) {
        WriteCommand(axis, "-S");
    } else {
        WriteCommand(axis, "+S");
    }
}


// Returns true if last limit switch update reported a closed switch.
bool CTurret::GetLimitSwitch(short axis)
{
    short mask = 0x03;
    if (!axis) {
        mask <<= 2;
    }
    return ((limit_switches & mask) != mask);
}



// Level of 0 tells it to watch for closed switch.
// Level of 1 tells it to watch for open switch.
void CTurret::WatchLimitSwitch(short axis, short enable, short level)
{
    short mask = 0x33;
    short bits = 0;
    short &limval = turret_limits_control_val;

    if (!enable) {
        bits |= 0x03;
    }
    if (level) {
        bits |= 0x30;
    }

    if (!axis) {
        mask <<= 2;
        bits <<= 2;
    }
    limval = (limval & ~mask) | bits;

    WriteCommand(limval, 'T');
}



void CTurret::RecalibrateHome(unsigned short speed)
{
    const unsigned short slow_speed = 10;
    const long timeout = 30;
    long last_time = 0;
    bool waitval[2];

    // Pause momentarily to get first position updates.
    sleep(1);

    // Make sure we start out with backlash engaged fully up.
    WatchLimitSwitch(0, 0, 0);
    WatchLimitSwitch(1, 0, 0);
    RotateSteps(0, -backlash[0], speed);
    RotateSteps(1, -backlash[1], speed);
    WaitUntilStopped(0);
    WaitUntilStopped(1);

    // Start homing X axis.
    if (GetLimitSwitch(0)) {
        WatchLimitSwitch(0, 1, 1);
        RotateToPos(0, position_maximum[0], speed);
	waitval[0] = false;
    } else {
        WatchLimitSwitch(0, 1, 0);
        RotateToPos(0, position_minimum[0], speed);
	waitval[0] = true;
    }

    // Simultaneously start homing Y axis.
    if (GetLimitSwitch(1)) {
        WatchLimitSwitch(1, 1, 1);
        RotateToPos(1, position_maximum[1], speed);
	waitval[1] = false;
    } else {
        WatchLimitSwitch(1, 1, 0);
        RotateToPos(1, position_minimum[1], speed);
	waitval[1] = true;
    }

    // Wait for both axes to stop or for timeout.
    last_time = time();
    while (time() - last_time < timeout &&
	    ((GetLimitSwitch(0) != waitval[0] && GetStepsLeft(0)) ||
	     (GetLimitSwitch(1) != waitval[1] && GetStepsLeft(1)))
	  );


    // Rewind X axis.
    if (!GetLimitSwitch(0)) {
        WatchLimitSwitch(0, 1, 0);
        RotateToPos(0, position_minimum[0], speed);
    }
    waitval[0] = true;

    // Simultaneously rewind Y axis.
    if (!GetLimitSwitch(1)) {
        WatchLimitSwitch(1, 1, 0);
        RotateToPos(1, position_minimum[1], speed);
    }
    waitval[1] = true;

    // Wait for both axes to stop or for timeout.
    last_time = time();
    while (time() - last_time < timeout &&
	    ((GetLimitSwitch(0) != waitval[0] && GetStepsLeft(0)) ||
	     (GetLimitSwitch(1) != waitval[1] && GetStepsLeft(1)))
	  );

    // Slowly find exact X axis home.
    if (GetLimitSwitch(0)) {
        WatchLimitSwitch(0, 1, 1);
        RotateToPos(0, position_maximum[0], slow_speed);
    }
    waitval[0] = false;

    // Simultaneously slowly find exact Y axis home.
    if (GetLimitSwitch(1)) {
        WatchLimitSwitch(1, 1, 1);
        RotateToPos(1, position_maximum[1], slow_speed);
    }
    waitval[1] = false;

    // Wait for both axes to stop or for timeout.
    last_time = time();
    while (time() - last_time < timeout &&
	    ((GetLimitSwitch(0) != waitval[0] && GetStepsLeft(0)) ||
	     (GetLimitSwitch(1) != waitval[1] && GetStepsLeft(1)))
	  );


    // Stop watching limit switches.
    WatchLimitSwitch(0, 0, 0);
    WatchLimitSwitch(1, 0, 0);
}





void CTurret::WriteAxis(short axis)
{
    if (previous_axis != axis) {
	if (axis == 2) {
	    command_buf.Put('B');
	} else {
	    command_buf.Put('X'+axis);
	}
	previous_axis = axis;
    }
}


void CTurret::WriteCommand(long val, char cmd)
{
    if (command_buf.SpaceLeft() < 10) {
        uart->WriteByte('<');
        while(command_buf.Count()) {
            char c = command_buf.Get();
            uart->WriteByte(c);
        }
        uart->WriteByte('>');
    }
    if (!turret_mutex++) {
        char buf[24];
        char *ptr = buf;
        int_to_str(val, false, 0, buf, sizeof(buf));
        while (*ptr) {
            command_buf.Put(*ptr++);
        }
        command_buf.Put(cmd);
    }
    turret_mutex--;
}


void CTurret::WriteCommand(const char* cmd)
{
    if (command_buf.SpaceLeft() < (signed int)strlen(cmd)) {
        uart->WriteByte('<');
        while(command_buf.Count()) {
            char c = command_buf.Get();
            uart->WriteByte(c);
        }
        uart->WriteByte('>');
    }
    if (!turret_mutex++) {
        while (*cmd) {
            command_buf.Put(*cmd++);
        }
    }
    turret_mutex--;
}


void CTurret::WriteCommand(short axis, long val, char cmd)
{
    if (command_buf.SpaceLeft() < 10) {
        uart->WriteByte('<');
        while(command_buf.Count()) {
            char c = command_buf.Get();
            uart->WriteByte(c);
        }
        uart->WriteByte('>');
    }
    if (!turret_mutex++) {
        char buf[24];
        char *ptr = buf;
	if (val > -10 && val < 10) {
	    if (val < 0) {
		*ptr++ = '-';
	        val = -val;
	    }
	    *ptr++ = '0' + val;
	    *ptr = '\0';
	    ptr = buf;
	} else {
	    int_to_str(val, false, 0, buf, sizeof(buf));
	}
	WriteAxis(axis);
        while (*ptr) {
            command_buf.Put(*ptr++);
        }
        command_buf.Put(cmd);
    }
    turret_mutex--;
}


void CTurret::WriteCommand(short axis, const char* cmd)
{
    if (command_buf.SpaceLeft() < (signed int)strlen(cmd)+1) {
        uart->WriteByte('<');
        while(command_buf.Count()) {
            char c = command_buf.Get();
            uart->WriteByte(c);
        }
        uart->WriteByte('>');
    }
    if (!turret_mutex++) {
	WriteAxis(axis);
        while (*cmd) {
            command_buf.Put(*cmd++);
        }
    }
    turret_mutex--;
}



void CTurret::SendCommand()
{
    if (time() - stepper_last_cmd_time > 3) {
        stepper_outstanding_cmds = 0;
    }
    if (stepper_outstanding_cmds < 1) {
        while (command_buf.Count()) {
            char c = command_buf.Get();
            uart->WriteByte(c);
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '?' || c == '=' || c == '!') {
		stepper_outstanding_cmds++;
		if (c != 'X' && c != 'Y' && c != 'B' && c != 'T') {
		    break;
		}
            }
        }
        stepper_last_cmd_time = time();
    }
}


void
TurretPoll()
{
    char buf[128];
    long val = 0;
    short valsigned = 0;
    long pos;
    CTurret* turret = cturret_list;

    while (turret) {
	if (turret->command_buf.SpaceLeft() > turret->command_buf.Count()) {
	    turret->WriteCommand(2, -1, '?');
	    turret->WriteCommand(2, -2, '?');
	    turret->WriteCommand(0,  5, '?');
	}
	turret->SendCommand();

	while (turret->uart->HasData()) {
	    if (!turret->uart->CharInReadBuf('\n')) {
		break;
	    }
	    turret->uart->GetString(buf, sizeof(buf));

	    if (buf[0] == '*') {
		if (turret->stepper_outstanding_cmds > 0) {
		    turret->stepper_outstanding_cmds--;
		} else {
		    turret->stepper_outstanding_cmds = 0;
		}
		continue;
	    }

	    if ((buf[0] == 'X' || buf[0] == 'Y') && buf[1] == ',') {
		if (buf[2] == '-' && buf[3] == '1' && buf[4] == ',') {

		    // Position update.
		    pos = 5;
		    val = 0;
		    valsigned = 0;
		    if (buf[pos] == '-') {
			valsigned = 1;
			pos++;
		    }
		    while (buf[pos] >= '0') {
			val = val * 10 + buf[pos] - '0';
			pos++;
		    }
		    if (valsigned) {
			val = -val;
		    }
		    turret->stepper_pos[buf[0]-'X'] = val;

		} else if (buf[2] == '-' && buf[3] == '2' && buf[4] == ',') {

		    // Speed update.
		    pos = 5;
		    val = 0;
		    valsigned = 0;
		    if (buf[pos] == '-') {
			valsigned = 1;
			pos++;
		    }
		    while (buf[pos] >= '0') {
			val = val * 10 + buf[pos] - '0';
			pos++;
		    }
		    if (valsigned) {
			val = -val;
		    }
		    turret->stepper_speed[buf[0]-'X'] = val;

		} else if (buf[2] == '5' && buf[3] == ',') {

		    // Limit switch update.
		    pos = 4;
		    val = 0;
		    while (buf[pos] >= '0') {
			val = val * 10 + buf[pos] - '0';
			pos++;
		    }
		    turret->limit_switches = val;

		} else if (buf[2] == '0' && buf[3] == ',') {

		    // Speed and position info update
		    pos = 4;
		    if (buf[pos] == '-') {
			valsigned = 1;
			pos++;
		    }
		    val = 0;
		    while (buf[pos] >= '0') {
			val = val * 10 + buf[pos] - '0';
			pos++;
		    }
		    if (valsigned) {
			val = -val;
		    }
		    turret->stepper_pos[buf[0]-'X'] = val;

		    pos++;
		    valsigned = 0;
		    if (buf[pos] == '-') {
			valsigned = 1;
			pos++;
		    }
		    val = 0;
		    while (buf[pos] >= '0') {
			val = val * 10 + buf[pos] - '0';
			pos++;
		    }
		    if (valsigned) {
			val = -val;
		    }
		    turret->stepper_speed[buf[0]-'X'] = val;
		}
	    }
	}
        turret = turret->next;
    }
}


