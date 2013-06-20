'''
Created on 14 Jun 2012

@author: AYODELE-M.AKINGBULU
'''
from rdflib import Graph, term, namespace
graph=Graph(store='Sleepycat',identifier='test')
graph.open("somefolder", create=True)
graph.add((term.URIRef('http://www.google.com/'),namespace.RDFS.label, term.Literal('Google home page')))
graph.add((term.URIRef('http://wikipedia.org/'), namespace.RDFS.label, term.Literal('Wikipedia home page')))
graph.close()
graph=Graph(store='Sleepycat',identifier='test')
graph.open("somefolder")
len(graph)

print "things in a_graph"
for s,p,o in graph:
  print s,p,o
