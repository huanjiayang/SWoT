'''
Created on 15 Jun 2012

@author: AYODELE-M.AKINGBULU
'''
import rdflib
import rdfextras
from pyprov.model.core import *
from pyprov.model.common import *
from rdflib.graph import Graph, ConjunctiveGraph
from rdflib.namespace import Namespace
from rdflib.term import Literal
from rdflib.term import URIRef
from rdflib import plugin

from rdflib.store import Store, NO_STORE, VALID_STORE

# Define Namespace
ns = Namespace("http://homesensor.com#")

#
Node1 = URIRef("http://homesensor.com/SN/Node1#")
Node2 = URIRef("http://homesensor.com/SN/Node2#")
Node3= URIRef("http://homesensor.com/SN/Node3#")
Node4= URIRef("http://homesensor.com/SN/Node4#")

#
SNode1 = URIRef("http://homesensor.com/SN/Node1#")
SNode2 = URIRef("http://homesensor.com/SN/Node2#")
SNode3= URIRef("http://homesensor.com/SN/Node3#")
SNode4= URIRef("http://homesensor.com/SN/Node4#")



mystore_graph_uri = "http://homesensor.com/mystore"
configString = "/var/tmp/mystore"

# Get the Sleepycat plugin.
mystore = plugin.get('Sleepycat', Store)('mystore')


#
g = ConjunctiveGraph(store=mystore)
g.bind("homesensor",ns)


# open store if it exists
rt = mystore.open("store_folder", create=False)
if rt == NO_STORE:
    
    # There is no underlying Sleepycat infrastructure, create it
    mystore.open("store_folder", create=True)
else:
    assert rt == VALID_STORE, "The underlying store is corrupt"




#
gNode1 = Graph(store="Sleepycat",identifier = SNode1)

gNode2 = Graph(store="Sleepycat",identifier = SNode2)

gNode3 = Graph(store="Sleepycat",identifier = SNode3)

gNode4 = Graph(store="Sleepycat",identifier = SNode4)


# Addition of triples to store

gNode1.add((URIRef('http://homesensor.com/SN/Node1/Temp'),ns['temperature'], Literal('64')))
gNode1.add((URIRef('http://homesensor.com/SN/Node1/Light'), ns['Light'], Literal('')))
gNode1.add((URIRef('http://homesensor.com/SN/Node1/Humidity'), ns['Humidity'], Literal('')))



gNode2.add((URIRef('http://homesensor.com/SN/Node2/Temp'),ns['temperature'], Literal('64')))
gNode2.add((URIRef('http://homesensor.com/SN/Node2/Light'), ns['Light'], Literal('')))
gNode2.add((URIRef('http://homesensor.com/SN/Node2/Humidity'), ns['Humidity'], Literal('')))


gNode3.add((URIRef('http://homesensor.com/SN/Node3/Temp'),ns['temperature'], Literal('64')))
gNode3.add((URIRef('http://homesensor.com/SN/Node3/Light'), ns['Light'], Literal('')))
gNode3.add((URIRef('http://homesensor.com/SN/Node3/Humidity'), ns['Humidity'], Literal('')))



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









