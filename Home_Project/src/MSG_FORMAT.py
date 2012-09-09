'''
Created on 16 Jul 2012

@author: AYODELE-M.AKINGBULU
'''

import os 
import sys 
import threading 
import time, datetime 
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
from webtry import *
import uuid
#from matplotlib.pyplot import *
#from pylab import *
from pyprov.model.type import *
from pyprov.model.relation import *
from pyprov.model.bundle import *
from string import     *

from Home_Sense_Model import *


DC = Namespace('http://purl.org/dc/elements/1.1/')
FOAF = Namespace('http://xmlns.com/foaf/0.1/')
HS = PROVNamespace('hs','http://homesensor.com/#')
SN = Namespace('sn',"http://homesensor.com/schemas/sensor_network#")
SENSORS = Namespace('sensors',"http://www.homesensor.com/sensors#")
RDFS = Namespace('http://www.w3.org/2000/01/rdf-schema#')
prov = Namespace("http://www.w3.org/ns/prov-dm/")
TIME_1 = Namespace('sensors',"http://www.homesensor.com/TIME#")
rdf = PROVNamespace("rdf","http://www.w3.org/TR/rdf-schema/#")

           
            
#---------------------Home Sensor Model Instance based on serial message received-------------------------------



#---------------------Function to check or add Home Sensor Model Instances--------------------------



# Open Store
sensor_graph=Mystore('mystore', 'mystore')

## Open serial Port for sensor data
#try:
#        
##        ser = serial.Serial('COM12',115200,timeout=1,parity=serial.PARITY_NONE,
##              stopbits=serial.STOPBITS_ONE,
##              bytesize=serial.EIGHTBITS,
##              )   
#    
#except:
#        print "Could not open serial port: ", sys.exc_info()[0]
#        sys.exit(2)
#        
## Read sensor data through serial port
#while True:
#data = str(ser.read(100).strip());

    #data = data.replace("\n","").replace("\r","")
    # To Test

def addtoStore(msg,msg_timestamp):
    

# Create appropriate instances(based on Home Sensor Model) for sensor data
    mac_address = msg[0]
    type1 = msg[1]
    type1_value = msg[2]
    type2 = msg[3]
    type2_value = msg[4]
    type3 = msg[5]
    type3_value = msg[6]  
    
    S_Network = Sensor_Network(identifier=HS[str(uuid.uuid1())], attributes=None, account=None)
    sensor_graph.addPROVInstance(S_Network) 

    sn1 = Sensor_Node(identifier=HS[mac_address],attributes=None, account=None)
    sensor_graph.addPROVInstance(sn1)
    
    sn2 = Sensor_Node(identifier=HS[mac_address],attributes=None, account=None)
    sensor_graph.addPROVInstance(sn2)
    
    sn3 = Sensor_Node(identifier=HS[mac_address],attributes=None, account=None)
    sensor_graph.addPROVInstance(sn3)
    
    sensor1 = Sensor(identifier=HS[mac_address+'_1'],sensor_type=type1,attributes=None, account=None)
    sensor_graph.addPROVInstance(sensor1)
    
    sensor_reading1 = Sensor_Readings(identifier=HS[str(uuid.uuid1())], value=type1_value)
    sensor_graph.addPROVInstance(sensor_reading1)
    
    # do the same, except the sensor node, for type2 and type3
    sensor2 = Sensor(identifier=HS[mac_address+'_2'],sensor_type=type2,attributes=None, account=None)
    sensor_graph.addPROVInstance(sensor2)
    
    sensor_reading2 = Sensor_Readings(identifier=HS[str(uuid.uuid1())], value=type2_value,attributes=None, account=None)
    sensor_graph.addPROVInstance(sensor_reading2)
    
    sensor3 = Sensor(identifier=HS[mac_address+'_3'],sensor_type=type3,attributes=None, account=None)
    sensor_graph.addPROVInstance(sensor3)
    
    sensor_reading3 = Sensor_Readings(identifier=HS[str(uuid.uuid1())], value=type3_value, attributes=None, account=None)
    sensor_graph.addPROVInstance(sensor_reading3)
    
    Activity4 = Sensor_Reading_Activity(identifier=HS["SRA"], attributes=None, account=None, Sensor_Reading=sensor_reading1)
    sensor_graph.addPROVInstance(Activity4)
    
    Activity5 = Sensor_Reading_Activity(identifier=HS["SRA"], attributes=None, account=None, Sensor_Reading=sensor_reading2)
    sensor_graph.addPROVInstance(Activity5)
    
    Activity6 = Sensor_Reading_Activity(identifier=HS["SRA"], attributes=None, account=None, Sensor_Reading=sensor_reading3)
    sensor_graph.addPROVInstance(Activity6)
    
    # entity instance in the Model
    Entity0 = Entity(identifier=None, attributes=None, account=None)
    sensor_graph.addPROVInstance(Entity0)
    
    Entity1 = Entity(identifier=None, attributes=None, account=None)
    sensor_graph.addPROVInstance(Entity1)
    
    Entity2 = Entity(identifier=None, attributes=None, account=None)
    sensor_graph.addPROVInstance(Entity2)
    
    # do the relations here as well
    # relationship between 3 sensor nodes(agent) and sensor network(agent)
    aOB0 = actedOnBehalfOf(subordinate=sn1, responsible=S_Network, identifier=None, attributes=None, account=None)
    sensor_graph.addPROVInstance(aOB0)
    aOB1 = actedOnBehalfOf(subordinate=sn2, responsible=S_Network, identifier=None, attributes=None, account=None)
    sensor_graph.addPROVInstance(aOB1)
    aOB2 = actedOnBehalfOf(subordinate=sn3, responsible=S_Network, identifier=None, attributes=None, account=None)
    sensor_graph.addPROVInstance(aOB2)


    # relationship between 3 sensor nodes(agent) and their sensors(agent)
    aOB3 = actedOnBehalfOf(subordinate=sensor1, responsible=sn1, identifier=None, attributes=None, account=None)
    sensor_graph.addPROVInstance(aOB3)
    aOB4 = actedOnBehalfOf(subordinate=sensor2, responsible=sn2, identifier=None, attributes=None, account=None)
    sensor_graph.addPROVInstance(aOB4)
    aOB5 = actedOnBehalfOf(subordinate=sensor3, responsible=sn3, identifier=None, attributes=None, account=None)
    sensor_graph.addPROVInstance(aOB5)


    # relationship between sensors(agent) and sensor reading activity(activity)
    wAW0 = wasAssociatedWith(activity=Activity4, agent=sensor1, identifier=HS["wAW0"], attributes=tripledict, account=None)
    sensor_graph.addPROVInstance(wAW0)
    wAW1 = wasAssociatedWith(activity=Activity5, agent=sensor2, identifier=HS["wAW1"], attributes=tripledict, account=None)
    sensor_graph.addPROVInstance(wAW1)
    wAW2 = wasAssociatedWith(activity=Activity6, agent=sensor3, identifier=HS["wAW2"], attributes=tripledict, account=None)
    sensor_graph.addPROVInstance(wAW2)
    
    
    # relationship between entity and activity
    wGB0 = wasGeneratedBy(entity=Entity0,activity=Activity4,identifier=HS["wGB0"],time=None,attributes=tripledict)
    sensor_graph.addPROVInstance(wGB0)
    
    wGB1 = wasGeneratedBy(entity=Entity1,activity=Activity5,identifier=HS["wGB0"],time=None,attributes=tripledict)
    sensor_graph.addPROVInstance(wGB1)
    
    wGB2 = wasGeneratedBy(entity=Entity2,activity=Activity6,identifier=HS["wGB0"],time=None,attributes=tripledict)
    sensor_graph.addPROVInstance(wGB2)

    # relationship between sensors(agent) and entities(entity)
    wAT0 = wasAttributedTo(entity=Entity0, agent=sensor1, identifier=HS["wAT0"], attributes=None, account=None)
    sensor_graph.addPROVInstance(wAT0)
    
    wAT1 = wasAttributedTo(entity=Entity1, agent=sensor2, identifier=HS["wAT1"], attributes=None, account=None)
    sensor_graph.addPROVInstance(wAT1)
    
    wAT2 = wasAttributedTo(entity=Entity2, agent=sensor3, identifier=HS["wAT2"], attributes=None, account=None)
    sensor_graph.addPROVInstance(wAT2)
    
    

data = ['Node: 8766 Temp: 23 Humidity: 65 Light: 4Node: 3fb7 Temp: 22 Humidity: 65 Light: 4']
msg = str(data)

msglist = msg.split(":")

for msg in msglist:
    
        
    msg = msg.split(",")[0]
    
#try:
        #if len(data) > 0:
        #    data.append(('timestamp:', time.asctime())) 
    # Print sensor data              
    print "data: ", msg
    msg_timestamp = datetime.datetime.now()
    addtoStore(msg, msg_timestamp)
    

 
   
        
#except:
#        print "No data from serial port: ", sys.exc_info()[1]
#        sys.exit(2)
    
   


    
    
    
    
        




