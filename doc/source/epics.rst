.. $Id$

==================================
EPICS interface
==================================

The EPICS interface consists of three files:

* :ref:`streams.protocol`
* :ref:`pv.database`
* :ref:`IOC.commands`
  
Configure the IOC by ...

.. TODO: finish this


.. _streams.protocol:

Streams protocol
--------------------

* describe what to do with this (nothing)

.. literalinclude:: ../../epics/cmd_response.proto
    :language: c++
    :linenos:


.. _pv.database:

EPICS PV database
--------------------

* describe how to configure this

.. literalinclude:: ../../epics/cmd_response.db
    :linenos:


.. _IOC.commands:

EPICS IOC startup commands
----------------------------------------

* describe how to configure the IOC for this
* describe the prerequisites!

.. literalinclude:: ../../epics/cmd_response.cmd
    :language: bash
    :linenos:
