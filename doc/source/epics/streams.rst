.. $Id$

.. _streams.protocol:

==================================
Streams protocol
==================================


=============  ================================================
protocol       meaning
=============  ================================================
ai(pin)        read analogRead(pin) into the record's RVAL
ai_mean(pin)   read average analogRead(pin) into RVAL
bi(pin)        read digitalRead(pin) into the record's VAL
bo(pin)        write digitalWrite(pin) from the record's VAL
pwm(pin)       write analogWrite(pin) from the record's RVAL
period         read the current averaging period, ms
rate           read the number of samples/second
=============  ================================================

.. note:: For the ``bo`` and ``pwm`` protocols, the selected
   pin will be configured by that protocol for output during
   record initialization.

* describe what to do with this (nothing)

----------


:EPICS Streams protocol:
   :download:`cmd_response.proto <../../../epics/cmd_response.proto>`

..
  .. literalinclude:: ../../../epics/cmd_response.proto
      :language: c++
      :linenos:

