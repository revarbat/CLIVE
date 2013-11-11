C.L.I.V.E.
==========

Celestial Laser Identification and Visualization Experiment: Astronomy
software to point out stars, planets, etc. using a stepper based turret.


Design History
--------------

This code used to run on a GameBoy Advance with an XPort FPGA card, but
is currently being rewritten to run under Linux. It is not yet functional.

The new design uses an app running on a Linux box (or Raspberry Pi) to do
the main orbital and 3D calculations, and to provide the main interface.
It controls the turret over USB using an Arduino Mega with a RAMPS shield,
with slightly custom firmware.  The physical turret is 3D printable in ABS.
Rough Alt/Az calibration is performed using compass and accelerometers on
the turret.  A GPS provides position and time info for orbital calculations.


Hardware
--------

This design requires the following hardware:
* Arduino Mega 1280 or 2560
* RAMPS board (Reprap Arduino Mega Pololu Shield)
* 2x Pololu sub-boards.
* FTDI USB to Serial Adaptor.
* Almost any Serial GPS sensor.
* MPU6050 I2C Accelerometer/Gyro sensor.
* HMC5883 I2C Compass sensor.
* 2x NEMA17 Stepper Motors.
* 3x four-pin header plugs for stepper and I2C sensor connections.
* Printed Turret Parts. (Designs in TurretModels Dir.)
* USB fullsized A-B cable.
* USB Mini cable.
* Linux computer or Raspberry Pi board with Raspian Linux.

----------

Directories:
<dl>
  <dt>TurretModels</dt>    <dd>3D Models for a printable turret.</dd>
  <dt>TurretFirmware</dt>  <dd>Custom firmware for a RAMPS turret stepper controller.</dd>
  <dt>MainApp</dt>         <dd>Sources for the main astronomy linux app.</dd>
</dl>

