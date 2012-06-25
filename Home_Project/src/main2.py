'''
Created on 11 Jun 2012

@author: AYODELE-M.AKINGBULU
'''
import rdflib
from rdflib.graph import ConjunctiveGraph as Graph
from rdflib import plugin
from rdflib.store import Store, NO_STORE, VALID_STORE
from rdflib.namespace import Namespace
from rdflib.term import Literal
from rdflib.term import URIRef
from tempfile import mkdtemp

default_graph_uri = "http://rdflib.net/rdfstore"
configString = "/var/tmp/rdfstore"

# Get the Sleepycat plugin.
store = plugin.get('Sleepycat', Store)('rdfstore')

# Open previously created store, or create it if it doesn't exist yet
graph = Graph(store="Sleepycat",
              identifier = URIRef(default_graph_uri))
path = mkdtemp()
rt = graph.open(path, create=False)
if rt == NO_STORE:
    # There is no underlying Sleepycat infrastructure, create it
    graph.open(path, create=True)
else:
    assert rt == VALID_STORE, "The underlying store is corrupt"

print "Triples in graph before add: ", len(graph)

# Now we'll add some triples to the graph & commit the changes
rdflib = Namespace('http://rdflib.net/test/')
graph.bind("test", "http://rdflib.net/test/")

graph.add((rdflib['pic:1'], rdflib['name'], Literal('Jane & Bob')))
graph.add((rdflib['pic:2'], rdflib['name'], Literal('Squirrel in Tree')))
graph.commit()

print "Triples in graph after add: ", len(graph)

# display the graph in RDF/XML
print graph.serialize()
