'''
Created on 16 Jul 2012

@author: AYODELE-M.AKINGBULU
'''

import os 
import sys 
import threading 
import time 
from time import sleep
import socket
import serial
import struct
import matplotlib
import re

## The default size of this message type in bytes.
#DEFAULT_MESSAGE_SIZE = 28
#
## The Active Message type associated with this message.
#AM_TYPE = 147

PORT = '/dev/ttyUSB1'
BAUD_RATE = 115200



#ser = serial.Serial(0)  # open first serial port
try:
        ser = serial.Serial('com4',9600,timeout=1)
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        ser = serial.Serial(PORT,BAUD_RATE,
              parity=serial.PARITY_ODD,
              stopbits=serial.STOPBITS_TWO,
              bytesize=serial.SEVENBITS,
              )
        ser.open()
except:
        print "Could not open serial port: ", sys.exc_info()[0]
        sys.exit(2)

print ser.portstr       # check which port was really used
#ser.write("hello")      # write a string

while True:
    try:
        data = str(ser.readline());
        data = data.replace("\n","").replace("\r","")
        data = data.split(",")
        if len(data) > 0:
            print 'Got:', data
    

        time.sleep(0.5)
        print ser.readline()
        print 'not blocked'

        ser.close()

 




def convert_temperature():

    f = float(raw_input("Enter temperature to be converted to degrees Celsius."))

    temp = (f - 32) * (5.0 / 9.0)

    return "temperature in degree Celsius is %d." % temp

#class Message():
#
#    def __init__(self, msg, data="", addr=None, gid=None, base_offset=0, data_length=28):
#        
#        
        