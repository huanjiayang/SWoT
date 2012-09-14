'''
Created on 31 Jul 2012

@author: AYODELE-M.AKINGBULU
'''

import web 
import re
import uuid
from rdflib.store import Store, NO_STORE, VALID_STORE
from rdflib import Namespace, BNode, Literal, RDF, URIRef
from urllib import quote_plus
import rdflib as db
from rdflib import plugin
from Prov_Store import *
from math import log 
from time import time 
import json 
import SocketServer 
import os 
import commands 
import logging
import sys


DC = Namespace('http://purl.org/dc/elements/1.1/')
FOAF = Namespace('http://xmlns.com/foaf/0.1/')
HS = PROVNamespace('hs','http://homesensor.com/#')
SN = Namespace('sn',"http://homesensor.com/schemas/sensor_network#")
SENSORS = Namespace('sensors',"http://www.homesensor.com/sensors#")
RDFS = Namespace('http://www.w3.org/2000/01/rdf-schema#')
prov = Namespace("http://www.w3.org/ns/prov-dm/")
TIME_1 = Namespace('sensors',"http://www.homesensor.com/TIME#")
rdf = PROVNamespace("rdf","http://www.w3.org/2000/01/rdf-schema#")





tripledict = {}
starttime = datetime.datetime(2012, 7, 6, 5, 4, 3)
starter = "starter"


started =   "started"


ag0 = Agent(sensornetworkURI)

ag1 = Agent(sensornode1URI)
ag2 = Agent(sensornode2URI) 
ag3 = Agent(sensornode3URI)

agT1 = Agent(sensortemp1URI)
agT2 = Agent(sensortemp2URI)
agT3 = Agent(sensortemp3URI)
agL1 = Agent(sensorlight1URI)
agL2 = Agent(sensorlight2URI)
agL3 = Agent(sensorlight3URI)
agH1 = Agent(sensorhum1URI)
agH2 = Agent(sensorhum2URI)
agH3 = Agent(sensorhum3URI)


a0 = Activity(HS["a0"],starttime=starttime,attributes=tripledict)

a1 = Activity(HS["a1"],starttime=starttime,attributes=tripledict)

a2 = Activity(HS["a2"],starttime=starttime,attributes=tripledict)


e0 = Entity(identifier=HS["e0"]) #40 degree

e1 = Entity(identifier=HS["e1"])

e2 = Entity(identifier=HS["e2"])

e3 = Entity(identifier=HS["e3"])

e4 = Entity(identifier=HS["e4"])

e5 = Entity(identifier=HS["e5"])

e6 = Entity(identifier=HS["e6"])



aOB0 = actedOnBehalfOf(ag1, ag0, identifier=HS["aOB0"], attributes=None)
aOB1 = actedOnBehalfOf(ag2, ag0, identifier=HS["aOB1"], attributes=None)
aOB2 = actedOnBehalfOf(ag3, ag0, identifier=HS["aOB2"], attributes=None)
aOB3 = actedOnBehalfOf(agT1, ag1, identifier=HS["aOB3"], attributes=None)
aOB4 = actedOnBehalfOf(agL1, ag1, identifier=HS["aOB4"], attributes=None)
aOB5 = actedOnBehalfOf(agH1, ag1, identifier=HS["aOB5"], attributes=None)
aOB6 = actedOnBehalfOf(agT2, ag2, identifier=HS["aOB6"], attributes=None)
aOB7 = actedOnBehalfOf(agL2, ag2, identifier=HS["aOB7"], attributes=None)
aOB8 = actedOnBehalfOf(agH2, ag2, identifier=HS["aOB8"], attributes=None)
aOB9 = actedOnBehalfOf(agT3, ag3, identifier=HS["aOB9"], attributes=None)
aOB10 = actedOnBehalfOf(agL3, ag3, identifier=HS["aOB10"], attributes=None)
aOB11 = actedOnBehalfOf(agH3, ag3, identifier=HS["aOB11"], attributes=None)

wAW0 = wasAssociatedWith(a0, ag1, identifier=HS["wAW0"], attributes=tripledict)
wAW1 = wasAssociatedWith(a1, ag2, identifier=HS["wAW1"], attributes=tripledict)
wAW2 = wasAssociatedWith(a2, ag3, identifier=HS["wAW2"], attributes=tripledict)

wAT0 = wasAttributedTo(e0, agT1, identifier=HS["wAT0"], attributes=None)
wAT1 = wasAttributedTo(e1, agT2, identifier=HS["wAT1"], attributes=None)
wAT2 = wasAttributedTo(e2, agT3, identifier=HS["wAT2"], attributes=None)
wAT3 = wasAttributedTo(e3, agL1, identifier=HS["wAT3"], attributes=None)
wAT4 = wasAttributedTo(e4, agL2, identifier=HS["wAT4"], attributes=None)
wAT5 = wasAttributedTo(e5, agL3, identifier=HS["wAT5"], attributes=None)
wAT6 = wasAttributedTo(e6, agH1, identifier=HS["wAT6"], attributes=None)
wAT7 = wasAttributedTo(e0, agH2, identifier=HS["wAT7"], attributes=None)
wAT8 = wasAttributedTo(e1, agH3, identifier=HS["wAT8"], attributes=None)


u0 = Used(a0,e1,identifier=HS["u0"],time=None,attributes=tripledict)

wGB0 = wasGeneratedBy(e0,a0,identifier=HS["wGB0"],time=None,attributes=tripledict)
wDF0 = wasDerivedFrom(e0, e1, identifier=HS["wDF0"],activity=a0,generation=wGB0,usage=u0,attributes=None)
wSBA0 = wasStartedByActivity(a1,a0,identifier=HS["wSB0"],attributes=None)
wSB0 = wasStartedBy(e0,a0,identifier=HS["b0"],attributes=None)







#import pprint
#pp = pprint.PrettyPrinter(indent=4)
#pp.pprint(webgraph.to_provJSON())


sensor_graph=Mystore('mystore', 'mystore')


sensor_graph.addPROVInstance(ag0)
sensor_graph.addPROVInstance(ag1)
sensor_graph.addPROVInstance(ag2)
sensor_graph.addPROVInstance(ag3)
sensor_graph.addPROVInstance(agT1)
sensor_graph.addPROVInstance(agT2)
sensor_graph.addPROVInstance(agT3)
sensor_graph.addPROVInstance(agL1)
sensor_graph.addPROVInstance(agL2)
sensor_graph.addPROVInstance(agL3)
sensor_graph.addPROVInstance(agH1)
sensor_graph.addPROVInstance(agH2)
sensor_graph.addPROVInstance(agH3)
sensor_graph.addPROVInstance(a0)
sensor_graph.addPROVInstance(a1)
sensor_graph.addPROVInstance(a2)
sensor_graph.addPROVInstance(e0)
sensor_graph.addPROVInstance(e1)
sensor_graph.addPROVInstance(e2)
sensor_graph.addPROVInstance(e3)
sensor_graph.addPROVInstance(e4)
sensor_graph.addPROVInstance(e5)
sensor_graph.addPROVInstance(e6)
sensor_graph.addPROVInstance(aOB0)
sensor_graph.addPROVInstance(aOB1)
sensor_graph.addPROVInstance(aOB2)
sensor_graph.addPROVInstance(aOB3)
sensor_graph.addPROVInstance(aOB4)
sensor_graph.addPROVInstance(aOB5)
sensor_graph.addPROVInstance(aOB6)
sensor_graph.addPROVInstance(aOB7)
sensor_graph.addPROVInstance(aOB8)
sensor_graph.addPROVInstance(aOB9)
sensor_graph.addPROVInstance(aOB10)
sensor_graph.addPROVInstance(aOB11)
sensor_graph.addPROVInstance(wAW0)
sensor_graph.addPROVInstance(wAW1)
sensor_graph.addPROVInstance(wAW2)
sensor_graph.addPROVInstance(wAT0)
sensor_graph.addPROVInstance(wAT1)
sensor_graph.addPROVInstance(wAT2)
sensor_graph.addPROVInstance(wAT3)
sensor_graph.addPROVInstance(wAT4)
sensor_graph.addPROVInstance(wAT5)
sensor_graph.addPROVInstance(wAT6)
sensor_graph.addPROVInstance(wAT7)
sensor_graph.addPROVInstance(wAT8)

#
#snURI = 'uri:uuid:sensnetworkayodele001'
#sn_node1URI = 'uri:uuid:sensornode01'
#SENSOR_NETWORK=SN[snURI]
#sensor_node = sn_node1URI


#sensor_graph.store.add((rdflib.URIRef('http://www.homesensor.com/'),RDF.type, rdflib.Literal('Google home page')))
#sensor_graph.store.add((SENSOR_NETWORK,RDF.type,SENSORS['sensor_node']))
#sensor_graph.store.add((rdflib.URIRef('http://www.homesensor.com/'),RDF.type, rdflib.Literal('Google home page')))
#sensor_graph.store.add((SENSOR_NETWORK,DC['title'],Literal(sensor_node)))
#sensor_graph.store.add((sensornetworkURI, prov['wasGeneratedBy'], Literal('Ayo')))

    
    



#labels = file('sensor_labels.txt', 'r').readlines() #Load graph labels from a file
render = web.template.render('templates/')


#def get_type(self,sensorid,node_id):
#        for line in labels:
#            node_id, label, fieldtype = line.split(',', 2)
#            if int(id) == int(sensorid):
#                return fieldtype.rstrip('\r\n')
            
#Get Label reads in sensor names, determines the type name of the sensor
#to generate graphs on page properly
#def get_label(sensorid, node_id, fieldtype):
#    for line in labels:
#        node_id, label, fieldtype = line.split(',', 2)
#        print 'line: '
#        print str(line)
#        if int(id) == int(sensorid):
#            return labels
        
#def VIEW(self,id):
#        fq = sensor_graph.store.query('select ?pred ?obj where {<%s> ?pred ?obj .}' % id)
#        sq = sensor_graph.store.query('select ?pred ?sub where {?sub ?pred <%s> .}' % id)
#        name = id
#        
#        returndict = {'whats_in_the_store' : '',
#                      'subject' : '',
#                      'predicate' : '',
#                      'object:)' : ''}
#        returnlist = []
#        for s in fq:
#            print s
#            if s['predicate']['value'] in returnlist:
#                name = s['object']['value']
#            t = web.template('graph1.html')
#            return t.render(name=name, id=id, fq=fq, sq=sq, qp=quote_plus)
#            returnlist.append(returndict)
#            web.header('Content-Type', 'application/json')
#            return json.dumps(returndict)
#        
#def createEntity(self,entityType1,entityType2,entityType,identifier='e0'):
#    entity = entityType(entityType1,entityType2)
#    e0= entityType1
#    e1 = entityType2
#    return entity
#        
        
        
#def func(self,provcontainer,(sub,pred,obj)):
#    container = provcontainer()
#    for sub,pred,obj in sensor_graph.store:
#        if pred == RDF.type:
#            yield pred
#            container.add(sub=None,pred=RDF.type,obj=None)
#        else:
#            if pred != RDF.type:
#                yield (None,None,None)
#        if sub == URIRef:
#            yield sub
#        else:
#            if sub != URIRef:
#                yield(None,pred,None)
#            elif sub == URIRef('prov:Entity'):
#                prov[Entity:e0]
#                container.add(e0)
#                prov[Entity:e1.createEntity]
#                container.add(e1)
#                prov[Activity:a0]
#                container.add(a0)
#                prov[Agent:ag0]
#                container.add(a0)
#                prov[wasGeneratedBy:f0]
#                container.add(f0)
#                prov[wasStartedBy:b0]
#                container.add(b0)
#                prov[wasStartedByActivity:s0]
#                container.add(s0)
#                prov[wasAssociatedWith:w0]
#                container.add(w0)
#                prov[Used:u0]
#                container.add(u0)
#                container.add(sub=URIRef('prov:Entity'),pred=None,obj=None)
#            elif obj == Literal(''):
#                yield(sub,pred,obj)
#        #return container
#        #return json.dumps(container)
#        
        
        
class HS_Network:
    def GET(self):
#        args = web.input()
#        start = int(args.get('start', time() - 86400))
        #cur = store.cursor() 
        #query triples in store
        #cur.execute('select ?pred ?obj where {<%s> ?pred ?obj .}' % id)
        returndict = {'whats_in_the_store' : '',
                      'subject' : '',
                      'predicate' : '',
                      'object:)' : ''}
        returnlist = []
        for s,p,o in sensor_graph.store:
            returndict['whats_in_the_store'] = returndict['whats_in_the_store'] + str(s) + str(p) + str(o)
            returndict['subject'] = str(s)
            returndict['predicate'] = str(p)
            returndict['object:)'] = str(o)
        
        returnlist.append(returndict)
            
        web.header('Content-Type', 'application/json')
        return json.dumps(returndict)
    
    

           
                
class test:
    def GET(self):
        builder = PROVBuilder()
        builder.traverseStore(sensor_graph.store)
        print builder.container._elementlist
        return json.dumps(builder.container.to_provJSON())
    

#class Sensor(Entity):
#    
#    def __init__(self,identifier,attributes=None,account=None):
#        Entity.__init__(self,identifier=identifier, attributes=attributes, account=account)
#        
#    def _toRDF(self):
#        Entity._toRDF(self)
#        self.rdftriples[self.identifier][rdf['type']] = HS['Sensor']
#        return self.rdftriples

    
if __name__ == "__main__":
#Links specific classes to URLs on the web server
    urls = (
    '/homesensorcom/', 'HS_Network', #Links URL homesensor.com to HS_Network Class
    '/homesensorcom/view/','view',
    '/test/','test'
        )
    app = web.application(urls, globals()) #Run the web server.
    app.run()
    


