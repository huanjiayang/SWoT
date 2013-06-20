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

testgraph = PROVContainer()
testgraph.set_default_namespace("http://www.example.com/")

ag1 = Agent(ex['user'],none)
testgraph.add(ag1)

ag2 = Agent(ex['sink node'],none)
testgraph.add(ag2)

ag3 = Agent(ex['sensor node'],none)
testgraph.add(ag3)

e1 = Entity(ex['sensor'],none)
testgraph.add(e1)

e2 = Entity(ex['measured value'],none)
testgraph.add(e2)

e3 = Entity(ex['request'],none)
testgraph.add(e3)

e4 = Entity(ex['command'],none)
testgraph.add(e4)

e5 = Entity(ex['respond'],none)
testgraph.add(e5)

a1 = Activity(ex['sand and recieve'],none)
testgraph.add(a1)

a2 = Activity(ex['measuring'],none)
testgraph.add(a2)

u1 = Used(a2,e1,identifier="u1",time=None,none)
testgraph.add(u1)

u2 = Used(a1,e2,identifier="u2",time=None,none)
testgraph.add(u2)

u3 = Used(a1,e3,identifier="u3",time=None,none)
testgraph.add(u3)

u4 = Used(a1,e4,identifier="u4",time=None,none)
testgraph.add(u4)

u5 = Used(a1,e5,identifier="u5",time=None,none)
testgraph.add(u5)

d1 = wasDerivedFrom(e1,e2,activity=a2,generation=g1,usage=u1,attributes=None)   