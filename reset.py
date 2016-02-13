#!/usr/bin/env python
import sys
import subprocess
import serial
import argparse
import time

# prepare argument parser to understand -P PORT
parser = argparse.ArgumentParser()
parser.add_argument("-P", dest="port")
# parse only known parameters
args = parser.parse_known_args()

# if port argument is present perform soft reset
if args[0].port:
  try: # try to initiate serial port connection on PORT with 1200 baudrate
    ser = serial.Serial(
      port=args[0].port,
      baudrate=1200,
      parity=serial.PARITY_NONE,
      stopbits=serial.STOPBITS_ONE,
      bytesize=serial.EIGHTBITS
    )
  except serial.SerialException, e:
    print "pySerial error: " + str(e) + "\n"
    sys.exit(1)

  try: # try to open PORT
    ser.isOpen()
    time.sleep(0.5)
  except serial.SerialException:
    print "pySerial error: " + str(e) + "\n"
    sys.exit(1)

  # and close it immediately signaling to bootloader that flashing is imminent
  print "Success... closing"
  time.sleep(0.5)
  ser.close()
  print "...done"

else:
    print "No args..."
