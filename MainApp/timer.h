#ifndef TIMER_H
#define TIMER_H

#define TIMER_BASE (&GBA_REG_TM0D)
#define TIMER_DATA(x) TIMER_BASE[x<<1]
#define TIMER_CNTL(x) TIMER_BASE[(x<<1)+1]

#define TIMER_FREQ_SYS  0x0
#define TIMER_FREQ_64   0x1
#define TIMER_FREQ_256  0x2
#define TIMER_FREQ_1024 0x3
#define TIMER_OVERFLOW  0x4
#define TIMER_IRQ_EN   0x40
#define TIMER_ENABLE   0x80

#endif

