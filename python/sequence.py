#!/usr/bin/env python

'''
Read the serial port for any output from the
Arduino.  Print any output to stdout.
'''

import time
import serial	# easy_install -U pyserial

ARDUINO_SERIAL_PORT = '/dev/ttyUSB0'
SERIAL_PORT_BAUD = 115200
PERIOD_ms = 100
SLEEP_TIME_S = 0.003*PERIOD_ms
REPETITIONS = 10

class Cmd_Response(object):

  def __init__(self, serial_port, baud=115200):
    self.serial_port = serial_port
    self.baud = baud
    self.port = serial.Serial(serial_port, baud)

  def send(self, cmd):
    self.port.write('%s\n' % cmd)

  def receive(self):
    return self.port.readline().strip()
  
  def request(self, cmd):
    self.send(cmd)
    return self.receive()
  
  def report(self, cmd):
    print "%s  " % cmd,
    print self.request(cmd)


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


cr = Cmd_Response(ARDUINO_SERIAL_PORT, SERIAL_PORT_BAUD)
cr.request('!t %d' % PERIOD_ms)
cr.request('!ai:watch 0 1')
cr.request('!pin 11 1')

if False:
  for _ in ('?id', '?v', '?t', '?k', '?rate'):
    cr.report(_)
 
  time.sleep(SLEEP_TIME_S)
  rate = cr.request('?rate')
  print rate
  print cr.request('?t')
  print cr.request('?ai 0')
  print cr.request('?ai:mean 0')

c = 0
for pwm in range(0, 256, 5):
  cr.request('!pwm 11 %d' % pwm)
  time.sleep(SLEEP_TIME_S)
  for _ in range(REPETITIONS):
    c += 1
    time.sleep(PERIOD_ms*0.0011)
    ai = int(cr.request('?ai 0'))
    mean = float(cr.request('?ai:mean 0'))/1000.
    print "%d %d %d %.3f" % (c, pwm, ai, mean)
