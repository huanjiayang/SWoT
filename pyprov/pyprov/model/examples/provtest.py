import json
import datetime
from pyprov.model.type import *
from pyprov.model.relation import *
from pyprov.model.bundle import *


FOAF = PROVNamespace("foaf","http://xmlns.com/foaf/0.1/")
ex = PROVNamespace("ex","http://www.example.com/")
dcterms = PROVNamespace("dcterms","http://purl.org/dc/terms/")
xsd = PROVNamespace("xsd",'http://www.w3.org/2001/XMLSchema-datatypes#')

testns = PROVNamespace("test",'http://www.test.org/')
exns = PROVNamespace("test",'http://www.example.org/')

examplegraph = PROVContainer()
examplegraph.set_default_namespace("http://www.example.com/")

#add namespaces
#examplegraph.add_namespace("ex","http://www.example.com/")
examplegraph.add_namespace("dcterms","http://purl.org/dc/terms/")
examplegraph.add_namespace("foaf","http://xmlns.com/foaf/0.1/")
#examplegraph.add_namespace("ex","http://www.example111.com/")

# add entities
attrdict = {"type": "File",
            ex["path"]: "/shared/crime.txt",
            ex["creator"]: FOAF["Alice"]}
e = Entity(ex['e0'],attributes=attrdict)
examplegraph.add(e)

attrdict = {"type": "Anything",
            ex["path"]: "/shared/anywhere",
            ex["creator"]: FOAF["Huanjia"]}
e = Entity(ex['e1'],attributes=attrdict)
examplegraph.add(e)



import pprint
pp = pprint.PrettyPrinter(indent=4)
pp.pprint(examplegraph.to_provJSON())