.. $Id$

.. _commands:

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

Examples Commands
------------------------

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
