########### SVN repository information ###################
# $Date$
# $Author$
# $Revision$
# $URL$
# $Id$
########### SVN repository information ###################

# BEGIN serial.cmd for cmd_response  project ------------------------------------------------------------

#epicsEnvSet("CMD_RESPONSE","/home/prjemian/sketchbook/cmd_response/epics")
#cd $(CMD_RESPONSE)

# Set up 1 local serial port(s)

# USB 0 connected to Arduino at 115200 baud
#drvAsynSerialPortConfigure("portName","ttyName",priority,noAutoConnect, noProcessEos)
drvAsynSerialPortConfigure("usb0", "/dev/ttyUSB0", 0, 0, 0)
asynSetOption(usb0,0,baud,115200)

#asynOctetSetInputEos(const char *portName, int addr, const char *eosin,const char *drvInfo)
asynOctetSetInputEos("usb0",0,"\r\n")

# asynOctetSetOutputEos(const char *portName, int addr, const char *eosin,const char *drvInfo)
asynOctetSetOutputEos("usb0",0,"\n")

# Make port available from the iocsh command line
#asynOctetConnect(const char *entry, const char *port, int addr, int timeout, int buffer_len, const char *drvInfo)
asynOctetConnect("usb0", "usb0")

# Load asynRecord records on all ports
dbLoadTemplate("asynRecord.substitutions")

# send impromptu message to serial device, parse reply
# (was serial_OI_block)
#dbLoadRecords("$(CMD_RESPONSE)/cmd_response.db","P=$(IOC_PREFIX)cr:,PORT=usb0")
dbLoadRecords("cmd_response.db","P=$(IOC_PREFIX)cr:,PORT=usb0")

# END serial.cmd --------------------------------------------------------------
