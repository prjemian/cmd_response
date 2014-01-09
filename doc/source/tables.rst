.. $Id$

.. _tables:

Tables
+++++++++

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

USB command interface
----------------------------------------

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
