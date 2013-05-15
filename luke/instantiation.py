from model import *
import json
import datetime
from pyprov.model.type import *
from pyprov.model.relation import *
from pyprov.model.bundle import *
import uuid

DC = PROVNamespace('dc', 'http://purl.org/dc/elements/1.1/')
PROV = PROVNamespace('prov',"http://www.w3.org/ns/prov-dm/")
RDF = PROVNamespace('rdf', "http://www.w3.org/2000/01/rdf-schema#")
MT = PROVNamespace('mt', "http://www.mytype.com/#")

data = ['Sink_Node,8766,Sensor_Node,8766S02,Sensor_id,346242,Results,Temp 23 Humidity 65 Light 4, satrttime, 16:13, endtime, 16:15,']

data = str(data)
data = data.replace("\n","").replace("\r","")
datastr = data.split(",")

InsGraph = PROVContainer()
InsGraph.set_default_namespace("http://www.mytype.com/#")

ag1 = Sensor_Node(identifier=MT[datastr[3]],Sink_Node=MT[datastr[1]])
InsGraph.add(ag1)

ag2 = Sink_Node(identifier=MT[datastr[1]])
InsGraph.add(ag2)

e1 = Sensor(identifier=MT[datastr[5]],Sensor_Type=MT[datastr[2]])
InsGraph.add(e1)

e2 = Measured_Value(value=MT[datastr[7]],Sensor_id=MT[datastr[5]])
InsGraph.add(e2)

a1 = Measuring(Sensor_id=MT[datastr[5]],starttime=MT[datastr[9]],endtime=MT[datastr[11]])
InsGraph.add(a1)

wGB1 = wasGeneratedBy(e2,a1,identifier='wGB1-urn:uuid:' + str(uuid.uuid1()),attributes=None)
InsGraph.add(wGB1)

U1 = Used(a1,e1,identifier='U1-urn:uuid:' + str(uuid.uuid1()),attributes=None)
InsGraph.add(U1)

wDF1 = wasDerivedFrom(e2,e1,activity=a1,generation=wGB1,usage=U1,identifier='wDF1-urn:uuid:' + str(uuid.uuid1()),attributes=None)
InsGraph.add(wDF1)

wAW1 = wasAssociatedWith(a1,ag1,identifier='wAW1-urn:uuid:' + str(uuid.uuid1()),attributes=None)
InsGraph.add(wAW1)

aOBO1 = actedOnBehalfOf(ag1,ag2,identifier='aOBO1-urn:uuid:' + str(uuid.uuid1()),attributes=None)
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
            
print json.dumps(InsGraph.to_provJSON(),indent=4)