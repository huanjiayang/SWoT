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
import re
import json
import uuid
from pyprov.model.type import *
from pyprov.model.relation import *
from pyprov.model.bundle import *
from string import     *
from rdflib import Namespace, BNode, Literal, RDF, URIRef
from Home_Sense_Model import *
import itertools


DC = Namespace('http://purl.org/dc/elements/1.1/')
FOAF = Namespace('http://xmlns.com/foaf/0.1/')
HS = PROVNamespace('hs','http://homesensor.com/')
SN = Namespace('sn',"http://homesensor.com/schemas/sensor_network#")
SENSORS = Namespace('sensors',"http://www.homesensor.com/sensors#")
RDFS = Namespace('http://www.w3.org/2000/01/rdf-schema#')
prov = Namespace("http://www.w3.org/ns/prov-dm/")
TIME_1 = Namespace('sensors',"http://www.homesensor.com/TIME#")
#rdf = PROVNamespace("rdf","http://www.w3.org/2000/01/rdf-schema#")

           
            
#---------------------Home Sensor Model Instance based on serial message received-------------------------------



#---------------------Function to check or add Home Sensor Model Instances--------------------------

class PROVBuilder:
    
    def __init__(self):
        self.container = PROVContainer()
        
    def _createEntity_Agent(self,entityURI, RDFStore):
        sss = None
        for ttt in RDFStore.triples((entityURI,RDFS['type'],None)):
            entity_type = ttt[2]
        if entity_type == HS['Sensor']:
            for stypetriple in RDFStore.triples((entityURI,HS['sensor_type'],None)):
                sensor_type = stypetriple[2]
            sss = Sensor(identifier = entityURI,sensor_type=sensor_type)
        elif entity_type == HS['Sensor_Node']:
            sss = Sensor_Node(identifier = entityURI)
        elif entity_type == HS['Sensor_Network']:
            sss = Sensor_Network(identifier = entityURI)
        elif entity_type == HS['Sensor_Readings']:
            for stypetriple in RDFStore.triples((entityURI,HS['value'],None)):
                value = stypetriple[2]
            sss = Sensor_Readings(identifier = entityURI,value=value )
        return sss
    

    def _createActivity(self,activityURI,RDFStore):
        for ttt in RDFStore.triples((activityURI,RDFS['type'],None)):
            entity_type = ttt[2]
        if entity_type == HS['Sensor']:
            sss = Sensor(identifier = activityURI)
        elif entity_type == HS['Sensor_Node']:
            sss = Sensor_Node(identifier = activityURI)
        elif entity_type == HS['Sensor_Network']:
            sss = Sensor_Network(identifier = activityURI)
        elif entity_type == HS['Sensor_Reading_Activity']:
            sss = Sensor_Reading_Activity(identifier = activityURI)
        elif entity_type == HS['Network Organization']:
            for stypetriple in RDFStore.triples((activityURI,HS['starttime'],None)):
                starttime = stypetriple[2]
            sss = Network_Organization(identifier = activityURI, starttime=starttime)
        elif entity_type == HS['Discovery']:
            for stypetriple in RDFStore.triples((activityURI, HS['starttime'], None)):
                starttime = stypetriple[2]
            sss = Discovery(identifier = activityURI, starttime=starttime)
        elif entity_type == HS['Query']:
            for stypetriple in RDFStore.triples((activityURI, HS['starttime'], None)):
                starttime = stypetriple[2]
            sss = Query(identifier = activityURI,starttime=starttime)
        elif entity_type == HS['Sensor_Node_Activity']:
            for stypetriple in RDFStore.triples((activityURI, HS['starttime'], None)):
                starttime = stypetriple[2]
            sss = Sensor_Node_Activity(identifier = activityURI, starttime=starttime)
        return sss
        
    def traverseStore(self,RDFstore):
        print "let's see what we have in store:"
        print str(RDF.type)
        for s,p,o in RDFstore:
            print s,p,o
        for RDFtype_triple in RDFstore.triples((None, RDFS['type'], None)):
            sub = RDFtype_triple[0]
            print 'sub = '
            print sub
            rdftype = RDFtype_triple[2]
            print 'type'
            print rdftype
            attrdict = {}
            for attr in RDFstore.triples((sub, None, None)):
                if not attr[1] == RDFS['type']:
                    attrdict[attr[1]] = attr[2]
            #if entity matches add to container
            if rdftype == HS['Sensor']:
                print 'Sensor found'
                e = Sensor(str(sub),attributes=attrdict)
                self.container.add(e)
            elif rdftype == prov['Activity']:
                print 'Activity found'
                for Activity_triple in RDFstore.triples((sub, prov['starttime'], None)):
                    starttime = Activity_triple[2]
                a = Activity(str(sub),attributes=attrdict,starttime=starttime)
                self.container.add(a)
            elif rdftype == prov['wasGeneratedBy']:
                print 'wGB found'
                for Activity_triple in RDFstore.triples((sub, prov['wasGeneratedBy'], None)):
                    generated = Activity_triple[2]
                for relation_triple in RDFstore.triples((sub, prov['entity'], None)):
                        entityURI = relation_triple[2]
                        entity = self._createEntity_Agent(entityURI,RDFstore)
                        self.container.add(entity)
                            
                            
                for relation_triple in RDFstore.triples((sub, prov['activity'], None)):
                    activityURI = relation_triple[2]
                    activity = self._createActivity(activityURI,RDFstore)
                    self.container.add(activity)
                    
                gb = wasGeneratedBy(entity, activity, identifier=str(sub))
                self.container.add(gb)
            elif rdftype == prov['wasStartedByActivity']:
                print 'wSBA found'
                for Activity_triple in RDFstore.triples((sub, prov['wasStartedByActivity'], None)):
                    startingact = Activity_triple[2]
                for relation_triple in RDFstore.triples((sub, prov['started'], None)):
                    activityURI = relation_triple[2]
                    started = self._createActivity(activityURI,RDFstore)
                    self.container.add(started)
                    
                for relation_triple in RDFstore.triples((sub, prov['starter'], None)):
                    activityURI = relation_triple[2]
                    starter = self._createActivity(activityURI,RDFstore)
                    self.container.add(starter)
                sba = wasStartedByActivity(started, starter, identifier=str(sub))
                self.container.add(sba)
            elif rdftype == prov['wasStartedBy']:
                print 'wSB found'
                for Activity_triple in RDFstore.triples((sub, prov['wasStartedBy'], None)):
                    wasStartedBy = Activity_triple[2]
                for relation_triple in RDFstore.triples((sub, prov['entity'], None)):
                    entityURI = relation_triple[2]
                    entity = self._createEntity_Agent(entityURI,RDFstore)
                    self.container.add(entity)
                for relation_triple in RDFstore.triples((sub, prov['activity'], None)):
                    activityURI = relation_triple[2]
                    activity = self._createActivity(activityURI,RDFstore)
                    self.container.add(activity)
                        
                sb = wasStartedBy(activity,entity,identifier=str(sub))
                self.container.add(sb)
            elif rdftype == prov['wasAssociatedWith']:
                print 'wAW found'
                for relation_triple in RDFstore.triples((sub, prov['wasAssociatedWith'], None)):
                    wasAssociatedWith = relation_triple[2]                        
                    for relation_triple in RDFstore.triples((sub, prov['agent'], None)):
                        agentURI = relation_triple[2]
                        ag = self._createEntity_Agent(agentURI,RDFstore)
                        self.container.add(ag)
                    for relation_triple in RDFstore.triples((sub, prov['activity'], None)):
                        activityURI = relation_triple[2]
                        activity = self._createActivity(activityURI,RDFstore)
                        self.container.add(activity)
                          
                    if not entity == None:
                        print activity, "wasAssociatedWith", entity
                    else:
                        print activity, "wasAssociatedWith", ag
                    aw = wasAssociatedWith(activity, ag, identifier=str(sub))
                    self.container.add(aw)
                
            elif rdftype == prov['wasDerivedFrom']:
                print 'wDF found'
                for Relation_triple in RDFstore.triples((sub, prov['wasDerivedFrom'], None)):
                    
                    wasDerivedFrom = Relation_triple[2]
                for Relation_triple in RDFstore.triples((sub, prov['generatedentity'], None)):
                    entityURI = relation_triple[2]
                    generatedentity = self._createEntity_Agent(entityURI,RDFstore)
                    self.container.add(generatedentity)
                    
                for Relation_triple in RDFstore.triples((sub, prov['usedentity'], None)):
                    entityURI = relation_triple[2]
                    usedentity = self._createEntity_Agent(entityURI,RDFstore)
                    self.container.add(usedentity)
                     
                df = wasDerivedFrom(generatedentity, usedentity, identifier=str(sub))
                self.container.add(df)  
            elif rdftype == prov['actedOnBehalfOf']:
                print 'aOBO'
                for Relation_triple in RDFstore.triples((sub,prov['responsible'], None)):
                    agentURI = relation_triple[2]
                    responsible = self._createEntity_Agent(agentURI,RDFstore)
                    self.container.add(responsible)
                    
                for Relation_triple in RDFstore.triples((sub,prov['subordinate'], None)):
                    agentURI = relation_triple[2]
                    subordinate = self._createEntity_Agent(agentURI,RDFstore)
                    self.container.add(subordinate)
                    
                print subordinate, "actedOnBehalfOf", responsible
                ob = actedOnBehalfOf(subordinate,responsible,identifier=str(sub))
                self.container.add(ob)
                
                
            elif rdftype == prov['wasAttributedTo']:
                print 'wAT'
                #for Relation_triple in RDFstore.triples((sub,prov['wasAttributedTo'], None)):
                #   wasAttributedTo = Relation_triple[2]
                #at = wasAttributedTo(str) 
                #self.container.add(at)
                 
                for Relation_triple in RDFstore.triples((sub, prov['entity'], None)):
                    entityURI = Relation_triple[2]
                    entity = self._createEntity_Agent(entityURI,RDFstore)
                    self.container.add(entity)
                for Relation_triple in RDFstore.triples((sub, prov['agent'], None)):
                    agent = Relation_triple[2]
                    
                at = wasAttributedTo(entity, agent, identifier=str(sub))
                self.container.add(at)

# Open Store
sg=Mystore('mystore', 'mystore')

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
    mac_address = msg[1]
    type1 = msg[2]
    type1_value = msg[3]
    type2 = msg[4]
    type2_value = msg[5]
    type3 = msg[6]
    type3_value = msg[7]  
    
    Literal_msg_timestamp = Literal(str(msg_timestamp))
    
    S_Network = Sensor_Network(identifier=HS[str(uuid.uuid1())], attributes=None, account=None)
    sg.addPROVInstance(S_Network) 

    sn1 = Sensor_Node(identifier=HS[mac_address],attributes=None,sensor_id=None, sensor_name=None, account=None)
    sg.addPROVInstance(sn1)
    
    sensor1URI = mac_address+'_1'
    sensor1 = Sensor(identifier=HS[sensor1URI],sensor_type = type1,attributes=None, account=None)
    sg.addPROVInstance(sensor1)
    
    sensor_reading1 = Sensor_Readings(identifier=HS[str(uuid.uuid1())], value=type1_value)
    sg.addPROVInstance(sensor_reading1)
    
    # do the same, except the sensor node, for type2 and type3
    sensor2 = Sensor(identifier=HS[mac_address+'_2'],sensor_type=type2,attributes=None, account=None)
    sg.addPROVInstance(sensor2)
    
    sensor_reading2 = Sensor_Readings(identifier=HS[str(uuid.uuid1())], value=type2_value,attributes=None, account=None)
    sg.addPROVInstance(sensor_reading2)
    
    sensor3 = Sensor(identifier=HS[mac_address+'_3'],sensor_type=type3,attributes=None, account=None)
    sg.addPROVInstance(sensor3)
    
    sensor_reading3 = Sensor_Readings(identifier=HS[str(uuid.uuid1())], value=type3_value, attributes=None, account=None)
    sg.addPROVInstance(sensor_reading3)
    
    Activity4 = Sensor_Reading_Activity(identifier=HS[str(uuid.uuid1())], starttime = Literal_msg_timestamp, attributes=None, account=None, Sensor_Reading=sensor_reading1)
    sg.addPROVInstance(Activity4)
    
    Activity5 = Sensor_Reading_Activity(identifier=HS[str(uuid.uuid1())], starttime = Literal_msg_timestamp, attributes=None, account=None, Sensor_Reading=sensor_reading2)
    sg.addPROVInstance(Activity5)
    
    Activity6 = Sensor_Reading_Activity(identifier=HS[str(uuid.uuid1())], starttime = Literal_msg_timestamp, attributes=None, account=None, Sensor_Reading=sensor_reading3)
    sg.addPROVInstance(Activity6)
    

    # do the relations here as well
    # relationship between 3 sensor nodes(agent) and sensor network(agent)
    aOB0 = actedOnBehalfOf(subordinate=sn1, responsible=S_Network, identifier=HS[str(uuid.uuid1())], attributes=None, account=None)
    sg.addPROVInstance(aOB0)

    # relationship between sensor nodes(agent) and their sensors(agent)
    aOB3 = actedOnBehalfOf(subordinate=sensor1, responsible=sn1, identifier=HS[str(uuid.uuid1())], attributes=None, account=None)
    sg.addPROVInstance(aOB3)
    aOB4 = actedOnBehalfOf(subordinate=sensor2, responsible=sn1, identifier=HS[str(uuid.uuid1())], attributes=None, account=None)
    sg.addPROVInstance(aOB4)
    aOB5 = actedOnBehalfOf(subordinate=sensor3, responsible=sn1, identifier=HS[str(uuid.uuid1())], attributes=None, account=None)
    sg.addPROVInstance(aOB5)


    # relationship between sensors(agent) and sensor reading activity(activity)
    wAW0 = wasAssociatedWith(activity=Activity4, agent=sensor1, identifier=HS[str(uuid.uuid1())], attributes=None, account=None)
    sg.addPROVInstance(wAW0)
    wAW1 = wasAssociatedWith(activity=Activity5, agent=sensor2, identifier=HS[str(uuid.uuid1())], attributes=None, account=None)
    sg.addPROVInstance(wAW1)
    wAW2 = wasAssociatedWith(activity=Activity6, agent=sensor3, identifier=HS[str(uuid.uuid1())], attributes=None, account=None)
    sg.addPROVInstance(wAW2)
    
    
    # relationship between entity and activity
    wGB0 = wasGeneratedBy(entity=sensor_reading1,activity=Activity4,identifier=HS[str(uuid.uuid1())],time=Literal_msg_timestamp,attributes=None)
    sg.addPROVInstance(wGB0)
    
    wGB1 = wasGeneratedBy(entity=sensor_reading2,activity=Activity5,identifier=HS[str(uuid.uuid1())],time=Literal_msg_timestamp,attributes=None)
    sg.addPROVInstance(wGB1)
    
    wGB2 = wasGeneratedBy(entity=sensor_reading3,activity=Activity6,identifier=HS[str(uuid.uuid1())],time=Literal_msg_timestamp,attributes=None)
    sg.addPROVInstance(wGB2)

    # relationship between sensors(agent) and entities(entity)
    wAT0 = wasAttributedTo(entity=sensor_reading1, agent=sensor1, identifier=HS[str(uuid.uuid1())], attributes=None, account=None)
    sg.addPROVInstance(wAT0)
    
    wAT1 = wasAttributedTo(entity=sensor_reading2, agent=sensor2, identifier=HS[str(uuid.uuid1())], attributes=None, account=None)
    sg.addPROVInstance(wAT1)
    
    wAT2 = wasAttributedTo(entity=sensor_reading3, agent=sensor3, identifier=HS[str(uuid.uuid1())], attributes=None, account=None)
    sg.addPROVInstance(wAT2)
    
    return msg

data = ['Node,8766,Temp,23,Humidity,65,Light,4']

data = str(data)
data = data.replace("\n","").replace("\r","")
msglist = data.split(";")

for msg in msglist:
    
        
    msg = msg.split(",")
    
#try:
        #if len(data) > 0:
        #    data.append(('timestamp:', time.asctime())) 
    # Print sensor data              
    print "data: ", msg
    msg_timestamp = str(datetime.datetime.now())
    # Create a counter starting at 10
    counter = itertools.count(10)
    print msg_timestamp, counter
    addtoStore(msg, msg_timestamp, counter)
 
  
mybuilder = PROVBuilder()
mybuilder.traverseStore(sg.store)
#print mybuilder.container._elementlist
#print json.dumps(mybuilder.container.to_provJSON())   
print "Let's the recreated store:"     
print mybuilder.container._provcontainer
print sg.store.serialize(format="n3", max_depth=3)
#except:
#        print "No data from serial port: ", sys.exc_info()[1]
#        sys.exit(2)
    
   


    
    
    
    
        




