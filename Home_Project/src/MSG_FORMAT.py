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
import numpy
import matplotlib
import twitter
import wx
import uuid
#from matplotlib.pyplot import *
#from pylab import *
from pyprov.model.type import *
from pyprov.model.relation import *
from pyprov.model.bundle import *


from Home_Sense_Model import *


#
#api = twitter.Api(consumer_key='your key here', consumer_secret='your key here', access_token_key='your key here', access_token_secret='your key here')
#
## Twitter username & password
#twitterusername = "sense_AY"
#twitterpassword = "sensorbizy_007"
#
#
#def TwitterIt(u, p, message):
#    api = twitter.Api(username=u, password=p)
#    print u, p
#    try:
#        status = api.PostUpdate(message)
#        print "%s just posted: %s" % (status.user.name, status.text)
#    except UnicodeDecodeError:
#        print "Your message could not be encoded.  Perhaps it contains non-ASCII characters? "
#        print "Try explicitly specifying the encoding with the  it with the --encoding flag"
#    except:
#        print "Couldn't connect, check network, username and password!"
#
#
#
#
#
##ser = serial.Serial('COM12',baudrate=115200,parity=serial.PARITY_NONE,stopbits=serial.STOPBITS_TWO,bytesize=serial.SEVENBITS)  # open first serial port
##print ser.portstr       # check which port was really used
###data = ser.read(10)
##data = str(ser.readline());
###line = ser.readline()
##print data     # write a string
##ser.close()             # close port
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#        
#    
#
#
##ser = serial.Serial(0)  # open first serial port
#try:
#        ser = serial.Serial('COM12',115200,timeout=1,parity=serial.PARITY_NONE,
#              stopbits=serial.STOPBITS_ONE,
#              bytesize=serial.EIGHTBITS,
#              )
#        print ser.portstr 
#        status = ser.read()
#        #Post status to twitter
#        api.PostUpdate(status.text)
#        time.sleep(3600)  ##sleep for 3600 seconds
##        ser.open()
#except:
#        print "Could not open serial port: ", sys.exc_info()[0]
#        sys.exit(2)
#
#print ser.portstr       # check which port was really used
##ser.write("hello")      # write a string
#
#while True:
#    ser.flushInput()
#    data = str(ser.read(100).strip());
#    data = data.replace("\n","").replace("\r","")
#    data = data.split(",")
#    try:
#        
#        if len(data) > 0:
#            data.append(('timestamp:', time.asctime()))
#            #print 'Got:', data
#  
#        
##        data = int("data",16)
##        ADC_value = raw_value / 10
##        temperature = (ADC_value - 500) / 10                               
##        time.sleep(0.5)
#        
##        data = struct.unpack('>fff', data)
##        data = struct.unpack('B', data)
#
#        print "data: ", data
#        
#        
#      
#
##        print 'not blocked'
#
#
# 
#
#
#    except KeyboardInterrupt:
#        break
#
#
#ser.close()
#
#
#
#
#
#def sensor_detail(self,get_sample,sensor,Node):
#    get_sample = self.get_temperature
#    get_sample = self.get_humidity
#    get_sample = self.get_Light
#    values = get_sample()
#    data.append(values)
#    if len(data) > 0:
#        data.pop(0)
#    result = []
#    for sensor in data[0]:
#        result .append([])
#    for sample in data:
#        for sensor_id in range(len(sample)):
#            value = sample[sensor_id]
#            result[sensor_id].append(value)
#            
            
#---------------------Home Sensor Model Instance based on serial message received-------------------------------

Agt0 = Sensor_Network(identifier=HS["SN"], attributes=None, account=None)
Agt1 = Sensor_Node(identifier=HS["Sensor_Node"], attributes=None, account=None, sensor_id=HS["sensor_id"])
Agt2 = Sensor(identifier=HS["Sensor"], sensor_id=None, sensor_name=None, attributes=None, account=None)

Ent0 = Temperature_Sensor(activity=None, identifier=HS["TS"], attributes=None, account=None)
Ent1 = Humidity_Sensor(identifier=HS["HM"], sensor_id=None, sensor_name=None, attributes=None, account=None)
Ent2 = Light_Sensor(identifier=HS["LS"], sensor_id=None, sensor_name=None, attribute=None, account=None)
Ent3 = Sensor_Readings(identifier=HS["SR"], attributes=None, account=None)
Ent4 = observation(identifier=HS["ob"], attributes=None, account=None, Observation=None,Temperature=None,Humidity=None,Light=None)


Avt0 = Network_Organization(identifier=HS["NO"], sensor_id=None, sensor_name=None, attributes=None, account=None)
Avt1 = Discovery(identifier=HS["D"], sensor_id=None, sensor_name=None, attributes=None, account=None)
Avt2 = Query(identifier=HS["Q"], attributes=None, account=None)
Avt3 = Sensor_Node_Activity(identifier=HS["SNA"], attributes=None, account=None)
Avt4 = Sensor_Reading_Activity(identifier=HS["SRA"], attributes=None, account=None, Sensor_Reading=None)


#---------------------Function to check or add Home Sensor Model Instances--------------------------

def sense_instance(self,sensor_dict):    
    #sensor_dict = [13]
    for i in sensor_dict:
        if (i != None):
            print i
             

sensor_dict = [Agt0,Agt1,Agt2,Ent0,Ent1,Ent2,Ent3,Ent4,Avt0,Avt1,Avt2,Avt3,Avt4]
sense_instance(sensor_dict)
    
   
    
#    
#URL_HOME_SENSOR = 'http://localhost:8080/homesensorcom/'
#MY_MSG_FORMAT_KEY = 'link from message format API'
#requests.post(URL_HOME_SENSOR+"/api/post?apikey=" + MY_MSG_FORMAT_KEY +"&json={data:"+str(data)+"}")
