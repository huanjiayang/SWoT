'''
Created on 12 Jul 2012

@author: AYODELE-M.AKINGBULU
'''
import rdflib
from rdflib import Namespace, BNode, Literal, RDF, URIRef
from Prov_Store import *
import rdfextras
from rdflib.graph import Graph, ConjunctiveGraph
from rdflib.namespace import Namespace
from rdflib.term import Literal
from rdflib.term import URIRef
from rdflib import Graph, term, namespace
from rdflib import plugin
from rdflib.store import Store, NO_STORE, VALID_STORE
from datetime import date, time, datetime
import datetime

tripledict = {}
starttime = datetime.datetime(2012, 7, 6, 5, 4, 3)
a1 = "starter"
a2 =   "started"
ag0 = Agent(sensornetworkURI)
a0 = Activity("a0",starttime=starttime,attributes=tripledict)
e0 = Entity(identifier="e0")
e1 = Entity(FOAF['Foo'],attributes=tripledict)
g0 = wasGeneratedBy(e0,a0,identifier="g0",time=None,attributes=tripledict)
u0 = Used(a0,e1,identifier="u0",time=None,attributes=tripledict)
d0 = wasDerivedFrom(e0, e1, activity=a0,generation=g0,usage=u0,attributes=None)
f0 = wasGeneratedBy(e0,g0,time=None,attributes=None)
w0 = wasAssociatedWith(ag0,a0,identifier="w0",attributes=tripledict)
s0 = wasStartedByActivity(a0,a1,a2,attributes=None)
b0 = wasStartedBy(e0,a0,identifier="b0",attributes=None)

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

sensor_graph=Mystore('Sleepycat', 'mystore')
sensor_graph.open("provfolder",create=False)
sensor_graph.addPROVInstance(e0)
sensor_graph.addPROVInstance(w0)
sensor_graph.addPROVInstance(a0)
sensor_graph.addPROVInstance(f0)
sensor_graph.addPROVInstance(ag0)
sensor_graph.addPROVInstance(s0)
sensor_graph.addPROVInstance(b0)
sensor_graph.addPROVInstance(d0)
sensor_graph.addPROVInstance(e1)
sensor_graph.addPROVInstance(g0)




sensor_graph.add((term.URIRef('http://www.homesensor.com/'),namespace.RDFS.label, term.Literal('Google home page')))
sensor_graph.add((term.URIRef('http://wikipedia.org/'), namespace.RDFS.label, term.Literal('Wikipedia home page')))
sensor_graph.add((sensornetworkURI, DC['create'], createtime))
sensor_graph.add((sensornetworkURI, prov['wasGeneratedBy'], person))
sensor_graph.add((person, FOAF["name"], Literal("Agent")))
sensor_graph.close()









print sensor_graph.serialize()









