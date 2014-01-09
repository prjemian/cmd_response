.. $Id$

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
