.. $Id$

==============================================
cmd_response
==============================================

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

Theory of Operations
+++++++++++++++++++++

The main goal of this project is to present
the Arduino as an I/O controller using a 
command-response communications language
over the USB port.  The code should run on
any Arduino with a USB port.  The code
should provide access to the basic I/O 
capabilities of the Arduino.

To fit the code into the smaller SRAM space of 
Arduino models such as the Uno R3 (a 2k space), 
all strings were coded as ``char *`` rather
than use ``String`` [#]_ objects.

.. [#] http://arduino.cc/en/Reference/StringObject

One objective was to make basic reads and writes
very short, to simplify typing at some interactive
terminal.  The USB commands have this form::

  //  USB command interface of form: baseCmd [arg1 [arg2]]
  //
  // char* baseCmd (lower case)
  // long arg1, arg2 (no octal or hex interpreted)

Additionally, these rules were used to 
build the interface:

* Commands either **get** (start with ``?``) 
  or **set** (start with ``!``) a value.
  
* All commands received should return a response.

* No automatic command echo is provided.

* All **set** commands return "Ok" or an error message.

* All **get** commands return a value or an error message.

* All :ref:`errors` start with the text: ``ERROR_``

* Most error messages will return the command text as a diagnostic.

* The device provides a useful message on startup.

Startup
+++++++++

On startup, the device will output a message such
as this to the USB port::

  cmd_response started: 7113

This message (from an Arduino Mega2560) signifies 
the device is *running* and the **cmd_response**
interface code is ready to receive a 
command and return a response.
The number ``7113`` is the amount of free 
SRAM available on startup.  On the Uno R3, the number
is ca. 1700, meaning there is still plenty of
SRAM available.

If this startup message does not appear.
the device is not ready for communications
and something is wrong.  The logical list
to check starts with the values in the 
Communication Parameters (given
in :ref:`tables`).  If the problem persists,
check the external host's USB port,
the FTDI driver on the external host, 
read/write permissions to the USB port, and
the USB cable.  If the problem still persists,
question that the Arduino is running 
the **cmd_response** code.


.. _basics:

Basic Operations
-----------------

The basic capabilities common to all Arduino boards
consist of:

* a number of digital (binary) input/output (configurable) channels
* some of the digital channels have pulse-width modulation output capability
* a number of analog input channels (10-bit analog-to-digital converter)

The commands which support these basic operations are:
:ref:`num_ai`, :ref:`num_bi`, :ref:`get_ai`, 
:ref:`get_bi`, :ref:`set_pin`, :ref:`set_bo`,
and :ref:`set_pwm`.


.. _averaging:

Signal Averaging
----------------

It takes a few dozen [#]_ microseconds to read each
analog input channel, much faster than typical 
communications over USB to request and retrieve 
a value.  Averaging the analog values over a 
fixed period makes sense.  

.. [#] The actual time to convert the ADC signal
   is dependent on several factors, including
   the signal level itself.  For more information,
   see: http://arduino.cc/en/Reference/AnalogRead
   
   On the Mega2560, the rate with no watched channels
   was reported as 25841/s.  Watching just one 
   channel, the reported rate dropped to 6567/s.  
   For two channels, the reported rate was 3781/s.

The signal averages are available from the
command interface with :ref:`ai_mean`, 
in addition to the 
instantaneous values available with the 
:ref:`get_ai` command.  However, since it takes
valuable time to read these analog signals, 
only the desired channels should be averaged, 
to optimize for speed.  

.. note:: A channel must be 
   configured (with :ref:`watch_ai`) 
   before calling :ref:`ai_mean`.

The average for each channel is obtained by
accumulating measurements of the analog input,
:math:`a_i` until the update period has elapsed,

:math:`(k/n) \sum_i^n a_i`.

The total number of values accumulated during
each period, :math:`n`, is combined with the
period length, :math:`t` to compute the update
rate (returned by :ref:`get_rate`),

:math:`n / t`.

The average value has higher precision than the
instantaneous value.  Exactly how much higher
is empirical.  Some external control systems
expect *raw* (integer) values from I/O controllers.
With such raw numbers, the external control system
will apply automatically pre-configured scaling 
factors to place the number into engineering units, 
such as *VDC*.

Here, a positive multiplier, :math:`k`, is used
to scale the average value so that its limit 
of precision can be expressed as a *long* integer.
To request the value of :math:`k`, use :ref:`get_k`.
To request the value of :math:`k`, use :ref:`set_k`.
Limits of :math:`k` are found by requesting
:ref:`get_k_min` and :ref:`get_k_max`.

All *watched* channels are accumulated during each
period.  The averages (for just the watched channels)
and update rate are 
recomputed at the end of each period.


Configuration
----------------

It can be useful for an external control system 
to verify which controller is communicating.
Two commands are available to identify
this software (:ref:`get_id`) and 
version (:ref:`get_version`).

Commands
+++++++++++

.. _num_ai:

``?#ai``
--------

:format:  ``?#ai``
:example: ``?#ai``
:returns: ``16``  [#]_

Returns the value of ``NUM_ANALOG_INPUTS`` as defined for
this model of Arduino.

.. [#] value reported by the Arduino Mega2560


.. _get_ai:

``?ai``
----------------

:format:  ``?ai pin``
:example: ``?ai 0``
:returns: ``171``

Returns the analog input *pin* value specified.
The returned value is between 0 and 1023 (10-bit ADC) which represents
a measured voltage between 0 and 5 VDC.
The pin must be within the range of ``0..NUM_ANALOG_INPUTS``


.. _watch_ai:

``!ai:watch``
----------------

:format:  ``!ai:watch pin 1|0``
:example: ``!ai:watch 0 1``
:returns: ``Ok``
:default: by default, no ai pins are averaged

Enables (or disables) signal averaging for
the specified analog input pin.  Note that it is
possible to turn this feature on and off at run time.


.. _ai_mean:

``?ai:mean``
----------------

:format:  ``?ai:mean pin``
:example: ``?ai:mean 0``
:returns: ``171000``

Returns the average analog input *pin* value specified, 
multiplied by a factor :math:`k`.  The factor is
requested by the ``?k`` command and 
set by the ``!k`` command.
The returned value is between 0 and 1023 (10-bit ADC) which represents
a measured voltage between 0 and 5 VDC.
The *pin* must be within the range of 
``0..NUM_ANALOG_INPUTS`` 
(use ``?#ai`` to get ``NUM_ANALOG_INPUTS``)
and must first be configured for signal averaging 
by calling ``!ai:watch pin 1``.


.. _num_bi:

``?#bi``
--------

:format:  ``?#bi``
:example: ``?#bi``
:returns: ``70``  [#]_

Returns the value of ``NUM_DIGITAL_PINS`` as defined for
this model of Arduino.

.. [#] value reported by the Arduino Mega2560


.. _get_bi:

``?bi``
----------------

:format:  ``?bi pin``
:example: ``?bi 3``
:returns: ``0|1``

Reads the digital input *pin* specified in the first argument.
The returned value is either 0 or 1.
The pin must be within the range of ``0..NUM_DIGITAL_PINS``.
(Use ``?#bi`` to get ``NUM_DIGITAL_PINS``.)


.. _set_bo:

``!bo``
----------------

:format:  ``!bo pin value``
:example: ``!bo 6 0``
:returns: ``Ok``

Sets the digital output *pin* specified in the first argument 
to the *value* specified in the second argument.  If the syntax
is correct and the value is within range, returns ``Ok``.  
The pin must be within the range of ``0..NUM_DIGITAL_PINS``
and must first be configured for output by calling ``!pin pin 1``.
(Use ``?#bi`` to get ``NUM_DIGITAL_PINS``.)

.. note:: Selection of a digital pin as *INPUT* (``0``)
   or *OUTPUT* (``1``) is based on the hardware wiring
   for that channel.  While configurable at run time,
   it is not expected to change this after its first
   setting.


.. _set_pwm:

``!pwm``
----------------

:format:  ``!pwm pin value``
:example: ``!pwm 11 127``
:returns: ``Ok``

Sets the PWM-enabled digital output *pin* specified in the first argument 
to the *value* specified in the second argument.  If the syntax
is correct and the value is within range, returns ``Ok``.  
The pin must first be configured for output by calling ``!pin pin 1``.

The pin number is checked for PWM hardware-support by a call to the
``digitalPinHasPWM(pin)`` macro (which is defined by the Arduino SDK 
for each supported board type in 
``<Arduino>/hardware/arduino/variants/*/pins_arduino.h``).


.. _set_pin:

``!pin``
----------------

:format:  ``!pin pin value``
:example: ``!pin 11 1``
:returns: ``Ok``
:default: all pins are configured for input by default

Configures the digital output *pin* specified in the first argument 
for output as a binary digital output and also as a PWM digital
output if the hardware enables this.

======  =================
value   meaning
======  =================
0       use pin as input
1       use pin as output
======  =================

.. note:: Selection of a digital pin as *INPUT* (``0``)
   or *OUTPUT* (``1``) is based on the hardware wiring
   for that channel.  While configurable at run time,
   it is not expected to change this after its first
   setting.


.. _set_period:

``!t``
----------------

:format:  ``!t period_ms``
:example: ``!t 100``
:returns: ``Ok``

Sets the length of the averaging period (milliseconds).
The argument ``period_ms`` (noted here as :math:`t`) 
must satisfy :math:`t_l <= t <= t_h` or an error 
is returned.
Here :math:`t_l` is returned by ``?t:min``
and :math:`t_h` is returned by ``?t:max``.

The period can be changed at run time as fits the
application.

.. _get_period:

``?t``
----------------

:format:  ``?t``
:example: ``?t``
:returns: ``Ok``
:default: ``1000``

Returns the length of the averaging period 
(milliseconds).


.. _get_period_min:

``?t:min``
----------------

:format:  ``?t:min``
:example: ``?t:min``
:returns: ``Ok``
:default: ``5``

Returns the minimum permitted length of the 
averaging period (milliseconds).  The minimum length
is fixed, somewhat arbitrarily, to allow for at least
a couple of updates if as many as 16 AI channels 
are to be averaged.


.. _get_period_max:

``?t:max``
----------------

:format:  ``?t:max``
:example: ``?t:max``
:returns: ``Ok``
:default: ``1000000``

Returns the maximum permitted length of the 
averaging period (milliseconds).  The maximum length
is fixed, somewhat arbitrarily, to ~20 minutes.
Is longer really necessary?


.. _set_k:

``!k``
----------------

:format:  ``!k multiplier``
:example: ``!k 100``
:returns: ``Ok``

Sets the multiplier to use when reporting 
averaged AI values.
The argument ``multiplier`` (noted here as :math:`k`) 
must satisfy :math:`k_l <= k <= k_h` or an error 
is returned.
Here :math:`k_l` is returned by ``?k:min``
and :math:`k_h` is returned by ``?k:max``.


.. _get_k:

``?k``
----------------

:format:  ``?k``
:example: ``?k``
:returns: ``Ok``
:default: ``1000``

Returns the multiplier used when reporting 
averaged AI values.


.. _get_k_min:

``?k:min``
----------------

:format:  ``?k:min``
:example: ``?k:min``
:returns: ``Ok``
:default: ``1``

Returns the minimum permitted multiplier used 
when reporting averaged AI values.  
The minimum multiplier is set, logically, at 1.


.. _get_k_max:

``?k:max``
----------------

:format:  ``?k:max``
:example: ``?k:max``
:returns: ``Ok``
:default: ``1000000``

Returns the maximum permitted multiplier used 
when reporting averaged AI values.  
The maximum length is fixed, somewhat arbitrarily, 
tat some large number.


.. _get_rate:

``?rate``
----------------

:format:  ``?rate``
:example: ``?rate``
:returns: ``25867`` [#]_

Returns the number of update loops per second.
Each additional AI to be watched will decrease
this number due to the operations of reading, 
accumulating, and averaging the signal.

.. [#] value for Arduino Mega2560 with no AI channels watched.

The update rate serves no purpose to the operation
of the Arduino as an I/O controller.  It is only
provided as a diagnostic signal for the remote
control system.

.. _get_version:

``?v``
----------------

:format:  ``?v``
:example: ``?v``
:returns: ``2``

Returns the software version number.


.. _get_id:

``?id``
----------------

:format:  ``?id``
:example: ``?id``
:returns: ``cmd_response``

Returns the software identifying string.


.. document each of these commands
   ?v	     long      returns version number
   ?id	     0         returns identification string
   ?rate     long      returns number of updates (technically: loops) per second

Examples
+++++++++++

.. TODO: need more

1. Read analog input from pin 0:

>>> ?ai 0
41

2. Set digital pin 11 for PWM output:

>>> !pin 11 1
Ok

3. Set PWM output pin 11 to 128:

>>> !pwm 11 128
Ok

4. Show how a bad command (no space between baseCmd and pin) is handled:

>>> !pwm11 128
ERROR_UNKNOWN_COMMAND:!pwm11 128

.. _errors:

Error messages
++++++++++++++++++++++

This is a list of the possible error messages and their meanings.
All error messages begin with the text ``ERROR_`` and then some 
terse descriptor of the error.
In most cases, the input that triggered the error message is 
returned.  A single ":" is used as the delimiter when the input is appended.

``ERROR_AI_PIN_NOT_AVAILABLE:input``
   The specified pin number is not available for 
   analog input on this hardware.

``ERROR_AI_PIN_NOT_WATCHED:input``
   The specified pin number was not set up for averaging.
   Need to call ``!ai:mean pin 1`` to enable averaging on this pin.

``ERROR_BINARY_RANGE:input``
  The value must be either ``0`` or ``1``.

``ERROR_BI_PIN_NOT_AVAILABLE:input``
   The specified pin number is not available for 
   binary (digital) input on this hardware.

``ERROR_BO_PIN_NOT_AVAILABLE:input``
   The specified pin number is not available for 
   binary (digital) output on this hardware.

``ERROR_BUFFER_OVERFLOW``
  Too many characters were received before the line terminator.
  All characters received so far will be discarded.
  
  Some Arduinos do not have much available RAM.
  The current buffer length is 40 characters.

``ERROR_COMMAND_FORMAT:input``
  All commands must have at least one space separating the baseCmd from 
  the pin number.  This command is generated when no space is detected
  in the input.

``ERROR_DIGITAL_PIN_NOT_AVAILABLE:input``
  The specified binary (digital) pin number is 
  not available on this hardware.

``ERROR_PIN_NOT_PWM:input``
  The specified pin is not supported for PWM on this Arduino hardware.
  This is determined by calling the Arduino system macro 
  ``digitalPinHasPWM(pin)`` which is defined for each different type 
  of Arduino hardware variation.

``ERROR_NOT_IMPLEMENTED_YET:input``
  The command specified has not yet been implemented.

``ERROR_PWM_RANGE:input``
  The PWM value must be between 0 and 255, inclusive.
  This error is reported for any value outside this range.

``ERROR_TOO_MANY_ARGUMENTS:input``
  The general form for input commands is ``baseCmd pin [value]``
  where ``baseCmd`` is given in the table above, ``pin`` is an 
  integer appropriate for the chosen hardware interface, and
  ``value`` is only used for "!" (set) commands.
  
  At this time, if ``value`` is specified for a "?" (read) command,
  it is ignored.  In the future, this will generate an error message.

``ERROR_UNKNOWN_COMMAND:input``
  The input was not recognized as a valid command.
  One reason for this might be the use of upper case.
  Other possibilities exist.

.. _tables:

Tables
+++++++++

.. rubric:: Table: Communications Parameters

======================   ========
term                     value
======================   ========
communications rate      115200
line terminator          ``\n``
buffer length (chars)    40
command length (chars)   16
======================   ========

.. rubric:: Table: USB command interface

When present, "#" refers to the Arduino pin number used in the operation
  
================  ========  =====================================================================
command           value     action
================  ========  =====================================================================
``?#ai``          int       returns NUM_ANALOG_INPUTS
``?ai pin``       0..1023   returns current value of numbered analog input
``!ai:watch``     0..1 	    sets up ai pin for averaging
``?ai:mean``      long 	    returns ``<ai>*k``
``?#bi``          int       returns NUM_DIGITAL_PINS
``?bi pin``       0..1      returns current value of numbered digital input
``!bo pin v``     0..1      sets numbered digital output to value v
``!pwm pin v``    0..255    sets numbered PWM digital output to value v
``!pin pin v``    0..1      sets mode of digital pin to value v (value: 1=OUTPUT, not 1=INPUT)
``!t``            long      sets averaging time, ms
``?t``            long      returns averaging time, ms
``?t:min``        long 	    returns minimum allowed averaging time, ms
``?t:max``        long 	    returns maximum allowed averaging time, ms
``!k``            long 	    sets averaging factor (``k``)
``?k``            long 	    returns averaging factor (``k``)
``?k:min``        long 	    returns minimum allowed averaging factor (``k``)
``?k:max``        long 	    returns maximum allowed averaging factor (``k``)
``?v``            long 	    returns version number
``?id``           0         returns identification string
``?rate``         long 	    returns number of updates (technically: loops) per second
other             ..        returns "ERROR_UNKNOWN_COMMAND:text"
================  ========  =====================================================================

notes: 

#. must use lower case (as shown in table)
#. integers must be specified in decimal (no octal or hex interpreted)
#. pin numbers are not checked for correctness in the current version
#. ``?`` commands return an integer
#. ``!`` commands return ``Ok``
#. Errors, starting with ``ERROR_`` will substitute for expected output


..
	EPICS Streams protocol
	=======================

	========  ================================================
	protocol  meaning
	========  ================================================
	ai(pin)   read analogRead(pin) into the record's RVAL
	bi(pin)   read digitalRead(pin) into the record's VAL
	bo(pin)   write digitalWrite(pin) from the record's VAL
	pwm(pin)  write analogWrite(pin) from the record's RVAL
	========  ================================================

	.. note:: For the ``bo`` and ``pwm`` protocols, the selected
	   pin will be configured by that protocol for output during
	   record initialization.
