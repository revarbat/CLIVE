#PREFIX=arm-agb-elf-
#PREFIX=arm-eabi-
PREFIX=
CC=$(PREFIX)gcc
CXX=$(PREFIX)g++
CPP=$(PREFIX)cpp
AS=$(PREFIX)as
OBJCOPY=$(PREFIX)objcopy
DEPSDIR=.

# EXTRA_DEFS= -DNO_XPORT -DNO_GBA
EXTRA_DEFS=
# ARCH_FLAGS= -mcpu=arm7tdmi -mthumb-interwork
ARCH_FLAGS=
ARCH_CFLAGS=
INCLUDES= -I../../include $(EXTRA_DEFS)
CFLAGS= -g -O2 -Wall $(ARCH_CFLAGS) $(INCLUDES)
CXXFLAGS= -g -O2 -Wall $(ARCH_CFLAGS) $(INCLUDES)
ASFLAGS= 
CPPFLAGS= $(ARCH_FLAGS)
LDFLAGS= $(ARCH_FLAGS) -Wl,-Map,$(notdir $@).map
LIBPATHS=
LIBS= -lg -lm
LINKSPECS=

TARGET= flareptr
OFILES= main.o fbtext.o fbinterrupt.o fbtimer.o clockutil.o rcservo.o turret.o gpio.o uart.o keypad.o keysin.o scroller.o matrixmath.o flarelist.o star.o starlist.o planetlist.o constlines.o celestial.o skyplot.o datetime.o laser.o gps.o graphics.o page.o p_config_int.o p_config_long.o p_config_bool.o p_tracker.o p_calibrate_coarse.o p_calibrate_3star.o p_starfinder.o p_planetfinder.o p_info.o p_debug.o
DFILES=$(OFILES:.o=.d)

TESTTARGET= ftest
TESTOFILES= testbench.o fbtext.o matrixmath.o datetime.o celestial.o planetlist.o starlist.o star.o
TESTDFILES=$(TESTOFILES:.o=.d)



####### Build rules


all: $(TARGET).bin $(TARGET).gba

-include $(DFILES)

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -v -O binary $(TARGET).elf $(TARGET).bin

upload: $(TARGET).bin 
	xpcomm $(TARGET).bin logic

$(TARGET).elf: $(OFILES)
	$(CXX) $(LDFLAGS) $(LINKSPECS) $(OFILES) $(LIBPATHS) $(LIBS) -o $(TARGET).elf

clean:
	rm -rf $(TARGET).bin $(TARGET).elf $(TARGET).elf.map $(TARGET).gba $(TARGET)
	rm -rf $(OFILES)
	rm -rf $(DFILES)
	rm -rf $(TESTTARGET)
	rm -rf $(TESTOFILES)
	rm -rf $(TESTDFILES)

$(TESTTARGET): $(TESTOFILES)
	$(CXX) $(TESTOFILES) -o $(TESTTARGET)

test:
	make CC=gcc CXX=g++ INCLUDES=-DTESTBENCH ARCH_FLAGS= ARCH_CFLAGS= LDFLAGS= LIBS= $(TESTTARGET)

noxport:
	make EXTRA_DEFS=-DNO_XPORT $(TARGET).bin

