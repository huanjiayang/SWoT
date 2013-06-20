'''
Created on 14 Jun 2012

@author: AYODELE-M.AKINGBULU
'''
'''
Created on 7 Jun 2012

@author: AYODELE-M.AKINGBULU
'''
from rdflib.graph import Graph
from rdflib import Literal, BNode, Namespace,URIRef
from rdflib.store import Store, NO_STORE, VALID_STORE
from rdflib import RDF
import rdfextras
rdfextras.registerplugins()

g = Graph()

# Bind a few prefix, namespace pairs.
g.bind("dc", "http://http://purl.org/dc/elements/1.1/")
g.bind("foaf", "http://xmlns.com/foaf/0.1/")

# Create a namespace object for the Friend of a friend namespace.
FOAF = Namespace("http://xmlns.com/foaf/0.1/")

# Create an identifier to use as the subject for Donna.
donna = BNode()

# Add triples using store's add method.
g.add((donna, RDF.type, FOAF["Person"]))
g.add((donna, FOAF["nick"], Literal("donna", lang="foo")))
g.add((donna, FOAF["name"], Literal("Donna Fales")))

# Iterate over triples in store and print them out.
print("--- printing raw triples ---")
for a in g:
    print(a)

# For each foaf:Person in the store print out its mbox property.
print("--- printing mboxes ---")
for person in g.subjects(RDF.type, FOAF["Person"]):
    for mbox in g.objects(person, FOAF["mbox"]):
        print(mbox)
        
        
