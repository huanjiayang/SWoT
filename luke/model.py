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

DC = Namespace('dc', 'http://purl.org/dc/elements/1.1/')
FOAF = Namespace('http://xmlns.com/foaf/0.1/')
PROV = Namespace("http://www.w3.org/ns/prov-dm/")
RDF = Namespace('"http://www.w3.org/2000/01/rdf-schema#')
MT = PROVNamesace('mt', "http://www.mytype.com/#")


class Sink_Node(Agent):
    
    def __init__(self, identifier, attributes = None, account = None):
        
        Agent.__init__(self, identifier=identifier,attributes=attributes,account=account)
        
    def _toRDF(self):
        Agent._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = MT['Sink_Node']
        
        return self.rdftriples
    
    
class Sensor_Node(Agent):
    
    def __init__(self, identifier, attributes = None, account = None):

        Agent.__init__(self, identifier=identifier,attributes=attributes,account=account)
        
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
    
    def __init__(self, identifier=None, starttime=None, endtime=None, attributes=None, account=None, sensor_name=None, sensor_id=None):
        
        Activity.__init__(self, indentifier=identifier, starttime=starttime, endtime=endtime, attributes=attributes, account=account)
        
        self.indentifier = identifier
        
    def _toRDF(self):
        Activity.to_RDF(self)
        self.rdftriples[self.identifier][rdf['type']] = MT['Measuring']
        if self.starttime is not None:
            self.rdftriples[self.identifier][MT['starttime']] = MT[self.starttime]
        if self.endtime is not None:
            self.rdftriples[self.identifier][MT['endtime']] = MT[self.endtime]
        return self.rdftriples


class Sand_and_Recieve(Activity):
    
    def __init__(self, identifier=None, starttime=None, endtime=None, attributes=None, account=None, sensor_name=None, sensor_id=None):
        
        Activity.__init__(self, indentifier=identifier, starttime=starttime, endtime=endtime, attributes=attributes, account=account)
        
        self.indentifier = identifier
        
    def _toRDF(self):
        Activity.to_RDF(self)
        self.rdftriples[self.identifier][rdf['type']] = MT['Sand_and_Recieve']
        if self.starttime is not None:
            self.rdftriples[self.identifier][MT['starttime']] = MT[self.starttime]
        if self.endtime is not None:
            self.rdftriples[self.identifier][MT['endtime']] = MT[self.endtime]
        return self.rdftriples
    
    
class Measured_value(Entity):
    
    def __init__(self, identifier=None, attributes=None, value=None, sensor_name=None, sensor_id=None,account=None):
        
        Entity.__init__(self, identifier=identifier, attributes=attributes, account=account)
        self.value = value
        self.identifier = identifier
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = MT['Measured_value']
        self.rdftriples[self.identifier][MT['value']] = Literal(self.value)
        return self.rdftriples
    

class Sensor(Entity):
    
    def __init__(self, identifier=None, attributes=None, sensor_name=None, sensor_id=None, account=None):
        
        Entity.__init__(self, identifier=idetifier, attributes=attributes, account=account)
        self.identifier = identifier
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = MT['Sensor']
        return self.rdftriples


class Request(Entity):
    
    def __init__(self, identifier=None, attributes=None, account=None):
        
        Entity.__init__(self, identifier=idetifier, attributes=attributes, account=account)
        self.identifier = identifier
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = MT['request']
        return self.rdftriples
    
    
class Command(Entity):
    
    def __init__(self, identifier=None, attributes=None, account=None):
        
        Entity.__init__(self, identifier=idetifier, attributes=attributes, account=account)
        self.identifier = identifier
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = MT['Command']
        return self.rdftriples
    
    
class Respond(Entity):
    
    def __init__(self, identifier=None, attributes=None, account=None):
        
        Entity.__init__(self, identifier=idetifier, attributes=attributes, account=account)
        self.identifier = identifier
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = MT['Respond']
        return self.rdftriples