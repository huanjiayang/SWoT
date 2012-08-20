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
import requests
import json





#ser = serial.Serial('COM12',baudrate=115200,parity=serial.PARITY_NONE,stopbits=serial.STOPBITS_TWO,bytesize=serial.SEVENBITS)  # open first serial port
#print ser.portstr       # check which port was really used
##data = ser.read(10)
#data = str(ser.readline());
##line = ser.readline()
#print data     # write a string
#ser.close()             # close port














        
    


#ser = serial.Serial(0)  # open first serial port
try:
        ser = serial.Serial('COM12',115200,timeout=1,parity=serial.PARITY_NONE,
              stopbits=serial.STOPBITS_ONE,
              bytesize=serial.EIGHTBITS,
              )
        print ser.portstr 
#        ser.open()
except:
        print "Could not open serial port: ", sys.exc_info()[0]
        sys.exit(2)

print ser.portstr       # check which port was really used
#ser.write("hello")      # write a string

while True:
    ser.flushInput()
    data = str(ser.read(100).strip());
    data = data.replace("\n","").replace("\r","")
    data = data.split(",")
    try:
        
        if len(data) > 0:
            data.append(('timestamp:', time.asctime()))
            print 'Got:', data
  
        
#        data = int("data",16)
#        ADC_value = raw_value / 10
#        temperature = (ADC_value - 500) / 10                               
#        time.sleep(0.5)
        
#        data = struct.unpack('>fff', data)
#        data = struct.unpack('B', data)

        print "data: ", data
        
        
      

#        print 'not blocked'


 


    except KeyboardInterrupt:
        break


ser.close()

