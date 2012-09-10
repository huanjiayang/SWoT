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



SENSORS = Namespace('sensors', "http://www.homesensor.com/module/sensors#")
DC = Namespace('dc', 'http://purl.org/dc/elements/1.1/')
FOAF = Namespace('http://xmlns.com/foaf/0.1/')
HS = PROVNamespace('hs', "http://homesensor.com/")
SN = Namespace('sn', "http://homesensor.com/schemas/sensor_network#")
RDF = Namespace('"http://www.w3.org/2000/01/rdf-schema#')
PROV = Namespace("http://www.w3.org/ns/prov-dm/")
rdf = PROVNamespace("rdf", "http://www.w3.org/2000/01/rdf-schema#")


#u = str(uuid.uuid1())
#SENSORS = SENSORS(u)
#
#v = str(uuid.uuid1())

#SN = SN(v)


# --------------------- class begins -------------------------------------------

class Sensor_Network(Entity):
    
    def __init__(self, identifier=None, attributes=None, account=None):
        if identifier is None:
            identifer = 'urn:uuid:' + str(uuid.uuid1())
        Entity.__init__(self, identifier=identifier, attributes=attributes, account=account)

    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor_Network']
        return self.rdftriples



class Sensor_Node(Agent):
    
    def __init__(self, identifier,attributes, sensor_id, sensor_name, account=None):
        #self.sensor_id = sensor_id
        
#        self.attributes.update({HS["sensor_id"]:sensor_id,HS["sensor_name"]:sensor_name})
#        if identifier is None:
#            identifier = 'urn:uuid:' + str(uuid.uuid1())
        Agent.__init__(self, identifier=identifier,attributes=attributes,account=account)
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor_Node']
        
        return self.rdftriples
        
        



class Sensor(Agent):
    def __init__(self, identifier=None, sensor_type=None, attributes=None, account=None):
        if identifier is None:
            identifer = 'urn:uuid:' + str(uuid.uuid1())
        Agent.__init__(self, identifier=identifier, attributes=attributes, account=account)
         
        self.sensor_type = sensor_type
    
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor']
        self.rdftriples[self.identifier][HS['sensor_type']] = HS[self.sensor_type]
        return self.rdftriples

    

class Network_Organization(Activity):
    def __init__(self, identifier=None, attributes=None, account=None, sensor_name=None,starttime=None, endtime=None):
        
        if identifier is None:
            identifer = 'urn:uuid:' + str(uuid.uuid1()) 
        Activity.__init__(self, identifier,starttime,endtime, attributes, account)  
        #Entity.__init__(self, identifier, attributes, account)
        self.identifier = identifier
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Network_Organization']
        return self.rdftriples
        
    
    
class Discovery(Activity):
    def __init__(self, identifier=None,starttime=None,endtime=None,attributes=None, account=None, sensor_name=None, sensor_id=None):
        Activity.__init__(self, identifier,starttime,endtime, attributes, account)
        self.identifier = identifier
        self.type = Discovery
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Discovery']
        return self.rdftriples
        
    
class Query(Activity):
    def __init__(self, identifier=None,starttime=None, endtime=None, attributes=None, account=None):
        Activity.__init__(self, identifier,starttime,endtime, attributes, account)
        self.identifier = identifier
        
    def _toRDF(self):
        Activity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Query']
        return self.rdftriples
    
class Sensor_Node_Activity(Network_Organization): 
    def __init__(self, identifier=None, attributes=None, account=None, Sensor_Reading=None):
        Activity.__init__(identifier=identifier,starttime=None,endtime=None,attributes=attributes,account=account)
        self.identifier = identifier
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor_Node_Activity']
        return self.rdftriples
    
    
class Sensor_Reading_Activity(Activity): 
    def __init__(self, identifier=None, attributes=None, account=None, starttime=None, endtime=None, Sensor_Reading=None):
        Activity.__init__(self, identifier=identifier,starttime=starttime,endtime=endtime,attributes=attributes,account=account)
        self.identifer = identifier
        
   
    def _toRDF(self):
        Activity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor_Reading_Activity']
        return self.rdftriples
        
    
class Sensor_Readings(Entity):
    def __init__(self, identifier=None, attributes=None, account=None, value=None):
        Entity.__init__(self,identifier=identifier,attributes=attributes,account=account)
        self.value = value
        self.identifier = identifier
        
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor_Readings']
        self.rdftriples[self.identifier][HS['value']] = Literal(self.value)
        return self.rdftriples 
      
    
#class observation(Entity):
#    def __init__(self, identifier=None, attributes=None, account=None, Observation=None,Temperature=None,Humidity=None,Light=None):
#        Entity.__init__(self, identifier, attributes, account)
#        self.value = Temperature
#        self.Humidity = Humidity
#        self.Light  = Light
#        
#        
#    def _toRDF(self):
#        Entity._toRDF(self)
#        self.rdftriples[self.identifier][rdf['type']] = HS['Observation']
#        return self.rdftriples
        
          
          
        
