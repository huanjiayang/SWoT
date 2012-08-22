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
import wx
import simplejson
from matplotlib.pyplot import *
from pylab import *






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
            #print 'Got:', data
  
        
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


# Create an animated graph
fig = plt.fig()
# plot data
screen = data.add_subplot(111)


# The axis for the screen and temperature data
screen_t = np.arange(0, 18, 1)
tempgraph_line, = screen.plot(screen_t, [0] * 18, color='blue')
screen.set_ylabel('temp')
screen.set_xlabel('measurements #')
screen.set_ylim(-200, 200)


# make a second axis for humidity data
Humiditygraph = screen.twinx()
Humiditygraph_line, = Humiditygraph.plot(screen_t, [0] * 18, color='green')
Humiditygraph.set_ylabel('Humidity')
Humiditygraph.set_ylim(-15, 15)


# make a second axis for Light data
Lightgraph = screen.twinx()
Lightgraph_line, = Humiditygraph.plot(screen_t, [0] * 18, color='black')
Lightgraph.set_ylabel('Light')
Lightgraph.set_ylim(-100, 100)


# and a legend for data
legend((Lightgraph_line, Humiditygraph_line,tempgraph_line ), ('temp', 'humidity', 'Light'))



def update_graph(self,temperature,Light,Humidity,TEMPSENSOR,LIGHTSENSOR,HUMIDITYSENSOR):
    # grab one packet from the xbee, or timeout
    get_sample = self.get_temperature
    get_sample = self.get_humidity
    get_sample = self.get_Light
    values = get_sample()
    data = data.find_packet(values)
    if len(data) > 0:
        xb = data(data)

        # we'll only store n-1 samples since the first one is usually messed up
        tempdata = [-1] * (data - 1)
        lightdata = [-1] * (data  -1)
        humiditydata = [-1] * (data  -1)
        # grab 1 thru n of the ADC readings, referencing the ADC constants
        # and store them in nice little arrays
        for i in range(len(data)):
            temperature[i] = data[i+1][TEMPSENSOR]
            Light[i] = data[i+1][LIGHTSENSOR] 
            Humidity[i] = data[i+1][HUMIDITYSENSOR] 
            
            
            print "temp values: ", tempdata
            print "light values:  ", lightdata
            print "hunidity values", humiditydata

        # Redraw our pretty picture
        fig.canvas.draw_idle()
        # Update with latest data
        tempgraph_line.set_ydata(tempdata)
        Lightgraph_line.set_ydata(lightdata)
        Humiditygraph_line.set_ydata(lightdata)
        # Update our graphing range so that we always see all the data
        maxtemp = max(tempdata)
        mintemp = min(tempdata)
        maxtemp = max(maxtemp, -mintemp)
        screen_t.set_ylim(maxtemp * -1.2, maxtemp * 1.2)
        
    
        
        ####### store sensor data and array of histories per sensor
class Fiveminutehistory():
    def __init__(self, sensornum):
        self.sensornum = sensornum
        self.fiveminutetimer = time.time()  # track data over 5 minutes
        self.lasttime = time.time()
        self.cumulativedata = 0
      
    def adddata(self, deltawatthr):
        self.data += float(data)

    def reset5mintimer(self):
        self.data = 0
        self.fiveminutetimer = time.time()

    def avgdataover5min(self):
        return self.cumulativewatthr * (60.0*60.0 / (time.time() - self.fiveminutetimer))
  
    def __str__(self):
        return "[ id#: %d, 5mintimer: %f, lasttime; %f, cumulativedata: %f ]" % (self.sensornum, self.fiveminutetimer, self.lasttime, self.cumulativedata)



sensor_history = []


def findsensorhistory(sensornum):
    for history in sensor_history:
        if history.sensornum == sensornum:
            return history
    # none found, create it!
    history = Fiveminutehistory(sensornum)
    sensor_history.append(history)
    return history
      



def sensor_detail(self,get_sample,sensor,Node):
    get_sample = self.get_temperature
    get_sample = self.get_humidity
    get_sample = self.get_Light
    values = get_sample()
    data.append(values)
    if len(data) > 0:
        data.pop(0)
    result = []
    for sensor in data[0]:
        result .append([])
    for sample in data:
        for sensor_id in range(len(sample)):
            value = sample[sensor_id]
            result[sensor_id].append(value)
    
   
    
    
URL_HOME_SENSOR = 'http://localhost:8080/homesensorcom/'
MY_MSG_FORMAT_KEY = 'link from message format API'
requests.post(URL_HOME_SENSOR+"/api/post?apikey=" + MY_MSG_FORMAT_KEY +"&json={data:"+str(data)+"}")
