.. index:: CSS BOY, EPICS; graphical interface

.. _cssboy:

CSS BOY screen
##############

.. figure:: cssboy_screen.png
   :alt: fig.cssboy_screen
   :width: 50%
   :align: center
   
   Example CSS BOY screen
   (:download:`cssboy_screen.png`)

Interesting to note the variation of the raw values for the two analog voltage
inputs is quite different.  For the photocell voltage,
:math:`V_P`, the signal is noisy but remains close to the
time-averaged value.  The signal for the voltage applied to the LED,
:math:`V_{LED}`, cycles between only two numbers: 0 and 4.868 VDC.
The :math:`V_P` signal is a steady voltage through a voltage divider circuit.
The :math:`V_{LED}` signal is the output from the pulse-width modulation
DIGITAL output pin D11.  In PWM, the voltage is cycled between 0 and supply
(:math:`V_{cc}`) so that the "on" duty cycle during the PWM period, is
proportional to the desired PWM level.  On a time-average, the output 
approximates a steady voltage, but when read as instantaneous values, only
0 or :math:`V_{cc}` are returned.

:download:`LED_sensor.opi <../../../epics/cssboy/LED_sensor.opi>`
