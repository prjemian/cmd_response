
============
cmd_response
============

--------------------------------------------------------------------------------------
simple command/response access via USB to I/O capabilities of Arduino Microcontrollers
--------------------------------------------------------------------------------------

:author: Pete R. Jemian <prjemian gmail com>
:contributors:  Keenan Lang
:URL: http://prjemian.github.io/cmd_response
:git: https://github.com/prjemian/cmd_response

.. toctree::
   :hidden:
   
   contents

The Arduino family of microcontrollers possess a reasonable
collection of input and output capabilities.  While there are
different versions, each with some unique capabilities, most
controllers have a USB port, some digital input and output ports,
a few pulse-width modulated (PWM) digital ports, and some analog
input ports.  This code module programs the Arduino with a 
simple command/response language via USB providing access to 
Arduino's I/O capabilities.

This code was developed, tested, and is known 
to operate properly using these Arduino boards:

* Arduino Uno R3
* OSEPP Uno R3
* Arduino Mega1280
* Arduino Mega2560

I/O System Interface
====================

.. index:: EPICS

The Arduino can contribute its collection of
I/O capabilities to an external control system, 
such as EPICS [#]_.  To EPICS (or some
other control system), the Arduino appears as
an I/O controller.
To interface with an external system, 
a generic command language, independent of the 
details of the external system, 
is programmed into the Arduino
and the board communicates over USB.
This project is only useful for those Arduino boards
which have an USB port.

.. [#] http://www.aps.anl.gov/epics
