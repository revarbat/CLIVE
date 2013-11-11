C.L.I.V.E.
==========

Celestial Laser Identification and Visualization Experiment: Astronomy
software to point out stars, planets, etc. using a stepper based turret.

This code used to run on a GameBoy Advance with an XPort FPGA card, but
is currently being rewritten to run under Linux. It is not yet functional.

The new design uses an app running on a Linux box (or Raspberry Pi) to do
the main orbital and 3D calculations, and to provide the main interface.
It controls the turret over USB using an Arduino Mega with a RAMPS shield,
with slightly custom firmware.  The physical turret is 3D printable in ABS.
Rough Alt/Az calibration is performed using compass and accelerometers on
the turret.  A GPS provides position and time info for orbital calculations.

----------

Directories:
  TurretModels    3D Models for a printable turret.
  TurretFirmware  Custom firmware for a RAMPS turret stepper controller.
  MainApp         Sources for the main astronomy linux app.

