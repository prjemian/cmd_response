.. $Id$

==============================================
Startup
==============================================

On startup, the device will output a message such
as this to the USB port::

  cmd_response started: 7113

This message (from an Arduino Mega2560) signifies 
the device is *running* and the **cmd_response**
interface code is ready to receive a 
command and return a response.
The number ``7113`` is the amount of free 
SRAM available on startup.  On the Uno R3, the number
is ca. 1700, meaning there is still plenty of
SRAM available.

If this startup message does not appear.
the device is not ready for communications
and something is wrong.  The logical list
to check starts with the values in the 
Communication Parameters (given
in :ref:`tables`).  If the problem persists,
check the external host's USB port,
the FTDI driver on the external host, 
read/write permissions to the USB port, and
the USB cable.  If the problem still persists,
question that the Arduino is running 
the **cmd_response** code.


.. _basics:

Basic Operations
-----------------

The basic capabilities common to all Arduino boards
consist of:

* a number of digital (binary) input/output (configurable) channels
* some of the digital channels have pulse-width modulation output capability
* a number of analog input channels (10-bit analog-to-digital converter)

The commands which support these basic operations are:
:ref:`num_ai`, :ref:`num_bi`, :ref:`get_ai`, 
:ref:`get_bi`, :ref:`set_pin`, :ref:`set_bo`,
and :ref:`set_pwm`.


.. _averaging:

Signal Averaging
----------------

It takes a few dozen [#]_ microseconds to read each
analog input channel, much faster than typical 
communications over USB to request and retrieve 
a value.  Averaging the analog values over a 
fixed period makes sense.  

.. [#] The actual time to convert the ADC signal
   is dependent on several factors, including
   the signal level itself.  For more information,
   see: http://arduino.cc/en/Reference/AnalogRead
   
   On the Mega2560, the rate with no watched channels
   was reported as 25841/s.  Watching just one 
   channel, the reported rate dropped to 6567/s.  
   For two channels, the reported rate was 3781/s.

The signal averages are available from the
command interface with :ref:`ai_mean`, 
in addition to the 
instantaneous values available with the 
:ref:`get_ai` command.  However, since it takes
valuable time to read these analog signals, 
only the desired channels should be averaged, 
to optimize for speed.  

.. note:: A channel must be 
   configured (with :ref:`watch_ai`) 
   before calling :ref:`ai_mean`.

The average for each channel is obtained by
accumulating measurements of the analog input,
:math:`a_i` until the update period has elapsed,

:math:`(k/n) \sum_i^n a_i`.

The total number of values accumulated during
each period, :math:`n`, is combined with the
period length, :math:`t` to compute the update
rate (returned by :ref:`get_rate`),

:math:`n / t`.

The average value has higher precision than the
instantaneous value.  Exactly how much higher
is empirical.  Some external control systems
expect *raw* (integer) values from I/O controllers.
With such raw numbers, the external control system
will apply automatically pre-configured scaling 
factors to place the number into engineering units, 
such as *VDC*.

Here, a positive multiplier, :math:`k`, is used
to scale the average value so that its limit 
of precision can be expressed as a *long* integer.
To request the value of :math:`k`, use :ref:`get_k`.
To request the value of :math:`k`, use :ref:`set_k`.
Limits of :math:`k` are found by requesting
:ref:`get_k_min` and :ref:`get_k_max`.

All *watched* channels are accumulated during each
period.  The averages (for just the watched channels)
and update rate are 
recomputed at the end of each period.


Configuration
----------------

It can be useful for an external control system 
to verify which controller is communicating.
Two commands are available to identify
this software (:ref:`get_id`) and 
version (:ref:`get_version`).
