.. $Id$

.. _IOC.commands:

==========================
EPICS IOC startup commands
==========================

.. caution:: TODO

   * describe how to configure the IOC for this
   * describe the prerequisites!

Make these two soft links in the ``$(TOP)/iocBoot/$(IOC)`` directory::

  ln -s ~/sketchbook/cmd_response/epics/ ./cmd_response
  ln -s ./cmd_response/cmd_response.proto ./

Make this addition to the ``$(TOP)/iocBoot/$(IOC)/st.cmd`` file,
replacing (or merging with if necessary) any ``asyn`` serial port setup::

  # Load Arduino support
  < cmd_response/cmd_response.cmd
  cd $(TOP)/iocBoot/$(IOC)

In the file ``cmd_response/cmd_response.cmd``, modify the path in this line
to fit your installation details, such as this changeset 
(``-``: line removed, ``+``:line added)::

  -epicsEnvSet("CMD_RESPONSE","/home/oxygen/JEMIAN/sketchbook/cmd_response/")
  +epicsEnvSet("CMD_RESPONSE","/home/prjemian/sketchbook/cmd_response/epics")

In the same file, modify the IOC prefix to fit, such as this changeset::

  -dbLoadRecords("$(CMD_RESPONSE)/cmd_response.db","P=prj:cr:,PORT=usb0")
  +dbLoadRecords("$(CMD_RESPONSE)/cmd_response.db","P=como:cr:,PORT=usb0")



file: ``cmd_reponse.cmd``
-------------------------

The file is too large for this documentation.

:EPICS IOC startup commands:
   :download:`cmd_response.cmd <../../../epics/cmd_response.cmd>`
