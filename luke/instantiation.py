import model
import json
import datetime
from pyprov.model.type import *
from pyprov.model.relation import *
from pyprov.model.bundle import *


DC = Namespace('dc', 'http://purl.org/dc/elements/1.1/')
FOAF = Namespace('http://xmlns.com/foaf/0.1/')
PROV = Namespace("http://www.w3.org/ns/prov-dm/")
RDF = Namespace('"http://www.w3.org/2000/01/rdf-schema#')
MT = PROVNamesace('mt', "http://www.mytype.com/#")

InsGraph = PROVContainer()
InsGraph.set_default_namespace("http://www.mytype.com/#")

ag1 = Sensor_Node(MT['Sensor_Node'],None)
InsGraph.add(ag1)

ag2 = Sink_Node(MT['Sink_Node'],None)
InsGraph.add(ag2)

e1 = Sensor(MT['Sensor'],None)
InsGraph.add(e1)

e2 = Measured_Value(MT['Measured_Value'],None)
InsGraph.add(e2)

a1 = Measuring(MT['Measuring'],None)
InsGraph.add(a1)

wGB1 = wasGeneratedBy(e2,a1,identifier='wGB1',time=None)
InsGraph.add(wGB1)

U1 = Used(a1,e1,identifier='U1',time=None)
InsGraph.add(U1)

wDF1 = wasDerivedFrom(e2,e1,activity=a1,generation=wGB1,usage=U1,identifier='wDF1')
InsGraph.add(wDF1)

wAW1 = wasAssociatedWith(a1,ag1,identifier='wAW1')
InsGraph.add(wAW1)

aOBO1 = actedOnBehalfOf(ag1,ag2,identifier='aOBO1')
InsGraph.add(aOBO1)

#testdict = e1._toRDF()
#for sub in testdict.keys():
#    print str(sub)
#   for pred in testdict[sub].keys():
#        print str(pred)
#       if isinstance(testdict[sub][pred],list):
#          for obj in testdict[sub][pred]:
#             print 'object: ' + str(obj) + '     '
#    else:        
#       print str(testdict[sub][pred])
            
print json.dumps(examplegraph.to_provJSON(),indent=4)