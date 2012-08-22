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


u = uuid.uuid1()
SENSORS = SENSORS(u)

v = uuid.uuid5()
SN = SN(v)


# --------------------- class begins -------------------------------------------

class Sensor_Network(Entity):
    
    def __init__(self, identifier, sensor_id, timestamp=None, sensor_name, attributes=None, account=None):
        Entity.__init__(self, identifier=identifier, attributes=attributes, account=account)
        self.identifier 
        self.attributelist.extend
        self.sensor_id
        self.timestamp = timestamp
        self.uuid = uuid



class Sensor_Node(Sensor_Network):
    
    def __init__(self, identifier, sensor_id, timestamp=None, sensor_name, attributes=None, account=None):
        Sensor_Network.__init__(self, identifier=identifier, attributes=attributes, account=account)
        self.identifier 
        self.attributelist.extend
        self.sensor_id
        self.timestamp = timestamp
        
    def __repr__(self):
        return "SensorReading _sensor_key:%s _sensor_value:%s _sensor_condition:%s" % \
      (self._sensor_key, self._sensor_value, self._sensor_condition)
  
    def __str__(self):
        r_str = self._sensor_key + " = " + self._sensor_value + self.unit_string()
        if (self._sensor_condition):
            r_str += " (" + self._sensor_condition + ")"
        return r_str
    
    def key(self):
        """Return key name (_sensor_key)."""
    ## This is a gratuitous accessor method, but is present for consistency with value()
        return self._sensor_key
    
    
    
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
        
        def _toRDF(self):
            Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor']
        return self.rdftriples

def sensor_value(self):
    """Return reading value."""
    try:
        return int(self._sensor_value)
    except ValueError:
        return float(self._sensor_value)
    
    
Node = Sensor_Node()
Node.sensor_value()    



class Sensor(Sensor_Node):
    def __init__(self, identifier, sensor_id, timestamp=None, sensor_name, attributes=None, account=None, data):
        Sensor_Node.__init__(self, identifier=identifier, attributes=attributes, account=account)
        self.identifier 
        self.attributelist.extend
        self.sensor_id
        self.timestamp = timestamp
        values = sensor_value()
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

    
class Temperature_Sensor(Sensor_Node):  
    def __init__(self, Temperature_Sensor, activity, identifier, attributes, account): 
        Sensor.__init__(self, identifier, attributes, account) 
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
        
class Humidity(Sensor_Node): 
    def __init__(self, Humidity_Sensor, identifier, attributes, account): 
        Sensor.__init__(self, identifier, attributes, account)
        self.activity
        self.identifier
        self._attributelist.extend      
     
    def get_humidity(self):
         
        return self.read_value('humidity')[0]
    
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor_Humidity']
        return self.rdftriples


        
         
        
class Light(Sensor_Node):    
    def __init__(self, Humidity_Sensor, identifier, attributes, account): 
        Sensor.__init__(self, identifier, attributes, account)      
    
    def get_Light(self):
          
        return self.read_value('Light')[0]
    
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor_Light']
        return self.rdftriples
    
class Network_Organization(Activity):
    def __init__(self, identifier=None, attributes=None, account=None):
        Entity.__init__(self, identifier, attributes, account)
        self.type = Network_Organization
        
    
    
class Discovery(Activity):
    def __init__(self, identifier=None, attributes=None, account=None):
        Entity.__init__(self, identifier, attributes, account)
        self.type = Discovery
        
    
class Query(Activity):
    def __init__(self, identifier=None, attributes=None, account=None):
        Entity.__init__(self, identifier, attributes, account)
        self.type = Query
    
class Sensor_Node_Activity(Network_Organization): 
    def __init__(self, identifier=None, attributes=None, account=None):
        Network_Organization.__init__(self, identifier, attributes, account)
        self.type = Query
    
    
class Sensor_Reading_Activity(Activity): 
    def __init__(self, identifier=None, attributes=None, account=None, Sensor_Reading):
        Activity.__init__(self, identifier, attributes, account)
        self.type = Sensor_Reading
        
    
class Sensor_Readings(Entity):
    def __init__(self, identifier=None, attributes=None, account=None):
        Entity.__init__(self, identifier, attributes, account)
        self.type = Query   
    
class observation(Entity):
    def __init__(self, identifier=None, attributes=None, account=None, Observation):
        Entity.__init__(self, identifier, attributes, account)
        self.type = Observation
        
          
          
        
