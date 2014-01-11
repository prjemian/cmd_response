.. $Id$

.. _streams.protocol:

==================================
Streams protocol
==================================

The Arduino is interfaced to EPICS through a USB connection (with *asyn* [#]_)
and uses the **cmd_response** sketch to define the interface language.
Software must be written in EPICS to communicate in this new language.
One easy method is to develop a Stream Device protocol. [#]_

The communication between the Arduino and the EPICS IOC 
goes through several layers,  Expanding part of the 
technology stack shown earlier:

* EPICS IOC software

 * EPICS IOC startup file
 * EPICS database
 * Stream Device protocol
 * *asyn*
 * Linux computer

* USB
* Arduino

 * **cmd_response** sketch

Using Stream Device, there is much less work to do than developing
traditional EPICS device support (such as [#]_).  
See this *Serial Stream Device* tutorial [#]_ for more details.

.. [#] The EPICS *asyn* package is used to communicate with the USB device.
.. [#] EPICS Stream Device: http://epics.web.psi.ch/software/streamdevice/
.. [#] "Very Simple Example of EPICS Device Support": http://www-linac.kek.jp/epics/second/
.. [#] *Serial Stream Device* tutorial: http://www.aps.anl.gov/epics/modules/soft/asyn/HowToDoSerial_StreamDevice.html


protocols
---------

The support consists of a Stream protocol file.  The protocol file
implements the desired **cmd_response** language commands
so they may be access by an EPICS database file (see :ref:`pv.database`).

Each of these protocols maps one (or more) **cmd_response** commands.

=============  =====================================================
protocol       meaning
=============  =====================================================
ai(pin)        read ``analogRead(pin)`` into the record's RVAL
ai_mean(pin)   read time-averaged ``analogRead(pin)`` into RVAL
bi(pin)        read ``digitalRead(pin)`` into the record's VAL
bo(pin)        write ``digitalWrite(pin)`` from the record's VAL
pwm(pin)       write ``analogWrite(pin)`` from the record's RVAL
period         read the current averaging period, ms
rate           read the number of samples/second
=============  =====================================================

.. note:: For the ``bo`` and ``pwm`` protocols, the selected
   pin will be configured by that protocol for output during
   record initialization.

.. _streams_ai:

``ai(pin)``
-------------

:purpose: read ``analogRead(pin)`` into the record's RVAL
:example: 
   ::
   
     record(ai, "A0:raw") {
     	 field(DTYP, "stream")
     	 field(INP,  "@cmd_response.proto ai(0) $(PORT)")
     	 field(AOFF, "0")
     	 field(ASLO, "0.004887585532746823069403714565")  # 5 VDC / 1023 ADC units
     	 field(HOPR, "5")
     	 field(LOPR, "0")
     }

In the example, the DTYP field indicates a stream protocol will be used.
The INP field says to find protocol ``ai(pin)`` (where pin = 0)
in file ``cmd_response.proto``.  The ``$(PORT)`` is passed to the database
from the IOC startup file and tells *Streams* which *asyn* port to use.

Since the value returned from the Arduino is an integer, it needs to be
scaled to the range of 0 .. 5 VDC.  The analog input channels (a.k.a., ADC:
analog-digital converter) on the Arduino are 10-bit.  They may have
a value from 0 to 1023.  Thus, an ADC value of 1023 represents a 
reading of a 5 VDC signal.

.. math::  \mbox{ASLO} = {5 \mbox{ VDC} \over 1023 \mbox{ ADC units}}

The ASLO field should be set at zero since these integers will 
never represent a negative voltage.  Also HOPR and LOPR are the EPICS
display limits (5 and 0 VDC, respectively).

.. _streams_ai_mean:

``ai_mean(pin)``
------------------

:purpose: read time-averaged ``analogRead(pin)`` into the record's RVAL
:example: 
   ::
   
     record(ai, "A0") {
     	 field(DTYP, "stream")
     	 field(INP,  "@cmd_response.proto ai_mean(0) $(PORT)")
     	 field(AOFF, "0")
     	 field(ASLO, "0.000004887585532746823069403714565")  # 5 VDC / 1023 ADC units / 1000 multiplier
     	 field(HOPR, "5")
     	 field(LOPR, "0")
     }

Similar to :ref:`ai(pin) <streams_ai>`, the computation must also
account for the multiplier, :math:`k`.  Here we assume the default
value of 1000.

The stream protocol also commands the Arduino
(during its initialization) to start watching
channel ``pin``, to take time averages.

.. TODO: must we describe _each_ command?


file: ``cmd_reponse.proto``
------------------------------

The file is too large for this documentation.

:EPICS Streams protocol:
   :download:`cmd_response.proto <../../../epics/cmd_response.proto>`
