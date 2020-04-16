.. _rpi_epics:

Installing EPICS on RaspberryPi
===============================

.. caution: work-in-progress

:date:
  2020-04-10
:RPi models:
  any (but older ones are much slower)
:requirements:
  16GB SD card (micro for most models), networking
:Raspberry Pi OS:
  Raspbian Buster
:optional:
  display, keyboard, mouse (only needed for initial setup)
:shell:
  ``/bin/bash``

This document is an update to http://prjemian.github.io/epicspi/

.. sidebar:: What is EPICS?

   For those who haven’t heard, EPICS (https://epics.anl.gov) is an
   open-source control system used worldwide for the routine operation and
   control of many particle accelerators such as FermiLab and SLAC, for the
   operation of scientific telescopes such as the Gemini and Keck
   telescopes, X-ray synchrotrons such as the Advanced Photon Source and
   the Diamond Light Source, neutron diffraction facilities such as the
   Spallation Neutron Source, and lots of other neat stuff. The system is
   scalable and runs on lots of different hardware. Here, we show you how
   to run EPICS on the Raspberry Pi!


Overview
--------

Here is how I installed the Experimental Physics and Industrial Control
System software `EPICS <https://epics.anl.gov/>`__ on the `Raspberry
Pi <https://www.raspberrypi.org>`__.

The EPICS software is a client/server system. To keep things simple, we
will run both the server and a client on the Raspberry Pi. (Clients on
other computers on our LAN can interact with our EPICS server but we
will not discuss that now.)

The EPICS **server** we will use is built in several parts:

-  `EPICS Base <https://epics.anl.gov/base/index.php>`__ provides all
   the development libraries and a few applications and utilities.
-  `EPICS synApps <https://www.aps.anl.gov/BCDA/synApps>`__ provides
   additional capabilities that will be useful in real projects. We only
   use a little of it here, though.

There are many, many possible EPICS **clients**. Since the RPi already
has Python, we’ll work with that:

-  `PyEpics <https://cars9.uchicago.edu/software/python/pyepics3/>`__ is
   an EPICS binding to the Python language, allowing us to build a
   simple client and interact with our server.

Prepare the RaspberryPi (RPi)
-----------------------------

1. OS: download latest OS image from one of the `Raspbian
   Mirrors <https://www.raspbian.org/RaspbianMirrors>`__
2. flash the OS to a 16GB microSD card using `Balena
   Etcher <https://www.balena.io/etcher/>`__ (or some other tool)
3. prepare the RPi
4. boot the RPi and configure it for networking on your LAN
5. update the OS per instructions
6. enable SSH access (via ``sudo raspi-config`` or GUI tool)

touchscreen rotation
~~~~~~~~~~~~~~~~~~~~

If you have the 7-inch touchscreen for your RPi, then you might want
to rotate the display.  Following
`advice <https://raspberrypiprojects.com/raspberry-pi-rotate-touch-screen/>`__

1. Open the Terminal screen.
2. Access the config file using ``sudo nano /boot/config.txt``
3. Enter a new line with the text ``lcd_rotate=2``
4. Press CONTROL+X then Y then ENTER to Save
5. Type ``sudo reboot`` to reboot and your LCD will be rotated!

For me, I paste these lines into the end of ``/boot/config.txt``:

::

   #--------------------------
   # 7" touch display screen rotation
   # default
   # lcd_rotate=0
   # rotate 90 degrees clockwise
   # lcd_rotate=3
   # rotate 180 degrees
   lcd_rotate=2
   # rotate 90 degrees counter clockwise
   # lcd_rotate=1

Install packages to build & manage EPICS
----------------------------------------

Here we describe how to install libraries, support packages, and other
tools to make it easier to manage an EPICS installation.

The *raspbian-buster* desktop distribution already has all the tools
necessary to build *EPICS Base*.

We’ll need additional libraries to compile `EPICS
synApps <https://www.aps.anl.gov/BCDA/synApps>`__ and `EPICS area
detector <https://cars9.uchicago.edu/software/epics/areaDetector.html>`__.

The `Microsoft Visual Studio Code
editor <https://pimylifeup.com/raspberry-pi-visual-studio-code/>`__ is a
great tool to have on hand. Really nice people have made it available
for the RPi.

additional libraries for EPICS synApps
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To build the *sequencer* module, the following package must be
installed. The *sequencer* module provides *State Notation Language*, a
C-like language to provide programming used by other *synApps* modules.

::

   sudo apt-get install re2c
   # you _might_ already have these installed, but to be sure...
   sudo apt-get install libnet-dev libpcap-dev libusb-1.0-0-dev

additional libraries for EPICS area detector
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

These packages must be added to build *GraphicsMagick* for 
*EPICS area detector*.

::

   sudo apt install libx11-dev libxext-dev

**NOTE** To learn about specific build requirements such as
GraphicsMagick, it was useful to search
`Debian <https://packages.debian.org/search?keywords=libgraphicsmagick>`__
for the package
`details <https://packages.debian.org/buster/libgraphicsmagick-q16-3>`__
which are specific to the arm architecture of the RPi.

Add logmake to .bash_aliases
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When I run ``make`` to compile software, it is convenient to write
any/all console output, including errors, to a file for later review. I
add this function definition my ``~/.bash_aliases`` file so it is always
available:

::

   function logmake
   {
       echo '#-----------------------------------------' >> makelog.txt 2>&1
       date >> makelog.txt 2>&1
       echo '#begin CMD: make' $@ >> makelog.txt 2>&1
       make $@ >> makelog.txt 2>&1
       echo '#done CMD: make' $@ >> makelog.txt 2>&1
       date >> makelog.txt 2>&1
   }

It also records time stamps so you can see how long the operation took.
It passes any command line arguments to the ``make`` command so you can
use it for ``logmake &`` or ``logmake rebuild &`` or ``logmake clean &``
or …

Install Microsoft Visual Studio Code editor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. tip:: This is optional, performance can be less than anticipated.

Following web
`guidance <https://pimylifeup.com/raspberry-pi-visual-studio-code/>`__,
both these installers worked for me (on separate RPi computers, of
course).

one-step installer
^^^^^^^^^^^^^^^^^^

::

       curl -L https://raw.githubusercontent.com/headmelted/codebuilds/master/docs/installers/apt.sh | sudo bash

two-step installer
^^^^^^^^^^^^^^^^^^

Install GPG keys:

::

   wget https://packagecloud.io/headmelted/codebuilds/gpgkey -O - | sudo apt-key add -

Install Visual Studio Code editor

::

   curl -L https://raw.githubusercontent.com/headmelted/codebuilds/master/docs/installers/apt.sh | sudo bash

Run *code*
^^^^^^^^^^

Once installed, start *code* with either:

-  desktop GUI: *Accessories* -> **Code-OSS**
-  terminal command line: ``code-oss``

Preparing for EPICS
-------------------

EPICS is flexible about where (into which directory path) it is placed.
Still, it helps to use standard locations. We’ll build it in a directory
called ``/usr/local/epics`` as user ``pi``. You’ll need to open a
terminal window:

::

   sudo mkdir /usr/local/epics
   sudo chown pi:pi /usr/local/epics
   cd /usr/local/epics

By making the *epics* directory in ``pi`` account, we will be able to
modify any of our EPICS resources without needing to gain higher
privileges.

Install EPICS base
------------------

*EPICS Base* is very easy to build.

All that is necessary is to define the host architecture (in
``EPICS_HOST_ARCH``, shown below) and then build it. First, download the
source. At this writing, the latest stable version of `EPICS
Base <https://epics.anl.gov/base/R7-0/3.php>`__ is 7.0.3.1:

::

   cd /usr/local/epics
   wget http://www.aps.anl.gov/epics/download/base/base-7.0.3.1.tar.gz
   tar xzf base-7.0.3.1.tar.gz
   # for convenience, make a soft link
   ln -s ./base-7.0.3.1 ./base

Make these definitions *before* you build:

::

   export EPICS_ROOT=/usr/local/epics
   export EPICS_BASE=${EPICS_ROOT}/base
   export EPICS_HOST_ARCH=`${EPICS_BASE}/startup/EpicsHostArch`

   export PATH=${PATH}:${EPICS_BASE}/bin/${EPICS_HOST_ARCH}
   # and for good measure (might not be needed)
   export PATH=${PATH}:${EPICS_BASE}/lib/${EPICS_HOST_ARCH}

We can check this value by printing it to the command-line:

::

   pi@raspberrypi:/usr/local/epics $ echo $EPICS_HOST_ARCH
   linux-arm

I make sure they are defined in my ``~/.bash_aliases`` file so they are
always defined when I start a command line. Adding to ``PATH`` puts
standard EPICS tools on the command line, such as: ``caget``, ``caput``,
``cainfo``, ``camonitor``, ``softIoc``

Next, compile EPICS base (write the build output, including any errors,
to a file using `logmake <#add-logmake-to-bashaliase>`__). If you
have a multi-CPU RPi, you can use more than one CPU to run make. (Careful! 
If you try this with older models, such as the RPi A, it will
just as likely force an automatic system crash.) My Raspberry Pi 2 Model B
v1.1 has four CPUs. Experience guides me to not use more than 2, so I
use ``logmake -j2 &``. My Raspberry Pi Zero W has one CPU. With this
guide, I show the single CPU build:

::

   cd /usr/local/epics/base
   logmake &
   tail -f makelog.txt

This process took different times on different RPi models (and ``make`` options) as shown:

================  =======  =============  ==========================================
model             options  time, minutes  ``grep "model name" /proc/cpuinfo``
================  =======  =============  ==========================================
Model 4 B 4GB     ``-j2``  18             ``ARMv7 Processor rev 4 (v7l)``
Model 4 B 4GB              32             ``ARMv7 Processor rev 4 (v7l)``
Model 3 B         ``-j2``  34             ``ARMv7 Processor rev 4 (v7l)``
2 Model B v1.1    ``-j2``  64             ``ARMv7 Processor rev 4 (v7l)``
2 Model B v1.1             110            ``ARMv7 Processor rev 4 (v7l)``
Zero W                     266            ``ARMv6-compatible processor rev 7 (v6l)``
================  =======  =============  ==========================================

(For comparison, this same process to build the same version of EPICS
base on a linux-x86_64 host with 8 cores at 1.6HGz/3.1GHz using
``logmake -j4 &`` took about 3 minutes.)

Wikipedia has a table describing the different RPi hardware:
https://en.wikipedia.org/wiki/Raspberry_Pi#Generations_of_released_models

Install synApps
---------------

Use the guidance from
`synApps/support <https://github.com/EPICS-synApps/support>`__ to
install the source code for synApps. A new feature is the ability to
name the directory into which the source code will be installed. We’ll
set that here and include both synApps and base version in the name (in
case we change either):

::

   cd /usr/local/epics
   export SYNAPPS_DIR=synApps_6.1__base.7.0.3.1
   wget https://raw.githubusercontent.com/EPICS-synApps/support/master/assemble_synApps.sh

Next, edit the file you just downloaded (``assemble_synApps.sh``) for
your version of EPICS base and local directory paths. You can comment
out any modules that are not obviously related to execution on the RPi
(such as the VME modules: *softGlue*, *softGlueZync*, *VME*, and perhaps
others)

.. raw:: html

   <details>

first part of my assemble_synApps.sh file:

**CAUTION**: This is guidance! Version numbers (even entire modules) may
have changed since this was written so please do not copy this verbatim.

::

   #not Rpi#ALLENBRADLEY=2.3
   #APS only#ALIVE=R1-1-1
   AREA_DETECTOR=master
   ASYN=R4-37
   AUTOSAVE=R5-10
   BUSY=R1-7-2
   CALC=R3-7-3
   #not Rpi#CAMAC=R2-7-1
   CAPUTRECORDER=R1-7-2
   #not Rpi#DAC128V=R2-9
   #not Rpi#DELAYGEN=R1-2-1
   #not Rpi#DXP=R6-0
   #not Rpi#DXPSITORO=R1-2
   DEVIOCSTATS=3.1.16
   #ETHERIP=ether_ip-3-1
   #GALIL=V3-6
   IP=R2-20-1
   IPAC=2.15
   #not Rpi#IP330=R2-9
   #not Rpi#IPUNIDIG=R2-11
   #not Rpi#LOVE=R3-2-7
   LUA=R2-0
   MCA=R7-8
   #not Rpi#MEASCOMP=R2-3
   MODBUS=R3-0
   MOTOR=R7-1
   OPTICS=R2-13-3
   #not Rpi#QUADEM=R9-2-1
   SNCSEQ=2.2.6
   #not Rpi#SOFTGLUE=R2-8-2
   #not Rpi#SOFTGLUEZYNQ=R2-0-2
   SSCAN=R2-11-3
   STD=R3-6
   STREAM=2.8.9
   #not Rpi#VAC=R1-9
   #not Rpi#VME=R2-9-2
   #not Rpi#YOKOGAWA_DAS=R2-0-1
   XXX=R6-1

Edit the location of EPICS_BASE (on or near line 34).

::

   EPICS_BASE=/usr/local/epics/base-7.0.3.1

The rest of that file remains untouched.

.. raw:: html

   </details>

Then, download and install the chosen synApps modules

::

   cd /usr/local/epics
   bash ./assemble_synApps.sh

Next, build *synApps*:

::

   cd /usr/local/epics/$(SYNAPPS_DIR)/support
   logmake &
   tail -f makelog.txt

This process took different times on different RPi models as shown:

==============  =======  =============  ==========================================
model           options  time, minutes  ``grep "model name" /proc/cpuinfo``
==============  =======  =============  ==========================================
Model 4 B 4GB   ``-j2``  TODO           ``ARMv7 Processor rev 4 (v7l)``
Model 3 B       ``-j2``  40             ``ARMv7 Processor rev 4 (v7l)``
2 Model B v1.1           TODO           ``ARMv7 Processor rev 4 (v7l)``
Zero W                   283            ``ARMv6-compatible processor rev 7 (v6l)``
==============  =======  =============  ==========================================

(For comparison, this same process to build the same version of EPICS
base on a linux-x86_64 host with 8 cores at 1.6HGz/3.1GHz using
``logmake -j4 &`` took about 3 minutes.)

Install PyEpics
---------------

It is possible to install & run the
`PyEpics <https://cars9.uchicago.edu/software/python/pyepics3/>`__
support from Matt Newville on the Raspberry Pi!

::

   sudo pip3 install pyepics

..
   TODO: What about ``libca`` and ``libCom``?

Test PyEpics - the EPICS interface
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Test** that PyEpics is available with this command. Here are my
results:

::

   pi@raspberrypi:~ $ python3 -c "import epics; print(epics.__version__, epics.__file__)"
   3.4.1 /usr/local/lib/python3.7/dist-packages/epics/__init__.py

To test that PyEpics actually talks with an EPICS IOC, we need to first
establish an IOC (EPICS server). We’ll use the *softIoc* executable from
EPICS base.

create EPICS database
^^^^^^^^^^^^^^^^^^^^^

First, create an EPICS database file ``simple.db``. An EPICS database
file is a plain text file that defines the process variables to be
available from the IOC.

::

   cat <<EOF > simple.db
   record(bo, "rpi:trigger")
   {
           field(DESC, "trigger PV")
           field(ZNAM, "off")
           field(ONAM, "on")
   }
   record(stringout, "rpi:message")
   {
           field(DESC, "message on the RPi")
           field(VAL,  "RPi default message")
   }
   EOF

**NOTE** The file ``simple.db`` defines two EPICS records:
``rpi:trigger`` and ``rpi:message``. The first record can take the value
of ``0`` or ``1``, which also have the string values of ``off`` and
``on``, respectively. The second record is text for a readable message.

start EPICS IOC
^^^^^^^^^^^^^^^

Next, run the EPICS soft IOC support with this database:

::

   pi@raspberrypi:~ $ softIoc -d simple.db
   dbLoadDatabase("/usr/local/epics/base-7.0.3.1/bin/linux-arm/../../dbd/softIoc.dbd")
   softIoc_registerRecordDeviceDriver(pdbbase)
   dbLoadRecords("simple.db")
   iocInit()
   Starting iocInit
   ############################################################################
   ## EPICS R7.0.3.1
   ## EPICS Base built Feb 21 2020
   ############################################################################
   iocRun: All initialization complete
   epics> 

monitor EPICS PVs
^^^^^^^^^^^^^^^^^

We’ll watch the EPICS process variables (PVs) for any changes in their
values using ``camonitor``, an executable from EPICS base. Open a
separate terminal window, and enter the ``camonitor`` command below:

::

   pi@raspberrypi:~ $ camonitor rpi:trigger rpi:trigger.DESC rpi:message rpi:message.DESC
   rpi:trigger                    <undefined> off UDF INVALID
   rpi:trigger.DESC               <undefined> trigger PV UDF INVALID
   rpi:message                    <undefined> RPi default message UDF NO_ALARM
   rpi:message.DESC               <undefined> message on the RPi UDF NO_ALARM

PyEpics program to write the PVs
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Next, in a new terminal, we write a short PyEpics program, ``test.py``,
to communicate with the PVs of the softIoc.

::

   cat <<EOF > test.py
   #!/usr/bin/env python3

   import epics

   print(epics.caget('rpi:trigger.DESC'))
   print(epics.caget('rpi:trigger'))
   print(epics.caget('rpi:message.DESC'))
   print(epics.caget('rpi:message'))

   epics.caput('rpi:message', 'setting trigger')
   epics.caput('rpi:trigger', 1)
   print(epics.caget('rpi:trigger.DESC'))
   print(epics.caget('rpi:trigger'))
   print(epics.caget('rpi:message.DESC'))
   print(epics.caget('rpi:message'))

   epics.caput('rpi:message', 'clearing trigger')
   epics.caput('rpi:trigger', 0)
   print(epics.caget('rpi:trigger.DESC'))
   print(epics.caget('rpi:trigger'))
   print(epics.caget('rpi:message.DESC'))
   print(epics.caget('rpi:message'))
   EOF

Make that python program executable:

::

   chmod +x test.py

Next, run that python program:

::

   pi@raspberrypi:~ $ ./test.py 
   trigger PV
   0
   message on the RPi
   RPi default message
   trigger PV
   1
   message on the RPi
   setting trigger
   trigger PV
   0
   message on the RPi
   clearing trigger

Note these new messages printed on the terminal running ``camonitor``:

::

   rpi:message                    2020-04-11 13:55:37.454539 setting trigger  
   rpi:trigger                    2020-04-11 13:55:37.455587 on  
   rpi:message                    2020-04-11 13:55:37.462112 clearing trigger  
   rpi:trigger                    2020-04-11 13:55:37.463091 off  

Delimiters: Parentheses, Braces, and Back-Quotes
------------------------------------------------

In the code examples above, a combination of parentheses, braces, and
back-quotes (a.k.a. accent grave or backtick) are used.

In the ``/bin/bash`` shell, braces, ``{`` and ``}``, are used to delimit
the scope of symbol names during shell expansion. In the code examples
above, the delimiters are probably unnecessary. Using these delimiters
is a cautious practice to adopt. Parentheses are not recognized in this
context:

::

   ~$ echo $EPICS_ROOT
   /usr/local/epics
   ~$ echo ${EPICS_ROOT}
   /usr/local/epics
   ~$ echo $(EPICS_ROOT)
   EPICS_ROOT: command not found

However, in the various files and commands that configure and command
the EPICS components, parentheses, ``(`` and ``)``, are the required
delimiters. See these examples from above:

::

   #AREA_DETECTOR=$(SUPPORT)/areaDetector-1-8beta1
   #IP=$(SUPPORT)/ip-2-13

Sometimes, in a shell script, it is necessary to assign a variable with
the value obtained from a command line tool. One common way to do that,
shared by ``bash`` and some other shells such as ``tcsh``, is to enclose
the command line tool with the ``\``` back-quote character. See this
example:

::

   ~$ echo $SHELL
   /bin/bash
   ~$ echo `/usr/local/epics/base-3.14.12.3/startup/EpicsHostArch`
   linux-x86_64

An alternative way to do this assignment in ``bash`` was pointed out, to
use shell expansion with parentheses as the delimiters, such as:

::

   ~$ echo $(/usr/local/epics/base-3.14.12.3/startup/EpicsHostArch)
   linux-x86_64

References
----------

Software
~~~~~~~~

=====================  ==================================
software               URL
=====================  ==================================
Balena Etcher          https://www.balena.io/etcher
EPICS                  https://epics.anl.gov/
EPICS area detector    https://cars9.uchicago.edu/software/epics/areaDetector.html
EPICS synApps          https://www.aps.anl.gov/BCDA/synApps
PyEpics                https://cars9.uchicago.edu/software/python/pyepics3/
Raspbian OS            https://www.raspbian.org
Raspbian OS Mirrors    https://www.raspbian.org
Raspberry Pi           https://www.raspberrypi.org
VSCode                 https://pimylifeup.com/raspberry-pi-visual-studio-code/
Touchscreen rotation   https://raspberrypiprojects.com/raspberry-pi-rotate-touch-screen/
=====================  ==================================


Hardware
~~~~~~~~

References for the Raspberry Pi models shown above:

==============  =================
hardware        URL
==============  =================
Model 4 B 4GB   https://www.raspberrypi.org/products/raspberry-pi-4-model-b/
Model 3 B       https://www.raspberrypi.org/products/raspberry-pi-3-model-b/
2 Model B v1.1  https://www.raspberrypi.org/products/raspberry-pi-2-model-b/
Zero W          https://www.raspberrypi.org/products/raspberry-pi-zero-w/
Wikipedia       https://en.wikipedia.org/wiki/Raspberry_Pi#Generations_of_released_models
==============  =================

Facilities
~~~~~~~~~~

These facilities are some of the many that use EPICS (https://epics-controls.org/):

==============================  ===================
EPICS facility                  URL
==============================  ===================
Advanced Photon Source          https://www.aps.anl.gov
Diamond Light Source            https://diamond.ac.uk
FermiLab                        https://fnal.gov
Gemini telescope                https://gemini.edu
Keck telescope                  https://keckobservatory.org
SLAC                            https://slac.standford.edu
Spallation Neutron Source       https://neutrons.ornl.gov/sns
==============================  ===================
