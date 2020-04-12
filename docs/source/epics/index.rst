.. _epics:

CONTENTS

.. toctree::
   :maxdepth: 2
   :glob:
   
   installing
   streams
   database
   ioc
   cssboy
   epid

.. index:: !EPICS constant lighting

Example: Constant Lighting with EPICS
###############################################

.. sidebar::  EPICS jargon approaching!

   Now we start to introduce some of the terms used in EPICS,
   such as *record*, *database*, *asyn*, and *Streams*.
   We'll try to explain the important ones as we go along.
   
   For more details, you might consider the first
   lecture or two in this series: 
   "Getting Started with Epics Lecture Series at APS",
   http://www.aps.anl.gov/epics/docs/GSWE.php
   
   :EPICS: 
      Experimental Physics and Industrial Control System,
      http://www.aps.anl.gov/epics


Our goal here is to integrate the Arduino into EPICS
using the **cmd_response** and demonstrate it as a 
sensor interface using PID controls.  
We'll make *constant lighting* (maintain a constant level of lighting).
Is this a useful thing to do?  It's useful here to demonstrate
how PID works in a very safe way; no moving parts, all contacts 
are low voltage and pose no significant electrical hazard.

In our earlier Python example (see :ref:`python_sequence`),
the live information from the Arduino was only available
to the Python program.  In "real world" situation, control system
information may come from many sources and be used for more than
one purpose at the same time (such as: logging, operator visualization, 
process control, web page).  
EPICS is scalable, allowing for multiple servers and clients,
and can easily integrate the signals
from the Arduino as part of such a control system.

.. index:: !technology stack

An EPICS system consists of many layers of components.
Let's visualize this *technology stack* with the Arduino at the root:

.. TODO: make this a figure?  No, the list is just as clear.  KISS applies.

.. index:: 
   single: EPICS; IOC
   single: EPICS; OPI
   single: EPICS; Channel Access

* you
* EPICS OPI (operator/programmer interface) client computer(s)
* EPICS Channel Access (over local computer network)
* EPICS IOC (input/output controller) server computer
* USB
* Arduino

In previous examples, we 
described the components at the bottom of this technology 
stack (*Arduino* and *USB*).
Moving upwards, our EPICS interface (where we integrate
the Arduino with an EPICS IOC) consists of three files:

* :ref:`installing.epics`
* :ref:`streams.protocol`
* :ref:`pv.database`
* :ref:`IOC.commands`

Then we describe the tool to simplify observation of this system:

* :ref:`cssboy`

Finally, we describe the PID controls:

* :ref:`epid_example`
