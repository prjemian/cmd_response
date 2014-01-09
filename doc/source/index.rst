.. $Id$

==============================================
cmd_response
==============================================

.. toctree::
   :hidden:
   
   contents

----------------------------------------------------------------------
simple command/response access via USB to Arduino's I/O capabilities
----------------------------------------------------------------------

.. rst2pdf -o cmd_response.pdf \
   --header="$Id$" \
   --footer="Page ###Page###" \
   cmd_response.rst

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
* Arduino Mega1280
* Arduino Mega2560

Arduino Interface
====================

The Arduino can contribute its collection of
I/O capabilities to an external control system, 
such as EPICS [#]_.  To EPICS (or some
other control system), the Arduino appears to
be an I/O controller.
To interface with an external system, 
a generic command language, independent of the 
details of the external system, 
can be programmed into the Arduino
and the board can communicate over the USB port.
This project is only useful for those Arduino boards
which have the USB port.

.. [#] http://www.aps.anl.gov/epics

------

documentation built: |today|
