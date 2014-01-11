.. $Id$

.. index:: example; Python program

.. _python_sequence:

=======================================
Example: Python program ``sequence.py``
=======================================

A Python program was written to demonstrate
how to communicate with the Arduino as an
I/O controller using the **cmd_response**
interface.

Use the circuit described previously (see :ref:`example_circuit`):

  :PWM:
    * pin 11: LED, pointing at the photocell, sets :math:`V_{LED}`

  :AI:
    * pin 0: photocell, monitors :math:`V_P`
    * pin 1: LED supply voltage, monitors :math:`V_{LED}`


The basics of this program are:

* attach to the USB port

  * On linux, confirm the USB connection using:
  
    >>> lsusb | grep FT232
    Bus 005 Device 002: ID 0403:6001 Future Technology Devices International, Ltd FT232 USB-Serial (UART) IC


* wait (arbitrary time) for the port to connect
* configure the PWM and averaging for a 
  simple circuit, 0.1 s period, PWM on pin 11,
  AI on pins 0 and 1::

   !t 100
   !ai:watch 0 1
   !ai:watch 1 1
   !pin 11 1

* wait a short time for the averaging period to set
* step the PWM (:math:`V_P`) through its range::

   '!pwm 11 %d' % pwm

* wait a short time for the ADC to settle 
  and the averaging period to complete
* measure the AI channels (:math:`V_P` and :math:`V_{LED}`) several times at each step::

   ?ai 0
   ?ai:mean 0
   ?ai:mean 1

Note the difference between readings of ``?ai 0`` and ``?ai:mean 0``.
This shows the effectiveness of signal averaging.  [#]_ 

.. [#] Also see the Arduino *Smoothing* example: http://arduino.cc/en/Tutorial/Smoothing

.. automodule:: sequence
    :members: 

Program Output
--------------

The full output (:download:`results.txt <../../../python/results.txt>`), 
more than 600 lines, is too long for this documentation.
Here are the first few lines, to give you an idea:

.. literalinclude:: ../../../python/results.txt
   :linenos:
   :lines: 1-10
