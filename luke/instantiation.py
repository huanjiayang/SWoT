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

InsGraph = PROVContainer()
InsGraph.set_default_namespace("http://www.mytype.com/#")

def msg_process(data):
    data = str(data)
    data = data.replace("\n","").replace("\r","")
    datastr = data.split(",")
    
    ag1 = Sensor_Node(identifier=MT[datastr[3]],Sink_Node=MT[datastr[1]])
    InsGraph.add(ag1)
    
    ag2 = Sink_Node(identifier=MT[datastr[1]])
    InsGraph.add(ag2)
    
    e1 = Sensor(identifier=MT[datastr[7]],Sensor_Node=MT[datastr[3]])
    InsGraph.add(e1)
    
    e2 = Measured_Value(value=MT[datastr[5]],value_type=MT[datastr[4]],Sensor_id=MT[datastr[7]])
    InsGraph.add(e2)
    
    a1 = Measuring(Sensor_id=MT[datastr[7]],starttime=MT[datastr[17]],endtime=MT[datastr[19]])
    InsGraph.add(a1)
    
    e3 = Sensor(identifier=MT[datastr[11]],Sensor_Node=MT[datastr[3]])
    InsGraph.add(e3)
    
    e4 = Measured_Value(value=MT[datastr[9]],value_type=MT[datastr[8]],Sensor_id=MT[datastr[11]])
    InsGraph.add(e4)
    
    a2 = Measuring(Sensor_id=MT[datastr[11]],starttime=MT[datastr[17]],endtime=MT[datastr[19]])
    InsGraph.add(a2)
    
    e5 = Sensor(identifier=MT[datastr[15]],Sensor_Node=MT[datastr[3]])
    InsGraph.add(e5)
    
    e6 = Measured_Value(value=MT[datastr[13]],value_type=MT[datastr[12]],Sensor_id=MT[datastr[15]])
    InsGraph.add(e6)
    
    a3 = Measuring(Sensor_id=MT[datastr[15]],starttime=MT[datastr[17]],endtime=MT[datastr[19]])
    InsGraph.add(a3)
    
    wGB1 = wasGeneratedBy(e2,a1,identifier='wGB1-urn:uuid:' + str(uuid.uuid1()),attributes=None)
    InsGraph.add(wGB1)
    
    wGB2 = wasGeneratedBy(e4,a2,identifier='wGB2-urn:uuid:' + str(uuid.uuid1()),attributes=None)
    InsGraph.add(wGB2)
    
    wGB3 = wasGeneratedBy(e6,a3,identifier='wGB3-urn:uuid:' + str(uuid.uuid1()),attributes=None)
    InsGraph.add(wGB3)
    
    U1 = Used(a1,e1,identifier='U1-urn:uuid:' + str(uuid.uuid1()),attributes=None)
    InsGraph.add(U1)
    
    U2 = Used(a2,e3,identifier='U2-urn:uuid:' + str(uuid.uuid1()),attributes=None)
    InsGraph.add(U2)
    
    U3 = Used(a3,e5,identifier='U3-urn:uuid:' + str(uuid.uuid1()),attributes=None)
    InsGraph.add(U3)
    
    wDF1 = wasDerivedFrom(e2,e1,activity=a1,generation=wGB1,usage=U1,identifier='wDF1-urn:uuid:' + str(uuid.uuid1()),attributes=None)
    InsGraph.add(wDF1)
    
    wDF2 = wasDerivedFrom(e4,e3,activity=a2,generation=wGB2,usage=U2,identifier='wDF2-urn:uuid:' + str(uuid.uuid1()),attributes=None)
    InsGraph.add(wDF2)
    
    wDF3 = wasDerivedFrom(e6,e5,activity=a3,generation=wGB3,usage=U3,identifier='wDF3-urn:uuid:' + str(uuid.uuid1()),attributes=None)
    InsGraph.add(wDF3)
    
    wAW1 = wasAssociatedWith(a1,ag1,identifier='wAW1-urn:uuid:' + str(uuid.uuid1()),attributes=None)
    InsGraph.add(wAW1)
    
    wAW2 = wasAssociatedWith(a2,ag1,identifier='wAW2-urn:uuid:' + str(uuid.uuid1()),attributes=None)
    InsGraph.add(wAW2)
    
    wAW3 = wasAssociatedWith(a3,ag1,identifier='wAW3-urn:uuid:' + str(uuid.uuid1()),attributes=None)
    InsGraph.add(wAW3)
    
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


msg01 = ['Sink_Node,8766,Sensor_Node,8766S02,Temp,23,Sensor_id,8766S0201,Humidity,65,Sensor_id,8766S0202,Light,4,Sensor_id,8766S0203,starttime,16:13,endtime,16:15,']
#            0      1     2            3     4    5     6        7         8     9     10         11      12  13   14       15        16        17   18       19
msg02 = ['Sink_Node,4569,Sensor_Node,4569S02,Temp,26,Sensor_id,4569S0201,Humidity,64,Sensor_id,4569S0202,Light,5,Sensor_id,4569S0203,starttime,16:14,endtime,16:19,']
#            0      1     2            3     4    5     6        7         8     9     10         11      12  13   14       15        16        17   18       19

msg_process(msg01)
msg_process(msg02)