'''
Created on 14 Jun 2012

@author: AYODELE-M.AKINGBULU
'''
#from rdflib import Graph

import random, sys
import pkg_resources
pkg_resources.require("rdflib>=3.1.0")

from rdflib import URIRef,Literal, Namespace, Graph, ConjunctiveGraph, RDF
import rdflib

pkg_resources.require("rdfextras>=0.1")
from rdfextras import *

file_store=rdflib.plugin.get('Sleepycat',rdflib.store.Store)()

code = file_store.open("test.db", create=True)
if code != rdflib.store.VALID_STORE:
  print "something went wrong"
  sys.exit(-1)

all_graphs = ConjunctiveGraph(store = file_store)
print "things already in the store"
for s,p,o,m in all_graphs.quads((None, None, None)):
  print s,p,o,m

file_store.bind('dc', Namespace("http://purl.org/dc/elements/1.1/"))

dc = Namespace("http://purl.org/dc/elements/1.1/")

a_graph = Graph(store=file_store, identifier=URIRef("http://example.com/graph1"))

a_graph.add( (URIRef("http://example.com/i1"), dc["creator"], Literal("Author "+str(random.randint(1,1000))) ) )

print "things in a_graph"
for s,p,o in a_graph:
  print s,p,o

b_graph = Graph(store=file_store, identifier=URIRef("http://example.com/graph2"))

b_graph.add( (URIRef("http://example.com/i2"), dc["creator"], Literal("Author "+str(random.randint(1,1000))) ) )

print "things in b_graph"
for s,p,o in b_graph:
  print s,p,o

print "things in store"
for s,p,o in all_graphs:
  print s,p,o

