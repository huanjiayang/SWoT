'''
Created on 10 Sep 2012

@author: AYODELE-M.AKINGBULU
'''
import re
import requests
import json
import numpy
import matplotlib
import twitter
import wx
from webtry import *
import uuid
from pyprov.model.type import *
from pyprov.model.relation import *
from pyprov.model.bundle import *
from string import     *
from Home_Sense_Model import *


DC = Namespace('http://purl.org/dc/elements/1.1/')
FOAF = Namespace('http://xmlns.com/foaf/0.1/')
HS = PROVNamespace('hs','http://homesensor.com/#')
SN = Namespace('sn',"http://homesensor.com/schemas/sensor_network#")
SENSORS = Namespace('sensors',"http://www.homesensor.com/sensors#")
RDFS = Namespace('http://www.w3.org/2000/01/rdf-schema#')
prov = Namespace("http://www.w3.org/ns/prov-dm/")
TIME_1 = Namespace('sensors',"http://www.homesensor.com/TIME#")
rdf = PROVNamespace("rdf","http://www.w3.org/TR/rdf-schema/#")

sensor_graph=Mystore('mystore', 'mystore')


def addtoStore(msg,msg_timestamp):
    

# Create appropriate instances(based on Home Sensor Model) for sensor data
    mac_address = msg[0]
    type1 = msg[1]
    type1_value = msg[2]
    type2 = msg[3]
    type2_value = msg[4]
    type3 = msg[5]
    type3_value = msg[6]  
    
    S_Network = Sensor_Network(identifier=HS[str(uuid.uuid1())], attributes=None, account=None)
    sensor_graph.addPROVInstance(S_Network) 

    sn1 = Sensor_Node(identifier=HS[mac_address],attributes=None,sensor_id=None, sensor_name=None, account=None)
    sensor_graph.addPROVInstance(sn1)
    
    sn2 = Sensor_Node(identifier=HS[mac_address],attributes=None,sensor_id=None, sensor_name=None, account=None)
    sensor_graph.addPROVInstance(sn2)
    
    sn3 = Sensor_Node(identifier=HS[mac_address],attributes=None,sensor_id=None, sensor_name=None, account=None)
    sensor_graph.addPROVInstance(sn3)
    
    sensor1 = Sensor(identifier=HS[mac_address+'_1'],sensor_type=type1,attributes=None, account=None)
    sensor_graph.addPROVInstance(sensor1)
    
    sensor_reading1 = Sensor_Readings(identifier=HS[str(uuid.uuid1())], value=type1_value)
    sensor_graph.addPROVInstance(sensor_reading1)
    
    # do the same, except the sensor node, for type2 and type3
    sensor2 = Sensor(identifier=HS[mac_address+'_2'],sensor_type=type2,attributes=None, account=None)
    sensor_graph.addPROVInstance(sensor2)
    
    sensor_reading2 = Sensor_Readings(identifier=HS[str(uuid.uuid1())], value=type2_value,attributes=None, account=None)
    sensor_graph.addPROVInstance(sensor_reading2)
    
    sensor3 = Sensor(identifier=HS[mac_address+'_3'],sensor_type=type3,attributes=None, account=None)
    sensor_graph.addPROVInstance(sensor3)
    
    sensor_reading3 = Sensor_Readings(identifier=HS[str(uuid.uuid1())], value=type3_value, attributes=None, account=None)
    sensor_graph.addPROVInstance(sensor_reading3)
    
    Activity4 = Sensor_Reading_Activity(identifier=HS[str(uuid.uuid1())], attributes=None, account=None, Sensor_Reading=sensor_reading1)
    sensor_graph.addPROVInstance(Activity4)
    
    Activity5 = Sensor_Reading_Activity(identifier=HS[str(uuid.uuid1())], attributes=None, account=None, Sensor_Reading=sensor_reading2)
    sensor_graph.addPROVInstance(Activity5)
    
    Activity6 = Sensor_Reading_Activity(identifier=HS[str(uuid.uuid1())], attributes=None, account=None, Sensor_Reading=sensor_reading3)
    sensor_graph.addPROVInstance(Activity6)
    
    # entity instance in the Model
    Entity0 = Entity(identifier=HS[str(uuid.uuid1())], attributes=None, account=None)
    sensor_graph.addPROVInstance(Entity0)
    
    Entity1 = Entity(identifier=HS[str(uuid.uuid1())], attributes=None, account=None)
    sensor_graph.addPROVInstance(Entity1)
    
    Entity2 = Entity(identifier=HS[str(uuid.uuid1())], attributes=None, account=None)
    sensor_graph.addPROVInstance(Entity2)
    
    # do the relations here as well
    # relationship between 3 sensor nodes(agent) and sensor network(agent)
    aOB0 = actedOnBehalfOf(subordinate=sn1, responsible=S_Network, identifier=None, attributes=None, account=None)
    sensor_graph.addPROVInstance(aOB0)
    aOB1 = actedOnBehalfOf(subordinate=sn2, responsible=S_Network, identifier=None, attributes=None, account=None)
    sensor_graph.addPROVInstance(aOB1)
    aOB2 = actedOnBehalfOf(subordinate=sn3, responsible=S_Network, identifier=None, attributes=None, account=None)
    sensor_graph.addPROVInstance(aOB2)


    # relationship between 3 sensor nodes(agent) and their sensors(agent)
    aOB3 = actedOnBehalfOf(subordinate=sensor1, responsible=sn1, identifier=None, attributes=None, account=None)
    sensor_graph.addPROVInstance(aOB3)
    aOB4 = actedOnBehalfOf(subordinate=sensor2, responsible=sn2, identifier=None, attributes=None, account=None)
    sensor_graph.addPROVInstance(aOB4)
    aOB5 = actedOnBehalfOf(subordinate=sensor3, responsible=sn3, identifier=None, attributes=None, account=None)
    sensor_graph.addPROVInstance(aOB5)


    # relationship between sensors(agent) and sensor reading activity(activity)
    wAW0 = wasAssociatedWith(activity=Activity4, agent=sensor1, identifier=HS["wAW0"], attributes=tripledict, account=None)
    sensor_graph.addPROVInstance(wAW0)
    wAW1 = wasAssociatedWith(activity=Activity5, agent=sensor2, identifier=HS["wAW1"], attributes=tripledict, account=None)
    sensor_graph.addPROVInstance(wAW1)
    wAW2 = wasAssociatedWith(activity=Activity6, agent=sensor3, identifier=HS["wAW2"], attributes=tripledict, account=None)
    sensor_graph.addPROVInstance(wAW2)
    
    
    # relationship between entity and activity
    wGB0 = wasGeneratedBy(entity=Entity0,activity=Activity4,identifier=HS["wGB0"],time=None,attributes=tripledict)
    sensor_graph.addPROVInstance(wGB0)
    
    wGB1 = wasGeneratedBy(entity=Entity1,activity=Activity5,identifier=HS["wGB0"],time=None,attributes=tripledict)
    sensor_graph.addPROVInstance(wGB1)
    
    wGB2 = wasGeneratedBy(entity=Entity2,activity=Activity6,identifier=HS["wGB0"],time=None,attributes=tripledict)
    sensor_graph.addPROVInstance(wGB2)

    # relationship between sensors(agent) and entities(entity)
    wAT0 = wasAttributedTo(entity=Entity0, agent=sensor1, identifier=HS["wAT0"], attributes=None, account=None)
    sensor_graph.addPROVInstance(wAT0)
    
    wAT1 = wasAttributedTo(entity=Entity1, agent=sensor2, identifier=HS["wAT1"], attributes=None, account=None)
    sensor_graph.addPROVInstance(wAT1)
    
    wAT2 = wasAttributedTo(entity=Entity2, agent=sensor3, identifier=HS["wAT2"], attributes=None, account=None)
    sensor_graph.addPROVInstance(wAT2)
    
    return msg

data = ['Node: 8766 Temp: 23 Humidity: 65 Light: 4Node: 3fb7 Temp: 22 Humidity: 65 Light: 4']

msg = str(data)
msg = msg.replace("\n","").replace("\r","")
msglist = msg.split(",")

for msg in msglist:
    
        
    msg = msg.split(",")[0]
    

    # Print sensor data              
    print "data: ", msg
    msg_timestamp = datetime.datetime.now()
    addtoStore(msg, msg_timestamp)

class PROVBuilder:
    
    def __init__(self):
        self.container = PROVContainer()
        
        
    def traverseStore(self,RDFstore):
        print "let's see what we have in store:"
        print str(RDF.type)
        for s,p,o in RDFstore:
            print s,p,o
        for RDFtype_triple in RDFstore.triples((None, RDF.type, None)):
            sub = RDFtype_triple[0]
            print 'sub = '
            print sub
            rdftype = RDFtype_triple[2]
            print 'type'
            print rdftype
            attrdict = {}
            for attr in RDFstore.triples((sub, None, None)):
                if not attr[1] == RDF.type:
                    attrdict[attr[1]] = attr[2]
            #if entity matches add to container
            if rdftype == prov['Entity']:
                print 'Entity found'
                e = Entity(str(sub),attributes=attrdict)
                self.container.add(e)
            elif rdftype == prov['Activity']:
                print 'Activity found'
                for Activity_triple in RDFstore.triples((sub, prov['starttime'], None)):
                    starttime = Activity_triple[2]
                a = Activity(str(sub),attributes=attrdict,starttime=starttime)
                self.container.add(a)
            elif rdftype == prov['wasGeneratedBy']:
                print 'wGB found'
                for Activity_triple in RDFstore.triples((sub, prov['wasGeneratedBy'], None)):
                    wasGeneratedBy = Activity_triple[2]
                for relation_triple in RDFstore.triples((sub, prov['entity'], None)):
                        entity = relation_triple[2]
                for relation_triple in RDFstore.triples((sub, prov['activity'], None)):
                        activity = relation_triple[2]
                gb = wasGeneratedBy(entity, activity, identifier=str(sub))
                self.container.add(gb)
            elif rdftype == prov['wasStartedByActivity']:
                print 'wSBA found'
                for Activity_triple in RDFstore.triples((sub, prov['wasStartedByActivity'], None)):
                    wasStartedByActivity = Activity_triple[2]
                for relation_triple in RDFstore.triples((sub, prov['started'], None)):
                        started = relation_triple[2]
                for relation_triple in RDFstore.triples((sub, prov['starter'], None)):
                        starter = relation_triple[2]
                sba = wasStartedByActivity(started, starter, identifier=str(sub))
                self.container.add(sba)
            elif rdftype == prov['wasStartedBy']:
                print 'wSB found'
                for Activity_triple in RDFstore.triples((sub, prov['wasStartedBy'], None)):
                    wasStartedBy = Activity_triple[2]
                for relation_triple in RDFstore.triples((sub, prov['entity'], None)):
                        entity = relation_triple[2]
                for relation_triple in RDFstore.triples((sub, prov['activity'], None)):
                        activity = relation_triple[2]
                sb = wasStartedBy(activity,entity,identifier=str(sub))
                self.container.add(sb)
            elif rdftype == prov['wasAssociatedWith']:
                print 'wAW found'
                for relation_triple in RDFstore.triples((sub, prov['wasAssociatedWith'], None)):
                    wasAssociatedWith = relation_triple[2]
                    entity = None
                    agent = None
                    
                    for relation_triple in RDFstore.triples((sub, prov['entity'], None)):
                        entity = relation_triple[2]
                    for relation_triple in RDFstore.triples((sub, prov['agent'], None)):
                        agent = relation_triple[2]
                    for relation_triple in RDFstore.triples((sub, prov['activity'], None)):
                        activity = relation_triple[2]
                                     
                    if not entity == None:
                        print activity, "wasAssociatedWith", entity
                    else:
                        print activity, "wasAssociatedWith", agent
                    aw = wasAssociatedWith(activity, agent, identifier=str(sub))
                    self.container.add(aw)
                
            elif rdftype == prov['wasDerivedFrom']:
                print 'wDF found'
                for Relation_triple in RDFstore.triples((sub, prov['wasDerivedFrom'], None)):
                    wasDerivedFrom = Relation_triple[2]
                for Relation_triple in RDFstore.triples((sub, prov['generatedentity'], None)):
                    generatedentity = Relation_triple[2]
                for Relation_triple in RDFstore.triples((sub, prov['usedentity'], None)):
                    usedentity = Relation_triple[2]  
                df = wasDerivedFrom(generatedentity, usedentity, identifier=str(sub))
                self.container.add(df)  
            elif rdftype == prov['actedOnBehalfOf']:
                print 'aOBO'
                for Relation_triple in RDFstore.triples((sub,prov['responsible'], None)):
                    responsible = Relation_triple[2]
                for Relation_triple in RDFstore.triples((sub,prov['subordinate'], None)):
                    subordinate = Relation_triple[2]
                print subordinate, "actedOnBehalfOf", responsible
                ob = actedOnBehalfOf(subordinate,responsible,identifier=str(sub))
                self.container.add(ob)
                
                
            elif rdftype == prov['wasAttributedTo']:
                print 'wAT'
                #for Relation_triple in RDFstore.triples((sub,prov['wasAttributedTo'], None)):
                 #   wasAttributedTo = Relation_triple[2]
                #at = wasAttributedTo(str) 
                #self.container.add(at)
                 
                for Relation_triple in RDFstore.triples((sub, prov['entity'], None)):
                    entity = Relation_triple[2]
                for Relation_triple in RDFstore.triples((sub, prov['agent'], None)):
                    agent = Relation_triple[2]
                at = wasAttributedTo(entity, agent, identifier=str(sub))
                self.container.add(at)
           
                
class test:
    def GET(self):
        builder = PROVBuilder()
        builder.traverseStore(sensor_graph.store)
        print builder.container._elementlist
        return json.dumps(builder.container.to_provJSON())
    
    
    
if __name__ == "__main__":
#Links specific classes to URLs on the web server
    urls = (
    '/homesensorcom/', 'HS_Network', #Links URL homesensor.com to HS_Network Class
    '/homesensorcom/view/','view',
    '/test/','test'
        )
    app = web.application(urls, globals()) #Run the web server.
    app.run()
    

