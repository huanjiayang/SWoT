import json
from provpy import *
import rdflib
from rdflib import Namespace


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
e0 = Entity(id=None,attributes=attrdict)
examplegraph.add(e0)
lit0 = PROVLiteral("2011-11-16T16:06:00",xsd["dateTime"])
attrdict ={"type": "File",
           ex["path"]: "/shared/crime.txt",
           dcterms["creator"]: [FOAF['Alice'],FOAF['Bill'],exns['Foo']],
           ex["content"]: "",
           dcterms["create"]: lit0,
           exns["testns"]:testns["localname"]}
e1 = Entity(FOAF['Foo'],attributes=attrdict)
examplegraph.add(e1)

# add activities
attrdict = {"recipeLink": "create-file"}
#starttime = datetime.datetime.utcnow().isoformat()
starttime = datetime.datetime(2008, 7, 6, 5, 4, 3)
a0 = Activity("a0",starttime=starttime,attributes=attrdict)
examplegraph.add(a0)

# add relation 
attrdict={ex["fct"]: "create"}
g0=wasGeneratedBy(e0,a0,id="g0",time=None,attributes=attrdict)
examplegraph.add(g0)

attrdict={ex["fct"]: "load",
          ex["typeexample"] : PROVLiteral("MyValue",ex["MyType"])}
u0 = Used(a0,e1,id="u0",time=None,attributes=attrdict)
examplegraph.add(u0)

d0=wasDerivedFrom(e0,e1,activity=a0,generation=g0,usage=u0,attributes=None)
examplegraph.add(d0)

#add accounts
acc0 = Account("acc0",ex["asserter_name"])
acc0.add_namespace("ex","http://www.example2222.com/")
#acc0.add_namespace('ex','www.example.com')
examplegraph.add(acc0)

acc0.add_entity()

en = examplegraph.add_entity('en',account=acc0)

#print json.dumps(examplegraph.to_provJSON(),indent=4)

import pprint
pp = pprint.PrettyPrinter(indent=4)
pp.pprint(examplegraph.to_provJSON())

nsdict = {'foaf' : "http://xmlns.com/foaf/0.1/",
            'ex' : "http://www.example.com/",
            'dcterms' : "http://purl.org/dc/terms/"}
testuri = testns['localname']
print testuri.name
print testuri.namespacename
print testuri.localname
print testuri.qname({"testprefix" : "http://www.test.org/"})

print u0.to_provJSON(nsdict)
print testuri.to_provJSON({'test':'http://www.test.org/'})

print d0.to_provJSON(nsdict)


ttt = PROVLiteral("MyValue",ex["MyType"])
print isinstance(ttt,PROVLiteral)

f = open('C:/exampleresult.json', 'w')
f.write(json.dumps(examplegraph.to_provJSON(),indent=4))

nsnsdict = {   'dcterms': 'http://purl.org/dc/terms/',
                  'default': 'http://www.example.com/',
                  'foaf': 'http://xmlns.com/foaf/0.1/',
                  'ns0': 'http://www.test.org/',
                  'prov': 'http://www.w3.org/ns/prov-dm/',
                  'test': 'http://www.example.org/',
                  'xsd': 'http://www.w3.org/2001/XMLSchema-datatypes#'}
print ex["fct"].qname(nsnsdict)


