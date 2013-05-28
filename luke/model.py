from rdflib.graph import Graph, ConjunctiveGraph
from rdflib import Namespace, BNode, Literal, RDF, URIRef
#from Prov_Store import *
import rdfextras
from rdflib import plugin
from rdflib.store import Store, NO_STORE, VALID_STORE
from pyprov.model.type import *
from pyprov.model.relation import *
from pyprov.model.bundle import *
import uuid

DC = Namespace('dc', 'http://purl.org/dc/elements/1.1/')
FOAF = Namespace('http://xmlns.com/foaf/0.1/')
PROV = Namespace("http://www.w3.org/ns/prov-dm/")
RDF = Namespace('"http://www.w3.org/2000/01/rdf-schema#')
MT = PROVNamespace('mt', "http://www.mytype.com/#")


class Sink_Node(Agent):
    
    def __init__(self, identifier, attributes = None,account = None):
        
        Agent.__init__(self, identifier=identifier,attributes=attributes,account=account)
        
        if self.attributes == None:
            self.attributes = {}
        self.attributes[MT['type']]=MT['Sink_Node']
        self.value_type = None
        self.sensor_id = None
        self.sensor_node_id = None
                
    def _toRDF(self):
        Agent._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = MT['Sink_Node']
        
        return self.rdftriples
    
    
class Sensor_Node(Agent):
    
    def __init__(self, identifier, Sink_Node, attributes = None, account = None):

        Agent.__init__(self, identifier=identifier,attributes=attributes,account=account)
        
        if self.attributes == None:
            self.attributes = {}
        self.attributes[MT['type']]=MT['Sensor_Node']
        self.attributes[MT['Sink_Node']]=Sink_Node
        self.value_type = None
        self.sensor_id = None
        self.sensor_node_id = identifier
        
            
    def _toRDF(self):
        Agent._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = MT['Sensor_Node']
        
        return self.rdftriples
    

class User(Agent):
    
    def __init__(self, identifier, attributes = None, account = None):

        Agent.__init__(self, identifier=identifier,attributes=attributes,account=account)
        
    def _toRDF(self):
        Agent._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = MT['User']
        
        return self.rdftriples
    
    
class Measuring(Activity):
    
    def __init__(self, Sensor_id, value_type, identifier=None, starttime=None, endtime=None, attributes=None, account=None):
        
        if identifier is None:
            identifier = 'urn:uuid:' + str(uuid.uuid1())
            
        Activity.__init__(self, identifier=identifier, starttime=starttime, endtime=endtime, attributes=attributes, account=account)
            
        self.identifier = identifier
        
        if self.attributes == None:
            self.attributes = {}
        self.attributes[MT['type']]=MT['Measuring']
        self.attributes[MT['Sensor_id']]=Sensor_id
        self.value_type = value_type
        self.sensor_id = Sensor_id
        self.sensor_node_id = None
        
        
    def _toRDF(self):
        Activity.to_RDF(self)
        self.rdftriples[self.identifier][rdf['type']] = MT['Measuring']
        if self.starttime is not None:
            self.rdftriples[self.identifier][MT['starttime']] = MT[self.starttime]
        if self.endtime is not None:
            self.rdftriples[self.identifier][MT['endtime']] = MT[self.endtime]
        return self.rdftriples


class Send_and_Recieve(Activity):
    
    def __init__(self, identifier=None, starttime=None, endtime=None, attributes=None, account=None, sensor_name=None, sensor_id=None):
        
        Activity.__init__(self, identifier=identifier, starttime=starttime, endtime=endtime, attributes=attributes, account=account)
        
        #self.identifier = identifier
        
    def _toRDF(self):
        Activity.to_RDF(self)
        self.rdftriples[self.identifier][rdf['type']] = MT['Send_and_Recieve']
        if self.starttime is not None:
            self.rdftriples[self.identifier][MT['starttime']] = MT[self.starttime]
        if self.endtime is not None:
            self.rdftriples[self.identifier][MT['endtime']] = MT[self.endtime]
        return self.rdftriples
    
    
class Measured_Value(Entity):
    
    def __init__(self, value, value_type, Sensor_id, identifier=None, attributes=None, account=None):
        
        if identifier is None:
            identifier = 'urn:uuid:' + str(uuid.uuid1())
            
        Entity.__init__(self, identifier=identifier, attributes=attributes, account=account)
        self.value=value
        self.identifier=identifier
        self.value_type=value_type
        self.sensor_node_id = None

        
        if self.attributes == None:
            self.attributes = {}
        self.attributes[MT['type']]=MT['Measured_Value']
        self.attributes[MT['Sensor_id']]=Sensor_id
        self.attributes[MT['value_type']]=value_type
        self.attributes[MT['value']]=Literal(self.value)
        self.value_type = value_type
        self.sensor_id = Sensor_id   
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = MT['Measured_Value']
        self.rdftriples[self.identifier][MT['value']] = Literal(self.value)
        return self.rdftriples
    

class Sensor(Entity):
    
    def __init__(self, identifier, Sensor_Node, value_type, attributes=None, account=None):
        
        Entity.__init__(self, identifier=identifier, attributes=attributes, account=account)
        self.identifier = identifier
        
        if self.attributes == None:
            self.attributes = {}
        self.attributes[MT['type']]=MT['Sensor']
        self.attributes[MT['Sensor_Node']]=Sensor_Node
        self.value_type = value_type
        self.sensor_id = identifier
        self.sensor_node_id = Sensor_Node
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = MT['Sensor']
        return self.rdftriples


class Request(Entity):
    
    def __init__(self, identifier=None, attributes=None, account=None):
        
        Entity.__init__(self, identifier=identifier, attributes=attributes, account=account)
        self.identifier = identifier
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = MT['request']
        return self.rdftriples
    
    
class Command(Entity):
    
    def __init__(self, identifier=None, attributes=None, account=None):
        
        Entity.__init__(self, identifier=identifier, attributes=attributes, account=account)
        self.identifier = identifier
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = MT['Command']
        return self.rdftriples
    
    
class Respond(Entity):
    
    def __init__(self, identifier=None, attributes=None, account=None):
        
        Entity.__init__(self, identifier=identifier, attributes=attributes, account=account)
        self.identifier = identifier
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = MT['Respond']
        return self.rdftriples