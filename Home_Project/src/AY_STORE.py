'''
Created on 15 Jun 2012

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

# Define Namespace
hs = Namespace("http://homesensor.com#")


#
SNode1 = URIRef("http://homesensor.com/Node1/")
SNode2 = URIRef("http://homesensor.com/Node2/")
SNode3= URIRef("http://homesensor.com/Node3/")
SNode4= URIRef("http://homesensor.com/Node4/")



mystore_graph_uri = "http://homesensor.com/mystore"
configString = "/var/tmp/mystore"

# Get the Sleepycat plugin.
mystore = plugin.get('Sleepycat', Store)('mystore')
mystore.open("ay_folder", create=True)

#
g = ConjunctiveGraph(store=mystore)
g.bind("homesensor",hs)


#
gNode1 = Graph(store=mystore,identifier = SNode1)
gNode1.open("store_folder", create=True)

gNode2 = Graph(store=mystore,identifier = SNode2)
gNode2.open("store_folder", create=True)

gNode3 = Graph(store=mystore,identifier = SNode3)
gNode3.open("store_folder", create=True)

gNode4 = Graph(store=mystore,identifier = SNode4)
gNode4.open("store_folder", create=True)

# Addition of triples to store

gNode1.add((SNode1, hs['hasTemperature'], Literal('64')))
gNode1.add((SNode1, hs['hasLight'], Literal('50')))
gNode1.add((SNode1, hs['hasHumidity'], Literal('55')))
gNode1.add((SNode1, hs['Located'], Literal('')))
gNode1.add((SNode1, hs['StartTime'],  Literal("2012-06-19T01:52:02Z")))
gNode1.add((SNode1, hs['EndTime'],  Literal("2012-06-19T01:52:02Z")))




gNode2.add((SNode2, hs['hasTemperature'], Literal('64')))
gNode2.add((SNode2, hs['hasLight'], Literal('56')))
gNode2.add((SNode2, hs['hasHumidity'], Literal('40')))
gNode2.add((SNode2, hs['Located'], Literal('')))
gNode2.add((SNode2, hs['StartTime'],  Literal("2012-06-19T01:52:02Z")))
gNode2.add((SNode2, hs['EndTime'],  Literal("2012-06-19T01:52:02Z")))


gNode3.add((SNode3,hs['hasTemperature'], Literal('64')))
gNode3.add((SNode3, hs['hasLight'], Literal('67')))
gNode3.add((SNode3, hs['hasHumidity'], Literal('88')))
gNode3.add((SNode3, hs['Located'], Literal('')))
gNode3.add((SNode3, hs['StartTime'],  Literal("2012-06-19T01:52:02Z")))
gNode3.add((SNode3, hs['EndTime'],  Literal("2012-06-19T01:52:02Z")))



# seperate and display the graphs in n3
print gNode1.serialize(format='n3')
print "==================="

print gNode2.serialize(format='n3')
print "==================="

print gNode3.serialize(format='n3')
print "==================="


print gNode4.serialize(format='n3')
print "==================="



#Display full graph
print g.serialize(format='n3')









