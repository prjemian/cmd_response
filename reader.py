#!/usr/bin/env python

'''
Read the serial port for any output from the
Arduino.  Print any output to stdout.
'''

import time
import serial	# easy_install -U pyserial

ARDUINO_SERIAL_PORT = '/dev/ttyUSB0'
SERIAL_PORT_BAUD = 115200
ARDUINO_PORT = None
SLEEP_TIME_S = 0.05

def reader(port):
    response = port.readline().strip()
    if len(response) > 0:
        print response
    time.sleep(SLEEP_TIME_S)

def main():
  ARDUINO_PORT = serial.Serial(ARDUINO_SERIAL_PORT, SERIAL_PORT_BAUD)
  while True:
    reader(ARDUINO_PORT)

if __name__ == '__main__':
    main()
