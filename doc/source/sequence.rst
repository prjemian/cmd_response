.. $Id$

.. index:: example; Python program

===============================
Example: ``sequence.py``
===============================

A Python program was written to demonstrate
how to communicate with the Arduino as an
I/O controller using the **cmd_response**
interface.

The hardware is wired (see the :ref:`EPID <fig.epid_circuit>` example):

  :AI:
    pin 0, photocell

  :PWM:
    pin 11, LED, pointing at the photocell


The basics of this program are:

* attach to the USB port
* wait (arbitrary time) for the port to connect
* configure the PWM and averaging for a 
  simple circuit, 0.1 s period, PWM on pin 11,
  AI on pin 0::

   !t 100
   !ai:watch 0 1
   !pin 11 1

* wait a short time for the averaging period to set
* step the PWM through its range::

   '!pwm 11 %d' % pwm

* wait a short time for the ADC to settle 
  and the averaging period to complete
* measure the AI channel several times at each step::

   ?ai 0
   ?ai:mean 0

.. automodule:: sequence
    :members: 

Program Output
----------------

-tba-
