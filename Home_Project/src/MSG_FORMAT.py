'''
Created on 16 Jul 2012

@author: AYODELE-M.AKINGBULU
'''

import os 
import sys 
import threading 
import time 
from time import sleep
import socket as sock
import serial
import struct
import matplotlib
import re

# The default size of this message type in bytes.
DEFAULT_MESSAGE_SIZE = 28

# The Active Message type associated with this message.
AM_TYPE = 147




#ser = serial.Serial(0)  # open first serial port
try:
        ser = serial.Serial('com4',9600,timeout=1)
        ser = serial.Serial(
              port='/dev/ttyUSB1',
              baudrate=9600,
              parity=serial.PARITY_ODD,
              stopbits=serial.STOPBITS_TWO,
              bytesize=serial.SEVENBITS,
              )
except:
        print "Could not open serial port: ", sys.exc_info()[0]
        sys.exit(2)

print ser.portstr       # check which port was really used
#ser.write("hello")      # write a string

while True:
    data = ser.read(9999)
    if len(data) > 0:
        print 'Got:', data

    time.sleep(0.5)
    print ser.readline()
    print 'not blocked'

ser.close()

 


#class Message():
#
#    def __init__(self, msg, data="", addr=None, gid=None, base_offset=0, data_length=28):
#        
#        
        