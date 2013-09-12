
########### SVN repository information ###################
# $Date: 2013-07-20 09:13:52 -0500 (Sat, 20 Jul 2013) $
# $Author: jemian $
# $Revision: 1916 $
# $URL: http://svn.jemian.org/svn/regitte/arduino/led_cascade/led_cascade.cmd $
# $Id: led_cascade.cmd 1916 2013-07-20 14:13:52Z jemian $
########### SVN repository information ###################

# BEGIN serial.cmd for cmd_response  project ------------------------------------------------------------

epicsEnvSet("CMD_RESPONSE","/home/prjemian/sketchbook/cmd_response/")
cd $(CMD_RESPONSE)

# Set up 1 local serial port(s)

# USB 0 connected to Arduino at 9600 baud
#drvAsynSerialPortConfigure("portName","ttyName",priority,noAutoConnect, noProcessEos)
drvAsynSerialPortConfigure("usb0", "/dev/ttyUSB0", 0, 0, 0)
asynSetOption(usb0,0,baud,9600)

#asynOctetSetInputEos(const char *portName, int addr, const char *eosin,const char *drvInfo)
asynOctetSetInputEos("usb0",0,"\r\n")

# asynOctetSetOutputEos(const char *portName, int addr, const char *eosin,const char *drvInfo)
asynOctetSetOutputEos("usb0",0,"\r\n")

# Make port available from the iocsh command line
#asynOctetConnect(const char *entry, const char *port, int addr, int timeout, int buffer_len, const char *drvInfo)
asynOctetConnect("usb0", "usb0")

# Load asynRecord records on all ports
dbLoadTemplate("asynRecord.substitutions")

# send impromptu message to serial device, parse reply
# (was serial_OI_block)
dbLoadRecords("$(CMD_RESPONSE)/cmd_response.db","P=como:cmd_response:,PORT=usb0")

# END serial.cmd --------------------------------------------------------------
