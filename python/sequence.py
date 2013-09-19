#!/usr/bin/env python

'''
Read the serial port for any output from the
Arduino.  Print any output to stdout.
'''

import time
import serial	# easy_install -U pyserial

ARDUINO_SERIAL_PORT = '/dev/ttyUSB0'
SERIAL_PORT_BAUD = 115200
PERIOD_ms = 50
SLEEP_TIME_S = 0.003*PERIOD_ms

class Cmd_Response(object):

  def __init__(self, serial_port, baud=115200):
    self.serial_port = serial_port
    self.baud = baud
    self.port = serial.Serial(serial_port, baud)

  def command(self, cmd):
    self.port.write('%s\n' % cmd)

  def response(self):
    return self.port.readline().strip()
  
  def command_response(self, cmd):
    self.command(cmd)
    print "%s  " % cmd,
    print self.response()


def command(port, cmd):
  port.write('%s\n' % cmd)
  print "%s  " % cmd,
  response = port.readline().strip()
  if len(response) > 0:
    print response

def pwm11_ai0(port, pwm):
  command(port, '!pwm 11 %d' % pwm)
  time.sleep(SLEEP_TIME_S)
  command(port, '?ai 0')
  command(port, '?ai:mean 0')


#   port = serial.Serial(ARDUINO_SERIAL_PORT, SERIAL_PORT_BAUD)
#
#   for _ in ('id', 'v', 't', 'k', 'rate'):
#     command(port, '?%s' %  _)
#
#   command(port, '!pin 11 1')
#   command(port, '!ai:watch 0 1')
#   command(port, '!t %d' % PERIOD_ms)
#   command(port, '?rate')
#   command(port, '?ai 0')
#
#   time.sleep(0.2)
#   command(port, '?rate')
#   command(port, '?ai 0')
#   command(port, '?ai:mean 0')
#
#   for _ in (250, 2, 250, 2, 250, 2, 250):
#     print ""
#     pwm11_ai0(port, _)
#     for i in range(3):
#	time.sleep(SLEEP_TIME_S)
#	command(port, '?ai 0')
#	command(port, '?ai:mean 0')

cr = Cmd_Response(ARDUINO_SERIAL_PORT, SERIAL_PORT_BAUD)
cr.command_response('?id')
