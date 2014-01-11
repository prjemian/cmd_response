.. $Id$

.. _commands:

USB command interface
++++++++++++++++++++++

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


-----

.. rubric:: commands

.. note:: Note about examples

   In all code examples below, command input to the Arduino
   is shown as:: 
   
     >>> some command 
   
   Response from the Arduino appears on the next line.
   When entering commands, do not type the ">>>" symbols.

.. _num_ai:

``?#ai``
--------

:purpose: How many ANALOG IN channels on this Arduino?
:command:  ``?#ai``
:returns: value of ``NUM_ANALOG_INPUTS`` as defined for this model of Arduino
:example: 
   (on Arduino Mega2560)
   
   >>> ?#ai
   16


.. _get_ai:

``?ai``
----------------

:purpose: read an ANALOG IN channel value
:command:  ``?ai pin``
:returns: 
   Returns the analog input *pin* value specified.
   The returned value is between 0 and 1023 (10-bit ADC) which represents
   a measured voltage between 0 and 5 VDC.
   The pin must be within the range of ``0..NUM_ANALOG_INPUTS``
:example:
   >>> ?ai 0
   171


.. _watch_ai:

``!ai:watch``
----------------

:purpose: 
   Enables (or disables) signal averaging for
   the specified analog input pin.  Note that it is
   possible to turn this feature on and off at run time.
:command:  ``!ai:watch pin 1|0``
:returns: ``Ok``
:default: by default, no ai pins are averaged
:example:
   >>> !ai:watch 0 1
   Ok


.. _ai_mean:

``?ai:mean``
----------------

:purpose: read a time-averaged ANALOG IN channel value
:command:  ``?ai:mean pin``
:returns: 
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
:example:
   >>> !ai:watch 0
   Ok
   >>> ?ai:mean 0
   171000


.. _num_bi:

``?#bi``
--------

:purpose: How many DIGITAL channels on this Arduino?
:command:  ``?#bi``
:returns: value of ``NUM_DIGITAL_PINS`` as defined for
   this model of Arduino.
:example:
   (on Arduino Mega2560)
   
   >>> ?#bi
   70


.. _get_bi:

``?bi``
----------------

:purpose: 
   Reads the digital input *pin* specified in the first argument.
   The returned value is either 0 or 1.
   The pin must be within the range of ``0..NUM_DIGITAL_PINS``.
   (Use ``?#bi`` to get ``NUM_DIGITAL_PINS``.)
:command:  ``?bi pin``
:returns: ``0|1``
:example:
   >>> ?bi 3
   0


.. _set_bo:

``!bo``
----------------

:purpose: 
   Sets the digital output *pin* specified in the first argument
   to the *value* specified in the second argument.  If the syntax
   is correct and the value is within range, returns ``Ok``.
   The pin must be within the range of ``0..NUM_DIGITAL_PINS``
   and must first be configured for output by calling ``!pin pin 1``.
   (Use ``?#bi`` to get ``NUM_DIGITAL_PINS``.)
:command:  ``!bo pin value``
:returns: ``Ok``
:example:
   >>> !pin 6 1
   Ok
   >>> !bo 6 0
   Ok

.. note:: Selection of a digital pin as *INPUT* (``0``)
   or *OUTPUT* (``1``) is based on the hardware wiring
   for that channel.  While configurable at run time,
   it is not expected to change this after its first
   setting.


.. _set_pwm:

``!pwm``
----------------

:purpose: 
   Sets the PWM-enabled digital output *pin* specified in the first argument
   to the *value* specified in the second argument.  If the syntax
   is correct and the value is within range, returns ``Ok``.
   The pin must first be configured for output by calling ``!pin pin 1``.

   The pin number is checked for PWM hardware-support by a call to the
   ``digitalPinHasPWM(pin)`` macro (which is defined by the Arduino SDK
   for each supported board type in
   ``<Arduino>/hardware/arduino/variants/*/pins_arduino.h``).
:command:  ``!pwm pin value``
:returns: ``Ok``
:example:
   >>> !pwm 11 127
   Ok



.. _set_pin:

``!pin``
----------------

:purpose: 
   Configures the digital output *pin* specified in the first argument
   for output as a binary digital output and also as a PWM digital
   output if the hardware enables this.

   ======  =================
   value   meaning
   ======  =================
   0	   use pin as input
   1	   use pin as output
   ======  =================
:command:  ``!pin pin value``
:returns: ``Ok``
:default: all pins are configured for input by default
:example:
  >>> !pin 11 1
  Ok

.. note:: Selection of a digital pin as *INPUT* (``0``)
   or *OUTPUT* (``1``) is based on the hardware wiring
   for that channel.  While configurable at run time,
   it is not expected to change this after its first
   setting.


.. _set_period:

``!t``
----------------

:purpose: 
   Sets the length of the averaging period (milliseconds).
   The argument ``period_ms`` (noted here as :math:`t`)
   must satisfy :math:`t_l <= t <= t_h` or an error
   is returned.
   Here :math:`t_l` is returned by ``?t:min``
   and :math:`t_h` is returned by ``?t:max``.

   The period can be changed at run time as fits the
   application.
:command:  ``!t period_ms``
:returns: ``Ok``
:example:
  >>> !t 100
  Ok



.. _get_period:

``?t``
----------------

:purpose: Read the length of the averaging period (milliseconds).
:command: ``?t``
:returns: length of averaging period (ms)
:default: ``1000``
:example:
   >>> ?t
   1000


.. _get_period_min:

``?t:min``
----------------

:purpose: 
   Returns :math:`t_l`, the minimum permitted length of the
   averaging period (milliseconds).  The minimum length
   is fixed, somewhat arbitrarily, to allow for at least
   a couple of updates if as many as 16 AI channels
   are to be averaged.
:command:  ``?t:min``
:returns: minimum allowed value of averaging period (ms)
:default: ``5``
:example:
   >>> ?t:min
   5


.. _get_period_max:

``?t:max``
----------------

:purpose: 
   Returns :math:`t_h`, the maximum permitted length of the
   averaging period (milliseconds).  The maximum length
   is fixed, somewhat arbitrarily, to ~20 minutes.
   Is any longer really necessary?
:command:  ``?t:max``
:returns: maximum allowed value of averaging period (ms)
:default: ``1000000``
:example:
   >>> 
   1000000


.. _set_k:

``!k``
----------------

:purpose: 
   Sets the multiplier to use when reporting
   averaged AI values.
   The argument ``multiplier`` (noted here as :math:`k`)
   must satisfy :math:`k_l <= k <= k_h` or an error
   is returned.
   Here :math:`k_l` is returned by ``?k:min``
   and :math:`k_h` is returned by ``?k:max``.
:command:  ``!k multiplier``
:returns: ``Ok``
:example:
   >>> !k 100
   Ok


.. _get_k:

``?k``
----------------

:command:  ``?k``
:returns: multiplier :math:`k` used when reporting averaged AI values
:default: ``1000``
:example:
   >>> ?k
   1000
   


.. _get_k_min:

``?k:min``
----------------

:purpose: 
   Returns :math:`k_l`, the minimum permitted multiplier used
   when reporting averaged AI values.
   The minimum multiplier is set, logically, at 1.
:command:  ``?k:min``
:returns: minimum allowed value of multiplier :math:`k`
:default: ``1``
:example:
   >>> ?k:min
   1


.. _get_k_max:

``?k:max``
----------------

:purpose: 
   Returns :math:`k_h`, the maximum permitted multiplier used
   when reporting averaged AI values.
   The maximum length is fixed, somewhat arbitrarily,
   at some large number.
:command:  ``?k:max``
:returns: maximum allowed value of multiplier :math:`k`
:default: ``1000000``
:example:
   >>> ?k:max
   1000000


.. _get_rate:

``?rate``
----------------

:purpose: How many ADC readings per second?
:command:  ``?rate``
:returns: 
   Returns the number of update loops per second.
   Each additional AI to be watched will decrease
   this number due to the operations of reading,
   accumulating, and averaging the signal.

   The update rate serves no purpose to the operation
   of the Arduino as an I/O controller.  It is only
   provided as a diagnostic signal for the remote
   control system.
:example:
   (Arduino Mega2560 with no AI channels watched)

   >>> ?rate
   25867


.. _get_version:

``?v``
----------------

:command:  ``?v``
:returns: Returns the software version number.

example:

>>> ?v
2


.. _get_id:

``?id``
----------------

:command:  ``?id``
:returns: Returns the software identifying string.

example:

>>> ?id
cmd_response


Example Commands
------------------------

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


.. _comm_parms:

Communications Parameters
----------------------------------------

======================   ========
term                     value
======================   ========
communications rate      115200
line terminator          ``\n``
buffer length (chars)    40
command length (chars)   16
======================   ========


file: ``cmd_reponse.ino``
------------------------------

The full Arduino ``cmd_response.ino``
code listing is too large for this documentation.

:EPICS Streams protocol:
   :download:`cmd_response.ino <../../cmd_response.ino>`
