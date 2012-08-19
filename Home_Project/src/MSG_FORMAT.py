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

## The default size of this message type in bytes.
#DEFAULT_MESSAGE_SIZE = 28
#
## The Active Message type associated with this message.
#AM_TYPE = 147

PORT = '/dev/ttyUSB1'
BAUD_RATE = 115200

URL_HOME_SENSOR = 'http://localhost:8080/homesensorcom/'
MY_MSG_FORMAT_KEY = 'link from message format API'



        



class Message():

    def __init__(self, msg, data="", addr=None, gid=None, base_offset=0, data_length=28):
        pass

    def convert_temperature(self,adc, adc_fs=1023,value):
        self.value = float(10000 * (adc_fs - adc) / adc)
        self.value = float(raw_input(""))
        
  
        temperature = self.value
        #temperature = (value - 32) * (5.0 / 9.0)
        
    


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
        
        raw_value = int(data[2])
        ADC_value = raw_value / 10
        temperature = (ADC_value - 500) / 10
        time.sleep(0.5)
        print ser.readline()
        print 'not blocked'


 


    except KeyboardInterrupt:
        break


ser.close()

requests.post(URL_HOME_SENSOR+"/api/post?apikey=" + MY_MSG_FORMAT_KEY +"&json={temperature:"+str(temperature)+"}")
return "temperature in degree Celsius is %d." % temperature       
        
        
        
def receive(self, src, msg):
    print "Message Received: "+ str(msg) + "\n"      
        
        
        
#        read_adc0 = readadc(adcnum, SPICLK, SPIMOSI, SPIMISO, SPICS)
#
#        # convert analog reading to millivolts = ADC * ( 3300 / 1024 )
#        millivolts = read_adc0 * ( 3300.0 / 1024.0)
#
#        # 10 mv per degree 
#        temp_C = ((millivolts - 100.0) / 10.0) - 40.0
#
#        # convert celsius to fahrenheit 
#        temp_F = ( temp_C * 9.0 / 5.0 ) + 32
#
#        # remove decimal point from millivolts
#        millivolts = "%d" % millivolts
#
#        # show only one decimal place for temprature and voltage readings
#        temp_C = "%.1f" % temp_C
#        temp_F = "%.1f" % temp_F
#
#        if DEBUG:
#                print "read_adc0:\t", read_adc0
#                print "millivolts:\t", millivolts
#                print "temp_C:\t\t", temp_C
#                print "temp_F:\t\t", temp_F
#                print        