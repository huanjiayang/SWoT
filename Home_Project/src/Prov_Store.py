'''
Created on 20 Jun 2012

@author: AYODELE-M.AKINGBULU
'''
import rdflib
import rdfextras
from rdflib.graph import Graph, ConjunctiveGraph
from rdflib.namespace import Namespace
from rdflib.term import Literal
from rdflib.term import URIRef
from rdflib import plugin
from rdflib.store import Store, NO_STORE, VALID_STORE

import datetime
from pyprov.model.type import *
from pyprov.model.relation import *
from pyprov.model.bundle import *



# Define Namespace
hs = Namespace("http://homesensor.com#")
prov = Namespace("http://www.w3.org/ns/prov-dm/")

S_Network = hs['SensorNetwork']

sensornetworkURI = 'uri:uuid:sensnetworkayodele001'

createtime = 'datetime...'

#
S_Network = URIRef("http://homesensor.com/")
Readings = hs['Reading']
Communication = URIRef("http://homesensor.com/Activity/")
Discovery = URIRef("http://homesensor.com/Activity/")
S_Node = URIRef("http://homesensor.com/S_node/")
Sensor = URIRef("http://homsensor.com/Sensor/")

graph=Graph(store='Sleepycat',identifier='test')
graph.open("provfolder", create=True)

# Addition of triples to graph





graph.add((S_Network, hs['wasGeneratedBy'], Literal('Network Organization')))
graph.add((S_Network, hs['wasAssociatedWith'], Literal('Communication')))
graph.add((S_Network, hs['wasAssociatedWith'], Literal('Aggregation')))
graph.add((S_Network, hs['wasAssociatedWith'], Literal('Routing')))
graph.add((Communication, hs['wasStartedByActivity'], Literal('Discovery')))
graph.add((Discovery, hs['wasStartedBy'], Literal('Network Organization')))

graph.add((S_Node, hs['actedOnbehalfOf'], Literal('Sensor Network')))
graph.add((S_Node, hs['wasAssociatedWith'], Literal('Discovery')))
graph.add((S_Node, hs['wasAssociatedWith'], Literal('Communication')))
graph.add((Readings, hs['startedAtTime'], Literal("2011-11-16T16:06:00")))
graph.add((Readings, hs['endedAtTime'],  Literal("2012-06-19T01:52:02Z")))
graph.add((Sensor,  hs['actedOnbehalfOf'], Literal('Sensor Node')))


class WSNPROV:
    
    def __init__(self):
        self.provgraph = PROVContainer()
        
    def addNetwork(self,sensornetworkURI,createtime,person):
#        graph.add((sensornetworkURI, RDF.type, S_Network))
#        graph.add((sensornetworkURI, dcterms['create'], createtime))
#        graph.add((sensornetworkURI, prov['wasGeneratedBy'], person))
        
        e0 = Agent(sensornetworkURI,attribute={})
        self.provgraph.add(e0)
    
    def convertDataToPROV(self,rawdata):
        pass
    
    def createWGB(self,para1,para2,para3):
        wGB = wasGeneratedBy(para1,para2,para3)
        return wGB
    
    def createEntity(self):
        pass
    
    def _toRDF(self):
        
        tripledict = {'subject01' : {'predicate01_01' : 'object01_01',},
                      'subject02' : {'pred02_01' : 'obj02_01'}}
        return tripledict
    
    


# commit graph
graph.commit()

print "Triples in graph after add: ", len(graph)


# display the graph in n3
print graph.serialize(format='n3')








