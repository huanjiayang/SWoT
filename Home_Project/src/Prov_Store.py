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
from pyprov.model.core import *
from pyprov.model.common import *




# Define Namespace
hs = Namespace("http://homesensor.com#")


#
S_Network = URIRef("http://homesensor.com/")
Reading = URIRef("http://homesensor.com/Activity/")
S_Node = URIRef("http://homesensor.com/S_node/")
Sensor = URIRef("http://homsensor.com/Sensor/")

graph=Graph(store='Sleepycat',identifier='test')
graph.open("provfolder", create=True)

# Addition of triples to graph

graph.add((S_Network, hs['wasGeneratedBy'], Literal('Network Organization')))
graph.add((S_Network, hs['wasAssociatedWith'], Literal('Communication')))
graph.add((S_Node, hs['actedOnbehalfOf'], Literal('Sensor Network')))
graph.add((Reading, hs['startedAtTime'], Literal("2011-11-16T16:06:00")))
graph.add((Reading, hs['endedAtTime'],  Literal("2012-06-19T01:52:02Z")))
graph.add((Sensor,  hs['actedOnbehalfOf'], Literal('Sensor Node')))


# commit graph
graph.commit()

print "Triples in graph after add: ", len(graph)


# display the graph in n3
print graph.serialize(format='n3')








