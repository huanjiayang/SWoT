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


class Sensor(Entity):
    
    def __init__(self,identifier,attributes=None,account=None):
        Entity.__init__(self,identifier=identifier, attributes=attributes, account=account)
        
    def _toRDF(self):
        Entity._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor']
        return self.rdftriples