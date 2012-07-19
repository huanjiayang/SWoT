'''
Created on 12 Jul 2012

@author: AYODELE-M.AKINGBULU
'''
import rdflib
from rdflib import Namespace, BNode, Literal, RDF, URIRef
from RDF_STORE import *
import rdfextras
from rdflib.graph import Graph, ConjunctiveGraph
from rdflib.namespace import Namespace
from rdflib.term import Literal
from rdflib.term import URIRef
from rdflib import plugin
from rdflib.store import Store, NO_STORE, VALID_STORE
from datetime import date, time, datetime



DC = Namespace('http://purl.org/dc/elements/1.1/')
FOAF = Namespace('http://xmlns.com/foaf/0.1/')
HS = Namespace('http://homesensor.com/#')
RDFS = Namespace('"http://www.w3.org/2000/01/rdf-schema#')
prov = Namespace("http://www.w3.org/ns/prov-dm/")
S_Network = HS['SensorNetwork']
createtime = 'datetime...'
person = 'ayo001'

sensornetworkURI = 'uri:uuid:sensnetworkayodele001'
sensornode1URI = 'uri:uuid:sensornode01'
sensornode2URI = 'uri:uuid:sensornode02'
sensornode3URI = 'uri:uuid:sensornode03'
sensortemp1URI = 'uri:uuid:sensorTEMP01'
sensortemp2URI = 'uri:uuid:sensorTEMP02'
sensortemp3URI = 'uri:uuid:sensorTEMP03'
sensorlight1URI = 'uri:uuid:sensorLIGHT01'
sensorlight2URI = 'uri:uuid:sensorLIGHT02'
sensorlight3URI = 'uri:uuid:sensorLIGHT03'
sensorHum1URI = 'uri:uuid:sensorHUM01'
sensorHum1URI = 'uri:uuid:sensorHUM02'
sensorHum1URI = 'uri:uuid:sensorHUM03'


sensor_graph=RDFSTORE()
sensor_graph.add((sensornetworkURI, RDF.type, S_Network))
sensor_graph.add((sensornetworkURI, DC['create'], createtime))
sensor_graph.add((sensornetworkURI, prov['wasGeneratedBy'], person))
sensor_graph.add((person, FOAF["name"], Literal("Agent")))


#print(sensor_graph.triples(('sensornetworkURI', 'wasGeneratedBy',None)))

#print(sensor_graph.triples((None,'RDF.type',None)))

#sensor_graph.value('sensornetworkURI','wasGeneratedBy',None)

print sensor_graph.serializess(format='n3')



