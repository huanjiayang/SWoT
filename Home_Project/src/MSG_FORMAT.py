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

#create all the appropriate instances based on what is in the message 
#remove hardcodings below

Agent0 = Sensor_Network(identifier=HS["SN"], attributes=None, account=None)
Agent1 = Sensor_Node(identifier=HS["Sensor_Node"], attributes=HS["sensor_id"], account=None, sensor_id=HS["sensor_id"], sensor_name=HS["sensor_name"])
Agent2 = Sensor_Node(identifier=HS["Sensor_Node"], attributes=HS["sensor_id"], account=None, sensor_id=HS["sensor_id"], sensor_name=HS["sensor_name"])
Agent3 = Sensor_Node(identifier=HS["Sensor_Node"], attributes=HS["sensor_id"], account=None, sensor_id=HS["sensor_id"], sensor_name=HS["sensor_name"])

Agent4 = Sensor(identifier=HS["Sensor"], sensor_id=None, sensor_name=None,attributes=None, account=None)
Agent5 = Sensor(identifier=HS["Sensor"], sensor_id=None, sensor_name=None,attributes=None, account=None)
Agent6 = Sensor(identifier=HS["Sensor"], sensor_id=None, sensor_name=None,attributes=None, account=None)


#activity instance in the model
Activity0 = Network_Organization(identifier=HS["NO"], network_id=None, sensor_name=None, attributes=None, account=None,starttime=None, endtime=None,)
Activity1 = Discovery(identifier=HS["D"], sensor_id=None, sensor_name=None, attributes=None, account=None)
Activity2 = Query(identifier=HS["Q"], attributes=None, account=None)
Activity3 = Sensor_Node_Activity(identifier=HS["SNA"], attributes=None, account=None)
Activity4 = Sensor_Reading_Activity(identifier=HS["SRA"], attributes=None, account=None, Sensor_Reading=None)
Activity5 = Sensor_Reading_Activity(identifier=HS["SRA"], attributes=None, account=None, Sensor_Reading=None)


# entity instance in the Model
Entity0 = Entity(identifier=None, attributes=None, account=None)
Entity1 = Entity(identifier=None, attributes=None, account=None)



# relationship between 3 sensor nodes(agent) and sensor network(agent)
aOB0 = actedOnBehalfOf(subordinate=Agent1, responsible=Agent0, identifier=None, attributes=None, account=None)
aOB1 = actedOnBehalfOf(subordinate=Agent2, responsible=Agent0, identifier=None, attributes=None, account=None)
aOB2 = actedOnBehalfOf(subordinate=Agent3, responsible=Agent0, identifier=None, attributes=None, account=None)


# relationship between 3 sensor nodes(agent) and their sensors(agent)

aOB3 = actedOnBehalfOf(subordinate=Agent4, responsible=Agent1, identifier=None, attributes=None, account=None)
aOB4 = actedOnBehalfOf(subordinate=Agent5, responsible=Agent2, identifier=None, attributes=None, account=None)
aOB5 = actedOnBehalfOf(subordinate=Agent6, responsible=Agent3, identifier=None, attributes=None, account=None)


# relationship between sensors(agent) and sensor reading activity(activity)

wAW0 = wasAssociatedWith(activity=Activity4, agent=Agent4, identifier=HS["wAW0"], attributes=tripledict, account=None)
wAW1 = wasAssociatedWith(aactivity=Activity4, agent=Agent5, identifier=HS["wAW1"], attributes=tripledict, account=None)
wAW2 = wasAssociatedWith(activity=Activity4, agent=Agent6, identifier=HS["wAW2"], attributes=tripledict, account=None)


# relationship between sensors(agent) and entities(entity)
wAT0 = wasAttributedTo(entity=Entity0, agent=Agent4, identifier=HS["wAT0"], attributes=None, account=None)
wAT1 = wasAttributedTo(entity=Entity0, agent=Agent5, identifier=HS["wAT1"], attributes=None)
wAT2 = wasAttributedTo(entity=Entity0, agent=Agent6, identifier=HS["wAT2"], attributes=None)


#---------------------Function to check or add Home Sensor Model Instances--------------------------
#
#def sense_instance(self,sensor_dict, sensor_store):    
#    #sensor_dict = [13]
#    found = False    
#    for i in sensor_dict:
#        if (i != None):
#            for s, p, o in sensor_store:
#                found = False
#                if (str(i) == str(s)):
#                    print i, " found."
#                    found = True
#                    break
#            if (found == False):
#                print i, " not in store."
#                self.store.add(i)
#                # TODO: add to store
             

#sensor_dict = [ag0, Agt0,Agt1,Agt2,Ent0,Ent1,Ent2,Ent3,Ent4,Avt0,Avt1,Avt2,Avt3,Avt4]
#sense_instance(sensor_dict, sensor_graph.store)
# Open serial Port for sensor data

sensor_graph=Mystore('mystore', 'mystore')

try:
        ser = serial.Serial('COM12',115200,timeout=1,parity=serial.PARITY_NONE,
              stopbits=serial.STOPBITS_ONE,
              bytesize=serial.EIGHTBITS,
              )   
    
except:
        print "Could not open serial port: ", sys.exc_info()[0]
        sys.exit(2)
        
# Read sensor data through serial port
while True:
    data = str(ser.read(100).strip());
    data = data.replace("\n","").replace("\r","")
    msglist = data.split(";")
    for msg in msglist:
        msg = msg.split(",")
    try:
        #if len(data) > 0:
        #    data.append(('timestamp:', time.asctime())) 
    # Print sensor data              
        print "data: ", msg
        msg_timestamp = time.asctime()
        addtoStore(msg, msg_timestamp)
    except:
        print "No data from serial port: ", sys.exc_info()[1]
        sys.exit(2)
    
   


def addtoStore(msg,msg_timestamp):
    

# Create appropriate instances(based on Home Sensor Model) for sensor data
    mac_address = msg[0]
    type1 = msg[1]
    type1_value = msg[2]
    type2 = msg[3]
    type2_value = msg[4]
    type3 = msg[5]
    type3_value = msg[6]    

    sn1 = Sensor_Node(identifier=HS[mac_address])
    sensor_graph.addPROVInstance(sn1)
    
    sensor1 = Sensor(identifier=HS[mac_address+'_1'],sensor_type=type1)
    sensor_reading1 = Sensor_Readings(identifier=HS[str(uuid.uuid1())], value=type1_value)
    # do the relations her as well
    
    
    # do the same, except the sensor node, for type2 and type3
    


# get rid of the rest
'''    
    if sensor_type == mac_address1:
        Agent1 = Sensor_Node(identifier=mac_address1)
        if sensor_type == type1:
            Agent3 = Sensor(identifier=type1,attributes=None, account=None)
            Activity3 = Sensor_Reading_Activity(identifier=type1_value, attributes=None, account=None, Sensor_Reading=None, starttime=None, endtime=None)
            wAW0 = wasAssociatedWith(a0, ag1, identifier=HS["wAW0"], attributes=tripledict)
        elif sensor_type == type2:
            Agent4 = Sensor(identifier=type2, attributes=None, account=None)
            Activity4 = Sensor_Reading_Activity(identifier=type2_value, attributes=None, account=None, Sensor_Reading=None, starttime=None, endtime=None)
            wAW0 = wasAssociatedWith(a0, ag1, identifier=HS["wAW0"], attributes=tripledict)
        elif sensor_type == type3:
            Agent5 = Sensor(identifier=type3, attributes=None, account=None)
            Activity5 = Sensor_Reading_Activity(identifier=type3_value, attributes=None, account=None, Sensor_Reading=None, starttime=None, endtime=None)   
            wAW0 = wasAssociatedWith(a0, ag1, identifier=HS["wAW0"], attributes=tripledict)
        else:
            print sensor_data + " does not contain required information."
        
    elif sensor_type == mac_address2:
        Agent2 = Sensor_Node(identifier=mac_address2)
        if sensor_type == type1_2:
            Agent3 = Sensor(identifier=type1_2,  attributes=None, account=None)
            Activity3 = Sensor_Reading_Activity(identifier=type1_2_value, attributes=None, account=None, Sensor_Reading=None, starttime=None, endtime=None,Light=None,Temperature=None,Humidity=None,sensor_type=None)
            wAW0 = wasAssociatedWith(a0, ag1, identifier=HS["wAW0"], attributes=tripledict)
        elif sensor_type == type2_2:
            Agent4 = Sensor(identifier=type2_2, attributes=None, account=None,)
            Activity4 = Sensor_Reading_Activity(identifier=type2_2_value, attributes=None, account=None, Sensor_Reading=None, starttime=None, endtime=None, Light=None,Temperature=None,Humidity=None,sensor_type=None)
            wAW0 = wasAssociatedWith(a0, ag1, identifier=HS["wAW0"], attributes=tripledict)
        elif sensor_type == type3_2:
            Agent5 = Sensor(identifier=type3_2, attributes=None, account=None)
            Activity5 = Sensor_Reading_Activity(identifier=type3_2_value, attributes=None, account=None, Sensor_Reading=None, starttime=None, endtime=None,Light=None,Temperature=None,Humidity=None,sensor_type=None)
            wAW0 = wasAssociatedWith(a0, ag1, identifier=HS["wAW0"], attributes=tripledict)   
        else:
            print sensor_data + " does not contain required information."
             
    elif sensor_type == mac_address3:
        Agent3 = Sensor_Node(identifier=mac_address3)
        if sensor_type == type1_3:
            Agent3 = Sensor(identifier=type1_3, attributes=None, account=None)
            Activity3 = Sensor_Reading_Activity(identifier=type1_3_value, attributes=None, account=None, Sensor_Reading=None, starttime=None, endtime=None,Light=None,Temperature=None,Humidity=None,sensor_type=None)
            wAW0 = wasAssociatedWith(a0, ag1, identifier=HS["wAW0"], attributes=tripledict)
        elif sensor_type == type2_3:
            Agent4 = Sensor(identifier=type2_3,  attributes=None, account=None,)
            Activity4 = Sensor_Reading_Activity(identifier=type2_3_value, attributes=None, account=None, Sensor_Reading=None, starttime=None, endtime=None, Light=None,Temperature=None,Humidity=None,sensor_type=None)
            wAW0 = wasAssociatedWith(a0, ag1, identifier=HS["wAW0"], attributes=tripledict)
        elif sensor_type == type3_3:
            Agent5 = Sensor(identifier=type3_3,  attributes=None, account=None)
            Activity5 = Sensor_Reading_Activity(identifier=type3_3_value, attributes=None, account=None, Sensor_Reading=type3_3_value, starttime=None, endtime=None,Light=None,Temperature=None,Humidity=None,sensor_type=None)
            wAW0 = wasAssociatedWith(a0, ag1, identifier=HS["wAW0"], attributes=tripledict)   
        else:
            print sensor_data + " does not contain required information."
    # close serial port    
    #ser.close()
'''   
    
#    
#URL_HOME_SENSOR = 'http://localhost:8080/homesensorcom/'
#MY_MSG_FORMAT_KEY = 'link from message format API'
#requests.post(URL_HOME_SENSOR+"/api/post?apikey=" + MY_MSG_FORMAT_KEY +"&json={data:"+str(data)+"}")
