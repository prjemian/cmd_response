.. $Id$

.. _epid_circuit:

=================================================
Electronic Circuit
=================================================

The **cmd_response** sketch is very general with respect 
to the ANALOG IN and DIGITAL I/O of the Arduino boards.
We can use it to read a wide variety of sensors from 
remote computer systems.  This was shown previously in the
Python code example: :ref:`python_sequence`.

To further demonstrate how the **cmd_response** sketch works, we construct
some simple sensor circuits.  The goal of these circuits is to
first demonstrate how they work with the Arduino.
Then, we show how to integrate the Arduino with EPICS.
Finally, we work with EPICS PID controls, where a chosen value of the
Arduino's photocell signal is maintained by adjusting 
the Arduino's LED brightness using a PID loop in EPICS.

photocell
----------

The photocell is a cadmium-sulfide photosensitive resistor.  [#]_
It is an inexpensive device that measures the amount of light
reaching its active surface.  The resistance of the photocell, R2, changes
as the intensity of light changes.

An easy way to sense this resistance is to build a simple voltage divider 
circuit such as the next figure and measure voltage, :math:`V_P`
at the midpoint of the circuit between R2 and R3.  

.. math::  V_P = V_{cc} \left( {R2 \over R2 + R3} \right)

Since the resistance of the photocell, R2,
drops with increasing light intensity, we choose to put it
closer to the supply voltage, :math:`V_{cc}` [#]_.  As the light intensity
increases, :math:`V_P` will increase towards :math:`V_{cc}`.  The other 
resistor, R3, is chosen to limit the maximum current through the divider
as R2 tends towards zero.

..
   .. figure:: photocell_v_divider.png
       :alt: fig.photocell_v_divider
       :width: 50%
       :align: center

       Voltage divider circuit with the photocell
       (:download:`photocell_v_divider.png`)

:connection: 
   We'll connect :math:`V_P` to ANALOG IN channel 0.

.. [#] photocell: http://en.wikipedia.org/wiki/Photoresistor
.. [#] Take :math:`V_{cc}` from the Arduino's 5 VDC supply.

LED
---

The LED is an inexpensive device that generates 
light when a current is passed through it.
A resistor is used to limit the current which
flows through the LED as it is driven from one of the DIGITAL
pins on the Arduino.  Arduino has an example to vary the intensity of the LED
using pulse-width modulation and the ``analogWrite()`` function.  [#]_

..
   .. figure:: LED_circuit.png
       :alt: fig.LED_circuit
       :width: 50%
       :align: center

       Lighting an LED with an applied voltage.
       (:download:`LED_circuit.png`)

The current that flows through the LED, :math:`i_{LED}` is given by:

.. math::  i_{LED} = {V_{LED} \over R1}

Only 10-30 mA should be given to :math:`i_{LED}`.  
With R1 = 330 Ohms and :math:`V_{LED}=5` VDC (full output from a DIGITAL pin),
then :math:`i_{LED}=15` mA.

.. note::  The intensity of light from LED1 is not linear with PWM value.


:connection: 
   We'll connect :math:`V_{LED}` to DIGITAL (pwm) channel 11.

.. [#] LED Fading: http://arduino.cc/en/Tutorial/Fade

Complete circuit
--------------------

.. _fig.epid_schematic:

.. figure:: LED_sensor_schem.png
    :alt: fig.epid_schematic
    :width: 50%
    :align: center

    Electronic Circuit Schematic: Arduino, photocell, and LED
    (:download:`LED_sensor_schem.png`)

.. _fig.epid_breadboard:

.. figure:: LED_sensor_bb.png
    :alt: fig.epid_breadboard
    :width: 80%
    :align: center

    Electronic Circuit Breadboard: Arduino, photocell, and LED
    (:download:`LED_sensor_bb.png`)

Reduce background light
------------------------

.. couple of photos of the circuit here

.. tip:: Reduce the background light that reaches the photocell.
   Place the photocell and LED inside a piece of black tubing.
   
   Suggestions for a black tube: 
   
   * piece of shrink wrap (don't shrink it!)
   * the outer insulation from a multiconductor cable


Fritzing layout
----------------

The circuit was layed out using the Fritzing software
(http://fritzing.org).  
Download the layout file here: :download:`LED_sensor.fzz`.
