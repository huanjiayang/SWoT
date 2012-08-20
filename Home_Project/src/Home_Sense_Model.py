'''
Created on 17 Aug 2012

@author: AYODELE-M.AKINGBULU
'''
from rdflib.graph import Graph, ConjunctiveGraph
from rdflib import Namespace, BNode, Literal, RDF, URIRef
from Prov_Store import *
import rdfextras
from rdflib import plugin
from rdflib.store import Store, NO_STORE, VALID_STORE
from pyprov.model.type import *
from pyprov.model.relation import *
from pyprov.model.bundle import *
import uuid
import csv

SENSORS = Namespace('sensors',"http://www.homesensor.com/module/sensors#")
DC = Namespace('dc','http://purl.org/dc/elements/1.1/')
FOAF = Namespace('http://xmlns.com/foaf/0.1/')
HS = PROVNamespace('hs',"http://homesensor.com/")
SN = Namespace('sn',"http://homesensor.com/schemas/sensor_network#")
RDF = Namespace('"http://www.w3.org/2000/01/rdf-schema#')
PROV = Namespace("http://www.w3.org/ns/prov-dm/")
rdf = PROVNamespace("rdf","http://www.w3.org/TR/rdf-schema/#")



# --------------------- class begins -------------------------------------------

class Sensor(Entity):
    
    def __init__(self,identifier,sensor_id,timestamp, sensor_name, attributes=None,account=None):
        Entity.__init__(self,identifier=identifier, attributes=attributes, account=account)
        self.identifier 
        self.attributelist.extend
        self.sensor_id
        self.timestamp = timestamp
        
def sensor_Update(self, timestamp, temperature, sensor_type, sensor_instance, sensor_data):
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor']
        return self.rdftriples
    
class Temperature(Sensor):  
    def __init__(self,Temperature_Sensor,activity,identifier,attributes,account): 
        Sensor.__init__(self,identifier,attributes,account) 
        self.activity
        self.identifier
        self._attributelist.extend
        
    
    def get_temperature(self):
        
        return self.read_value('temperature')[0]
#get_sample = self.get_temperature
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor_Temp']
        return self.rdftriples
        
class Humidity(Sensor): 
    def __init__(self,Humidity_Sensor,identifier,attributes,account): 
        Sensor.__init__(self,identifier,attributes,account)
        self.activity
        self.identifier
        self._attributelist.extend      
     
    def get_humidity(self):
         
        return self.read_value('humidity')[0]
    
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor_Humidity']
        return self.rdftriples


        
         
        
class Light(Sensor):    
    def __init__(self,Humidity_Sensor,identifier,attributes,account): 
        Sensor.__init__(self,identifier,attributes,account)      
    
    def get_Light(self):
          
        return self.read_value('Light')[0]
    
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor_Light']
        return self.rdftriples
          
          
          
        