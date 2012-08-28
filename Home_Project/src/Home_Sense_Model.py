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




SENSORS = Namespace('sensors', "http://www.homesensor.com/module/sensors#")
DC = Namespace('dc', 'http://purl.org/dc/elements/1.1/')
FOAF = Namespace('http://xmlns.com/foaf/0.1/')
HS = PROVNamespace('hs', "http://homesensor.com/")
SN = Namespace('sn', "http://homesensor.com/schemas/sensor_network#")
RDF = Namespace('"http://www.w3.org/2000/01/rdf-schema#')
PROV = Namespace("http://www.w3.org/ns/prov-dm/")
rdf = PROVNamespace("rdf", "http://www.w3.org/TR/rdf-schema/#")


#u = uuid.uuid1()
#SENSORS = SENSORS(u)
#
#v = uuid.uuid1()
#SN = SN(v)


# --------------------- class begins -------------------------------------------

class Sensor_Network(Entity):
    
    def __init__(self, identifier=None, attributes=None, account=None):
        if identifier is None:
            identifer = 'urn:uuid:' + str(uuid.uuid1())
        Entity.__init__(self, identifier=identifier, attributes=attributes, account=account)

    def to_RDF(self):
        Entity.to_RDF(self)
        self.rdftriples[self.identifier][rdf['type']] = prov['Sensor_Network']
        return self.rdftriples



class Sensor_Node(Agent):
    
    def __init__(self, identifier, sensor_id, sensor_name, attributes, account):
        #self.sensor_id = sensor_id
        
        self.get_record_attributes().update({HS["sensor_id"]:sensor_id,
                           HS["sensor_name"]:sensor_name})
        if identifier is None:
            identifier = 'urn:uuid:' + str(uuid.uuid1())
        Agent.__init__(self, identifier=identifier, attributes=attributes, account=account)
        
        
#    def __repr__(self):
#        return "SensorReading _sensor_key:%s _sensor_value:%s _sensor_condition:%s" % \
#      (self._sensor_key, self._sensor_value, self._sensor_condition)
#  
#    def __str__(self):
#        r_str = self._sensor_key + " = " + self._sensor_value + self.unit_string()
#        if (self._sensor_condition):
#            r_str += " (" + self._sensor_condition + ")"
#        return r_str
    
    def get_node(self, node_id, data):
        self.Temp
        self.Humidity
        self.Light
        for line in data:
            Node, Temp, Humidity, Light = line.split(',', 3)
        if int(Node) == int(node_id):
            return Node
        
    def get_sensor_type(self, Node, data, sensor_type):
        for line in data:
            Temp, Humidity, Light = line.split(',', 3)
        if self.sensor_type == "Temperature":
            return Temp
        elif self.type == "Humidity":
            return Humidity
        elif self.type == "Light":
            return Light
        else:
            return "null"
      
        
    def sensor_Update(self, timestamp, temperature, Light, Humidity, sensor_type, sensor_instance, sensor_data):
        pass
    
    def sensor_value(self):
        """Return reading value."""
        try:
            return int(self._sensor_value)
        except ValueError:
            return float(self._sensor_value)
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor_Node']
        return self.rdftriples


    
    
#Node = Sensor_Node()
#Node.sensor_value()    



class Sensor(Agent):
    def __init__(self, identifier, sensor_id,sensor_name, attributes, account):
        self.get_record_attributes().update({HS["sensor_id"]:sensor_id,
                           HS["sensor_name"]:sensor_name})
        if identifier is None:
            identifer = 'urn:uuid:' + str(uuid.uuid1())
        Agent.__init__(self, identifier=identifier, attributes=attributes, account=account)
         
        
        
    def sensor_value(self,data):
        """Return reading value."""
        try:
            return int(self._sensor_value)
        except ValueError:
            return float(self._sensor_value)
        values = self.sensor_value
        data.append(values)
        if len(data) > 0:
            data.pop(0)
            result = []
        for sensor in data[0]:
            result.append([])
        for sample in data:
            for sensor_id in range(len(sample)):
                value = sample[sensor_id]
        result[sensor_id].append(value)
        
    
        
    def to_RDF(self):
        Entity.to_RDF(self)
        self.rdftriples[self.identifier][rdf['type']] = prov['Sensor']
        return self.rdftriples

    
class Temperature_Sensor(Sensor):  
    def __init__(self, activity,sensor_id=None,sensor_name=None, identifier=None, attributes=None, account=None): 
        self.get_record_attributes().update({HS["sensor_id"]:sensor_id,
                           HS["sensor_name"]:sensor_name})
        if identifier is None:
            identifer = 'urn:uuid:' + str(uuid.uuid1())
        Sensor.__init__(self, identifier, sensor_id,sensor_name, attributes, account) 

        self.identifier = identifier
        self._attributelist.extend
        
    
    def get_temperature(self):
        
        return self.read_value('temperature')[0]
    
#get_sample = self.get_temperature
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor_Temp']
        return self.rdftriples
        
class Humidity_Sensor(Sensor): 
    def __init__(self,sensor_id,sensor_name, identifier, attributes=None, account=None): 
        self.get_record_attributes().update({HS["sensor_id"]:sensor_id,
                           HS["sensor_name"]:sensor_name})
        if identifier is None:
            identifer = 'urn:uuid:' + str(uuid.uuid1())
        Sensor.__init__(self, identifier,sensor_id,sensor_name, attributes=attributes, account=account)

        
        self._attributelist.extend      
     
    def get_humidity(self):
         
        return self.read_value('humidity')[0]
    
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor_Humidity']
        return self.rdftriples


        
         
        
class Light_Sensor(Sensor):    
    def __init__(self,sensor_id,sensor_name, identifier, attributes, account):
        self.get_record_attributes().update({HS["sensor_id"]:sensor_id,
                           HS["sensor_name"]:sensor_name})
        if identifier is None:
            identifer = 'urn:uuid:' + str(uuid.uuid1()) 
        Sensor.__init__(self, identifier, attributes, account,attributes=attributes, account=account)      
    
    def get_Light(self):
          
        return self.read_value('Light')[0]
    
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor_Light']
        return self.rdftriples
    
class Network_Organization(Activity):
    def __init__(self, identifier=None, attributes=None, account=None, network_id = None, sensor_name=None,starttime=None, endtime=None,):
        
        if identifier is None:
            identifer = 'urn:uuid:' + str(uuid.uuid1()) 
        Activity.__init__(self, identifier, attributes, account)  
        #Entity.__init__(self, identifier, attributes, account)
        self.identifier = identifier
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Network_Organization']
        return self.rdftriples
        
    
    
class Discovery(Activity):
    def __init__(self, identifier=None, attributes=None, account=None, sensor_name=None, sensor_id=None):
        Activity.__init__(self, identifier, attributes, account)
        self.identifier = identifier
        self.type = Discovery
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Discovery']
        return self.rdftriples
        
    
class Query(Activity):
    def __init__(self, identifier=None, attributes=None, account=None):
        Activity.__init__(self, identifier, attributes, account)
        self.identifier = identifier
        
    def _toRDF(self):
        Activity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Query']
        return self.rdftriples
    
class Sensor_Node_Activity(Network_Organization): 
    def __init__(self, identifier=None, attributes=None, account=None):
        Network_Organization.__init__(self, identifier, attributes, account)
        self.identifier = identifier
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor_Node_Activity']
        return self.rdftriples
    
    
class Sensor_Reading_Activity(Activity): 
    def __init__(self, identifier=None, attributes=None, account=None, Sensor_Reading=None):
        Activity.__init__(self, identifier, attributes, account)
        self.identifer = identifier
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor_Reading_Activity']
        return self.rdftriples
        
    
class Sensor_Readings(Entity):
    def __init__(self, identifier=None, attributes=None, account=None, reading=None):
        Entity.__init__(self, identifier, attributes, account)
        self.reading = reading
        self.identifier = identifier
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor_Readings']
        return self.rdftriples   
    
class observation(Entity):
    def __init__(self, identifier=None, attributes=None, account=None, Observation=None,Temperature=None,Humidity=None,Light=None):
        Entity.__init__(self, identifier, attributes, account)
        self.value = Temperature
        self.Humidity = Humidity
        self.Light  = Light
        
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Observation']
        return self.rdftriples
        
          
          
        
