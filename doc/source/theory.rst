.. $Id$

==============================================
Theory of Operations
==============================================


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
