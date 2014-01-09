.. $Id$

.. index:: example; Getting started

=================================================
Example: Getting started
=================================================

First, install and configure the Arduino software on your computer, 
install the **cmd_response** project where the Arduino software will find it,
connect your Arduino board by USB to your computer, 
identify the board and serial port on the 
*Tools* menu, and upload the **cmd_response** sketch to your Arduino.

Once these steps are complete,
start a *Serial Monitor* session (again in the Arduino software tool).
Be certain to set the end-of-line terminator to *Newline* and 
the BAUD rate to 115200 baud.  (See :ref:`comm_parms`.
If you don't do these two steps, 
your Arduino will not respond to your typed commands.)

.. note:: Note about examples

   In all code examples below, command input to the Arduino
   is shown as:: 
   
     >>> some command 
   
   Response from the Arduino appears on the next line.
   When entering commands, do not type the ">>>" symbols.


Is our sketch running?
--------------------------

When you open the serial monitor and it is set 
with the correct BAUD rate, then this message (or one very like it) appears::

  cmd_response started: 1581

This output is produced by the **cmd_response** sketch (from an OSEPP Uno).
It tells us the sketch name and the amount of free RAM available.
Likely, this last number is only of interest to those wishing 
to modify the **cmd_response** software for a larger vocabulary.

What is our sketch ID?
--------------------------

The ID string of the sketch is available:

>>> ?id
cmd_response

.. note::  The ID command is a capability of the **cmd_response** sketch.
   Don't expect it to work on any other sketches.

What is our software version?
-------------------------------

The software version number of the sketch is available:

>>> ?v
2

This number is set in the sketch.

Light the on-board LED
--------------------------

Most Arduino boards have an LED connected to digital pin 13.

Switch the on-board LED on:

>>> !bo 13 1
Ok

Switch the on-board LED off:

>>> !bo 13 0
Ok

Read analog input channels: 0 and 1
--------------------------------------

You read the value of an analog input channel, such as channel 0:

>>> ?ai 0
149

Read channel 1:

>>> ?ai 1
9

Since there is no electrical circuit supplying a signal 
to either of these two analog inputs, the values appear to be random
and are said to be *floating*.

You stabilize the value returned by connecting a signal.
The easiest available signals are either GND or 5V provided by the board.
Connect a jumper wire from GND to ANALOG IN channel 0 and read again:

>>> ?ai 0
0

Remove the jumper wire and reconnect it to 5V and ANALOG IN channel 0, then read again:

>>> ?ai 0
1023

Test the voltage of a AA battery
--------------------------------------

A single cell battery such as a AA [#]_ makes a clean signal to test.
Using a pair of jumper wires, connect one wire between GND and the 
*(-)* end of the battery.  Connect the other wire from ANALOG IN channel 1
to the *(+)* end of the battery.  Hold these two wires firmly with one hand and read:

>>> ?ai 1
310

Using a little math, we divide the value returned by the full scale value of the
analog input channel (1023) and then multiply by the full scale voltage of the
analog inputs (5 VDC):  :math:`1.515 = (310/1023) \times 5`.  

At 1.515 VDC, this AA battery is still "good" 
(its voltage is close to the stated value of 1.5 VDC). 

.. [#] Any single cell battery under 5 VDC 
   (such as AA, AAA, AAAA, C, D, ...)
   makes a clean, quiet signal to test.
   Don't use any signal more than 5 VDC 
   (such as a 9 VDC battery) 
   since it may damage your Arduino board.
   Also note, some Arduino boards have 3.3 VDC inputs.  
   Take note what your board can handle.

Measuring a time-averaged signal
----------------------------------

Some analog signals are not as quiet as the output from a battery.
The **cmd_response** sketch has a feature to time-average the signal 
on selected analog input channels.  
For each channel monitored, the effective sampling rate [#]_ drops
as the program has more work to do.

While the software can average 
all channels on the board, the averaging process takes some time.
With no channels watched, the sampling rate of my board is:

>>> ?rate
43907

This means the main loop of the program is starting 
every 0.023 milliseconds (:math:`1/43907`),
collecting data on any watched channels,
averaging watched channels at the programmed interval,
watching the input buffer (for user commands such as "``?ai 1``"), 
and then processing the user command (if received).

Once we start watching a channel, the effective sample rate is changed:

>>> !ai:watch 1
Ok
>>> ?rate
7327

The the main loop of the program is starting every 0.136 milliseconds 
now (:math:`1/7327`) as the sketch collects the analog signal and 
periodically averages the values.

The values are averaged on a constant period (:ref:`get_period`) 
which defaults to 1 second (1000 ms):

>>> ?t
1000

Read the time-averaged (1 second) value:

>>> ?ai:mean 1
309074

This value is the average (mean) value, scaled by a *multiplier* (:ref:`get_k`).
By default, the multiplier is 1000 (so the signal is 309074 mVDC):

>>> ?k
1000

A multiplier is used so the value returned is an integer (as required by 
some common control software such as EPICS [#]_).  If the multiplier
were set to one, then the effect of averaging would be lost by removing the 
decimal fraction.  Thus, a multiplier was introduced for convenience.

You can change the multiplier to 10 to truncate the averages at 0.1 VDC:

>>> !k 10
Ok
>>> ?ai:mean 1
3090

.. [#] effective sampling rate (:ref:`get_rate`): number of times per 
   second each analog channel is read
.. [#] EPICS: :ref:`epics`

Start averaging a second channel:

>>> !ai:watch 0
Ok
>>> ?rate
4030

The main loop of the program is starting every 0.248 milliseconds 
now (:math:`1/4030`) as the sketch collects two analog signals and 
periodically averages their values.

Measured scan rates
-----------------------

These are the update rates and cycle times I measured on one
of my boards (OSEPP Uno).

===================  ===================   ==================
# channels watched   updates per second    cycle time, ms
===================  ===================   ==================
0                    43907                 0.023
1                    7327                  0.136
2                    4030                  0.248
3                    2838                  0.352
4                    2153                  0.464
5                    1736                  0.576
6                    1451                  0.689
===================  ===================   ==================

This means that if to watch all 6 available channels (:ref:`num_ai`),
the shortest averaging period (:ref:`set_period`) is 1 ms.  This will get
only one sample.  The more reasonable shortest time is 10 ms, which will get
about 14 samples per average.  Given that communications with a computer host
also takes some time, an averaging period of 100 ms might be the shortest 
useful period, with 140 samples.

.. tip::  To maximize the number of samples contributing to the average,
   only watch the channels you want to average.
