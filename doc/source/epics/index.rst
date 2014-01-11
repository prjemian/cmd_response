.. $Id$

.. index:: EPICS

.. _epics:

========================
Example: EPICS interface
========================

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

An EPICS system consists of many layers of components.
Let's visualize this technology stack with the Arduino at the root:

.. TODO: make this a figure

* you
* EPICS OPI (operator/programmer interface) client computer(s)
* EPICS Channel Access (over local computer network)
* EPICS IOC (input/output controller) server computer
* USB
* Arduino

One strength of EPICS is that it is scalable.
It allows more than one client to view the same 
data at the same time (multiple OPI).
It also allows for multiple IOC servers.

Our goal here is to integrate the Arduino into EPICS
using the **cmd_response** and demonstrate it as a 
sensor interface using PID controls.  We already begun
describing of this technology stack, starting at the bottom.
Moving upwards, our EPICS interface (where we integrate
the Arduino with an EPICS IOC) consists of three files:

* :ref:`streams.protocol`
* :ref:`pv.database`
* :ref:`IOC.commands`

Contents
--------

.. toctree::
   :maxdepth: 1
   :glob:
   
   streams
   database
   ioc
   cssboy
   epid
