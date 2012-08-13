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



webgraph = PROVContainer()


tripledict = {}
starttime = datetime.datetime(2012, 7, 6, 5, 4, 3)
a1 = "starter"
webgraph.add(a1)

a2 =   "started"
webgraph.add(a2)

ag0 = Agent(sensornetworkURI)
webgraph.add(ag0)

a0 = Activity(HS["a0"],starttime=starttime,attributes=tripledict)
webgraph.add(a0)

a1 = Activity(HS["a1"],starttime=starttime,attributes=tripledict)
webgraph.add(a1)

e0 = Entity(identifier=HS["e0"])
webgraph.add(e0)

e1 = Entity(FOAF['Foo'],attributes=tripledict)
webgraph.add(e1)

g0 = wasGeneratedBy(e0,a0,identifier=HS["g0"],time=None,attributes=tripledict)
webgraph.add(g0)

u0 = Used(a0,e1,identifier=HS["u0"],time=None,attributes=tripledict)
webgraph.add(u0)

d0 = wasDerivedFrom(e0, e1, identifier=HS["wDF0"],activity=a0,generation=g0,usage=u0,attributes=None)
webgraph.add(d0)

f0 = wasGeneratedBy(e0,g0,identifier=HS["wGB0"],time=None,attributes=None)
webgraph.add(f0)

w0 = wasAssociatedWith(ag0,a0,identifier=HS["w0"],attributes=tripledict)
webgraph.add(w0)

s0 = wasStartedByActivity(a1,a0,identifier=HS["wSB0"],attributes=None)
webgraph.add(s0)

t0 = wasAttributedTo(a1,a0,identifier=HS["wAT"],attributes=None)

b0 = wasStartedBy(e0,a0,identifier=HS["b0"],attributes=None)
webgraph.add(b0)



import pprint
pp = pprint.PrettyPrinter(indent=4)
pp.pprint(webgraph.to_provJSON())


sensor_graph=Mystore('mystore', 'mystore')



sensor_graph.addPROVInstance(e0)
sensor_graph.addPROVInstance(w0)
sensor_graph.addPROVInstance(a0)
sensor_graph.addPROVInstance(a1)
sensor_graph.addPROVInstance(f0)
sensor_graph.addPROVInstance(ag0)
sensor_graph.addPROVInstance(s0)
sensor_graph.addPROVInstance(b0)
sensor_graph.addPROVInstance(d0)
sensor_graph.addPROVInstance(e1)
sensor_graph.addPROVInstance(g0)

snURI = 'uri:uuid:sensnetworkayodele001'
sn_node1URI = 'uri:uuid:sensornode01'
SENSOR_NETWORK=SN[snURI]
sensor_node = sn_node1URI


sensor_graph.store.add((rdflib.URIRef('http://www.homesensor.com/'),RDF.type, rdflib.Literal('Google home page')))
sensor_graph.store.add((SENSOR_NETWORK,RDF.type,SENSORS['sensor_node']))
sensor_graph.store.add((rdflib.URIRef('http://www.homesensor.com/'),RDF.type, rdflib.Literal('Google home page')))
sensor_graph.store.add((SENSOR_NETWORK,DC['title'],Literal(sensor_node)))
sensor_graph.store.add((sensornetworkURI, prov['wasGeneratedBy'], Literal('Ayo')))
    
    



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
        
def VIEW(self,id):
        fq = sensor_graph.store.query('select ?pred ?obj where {<%s> ?pred ?obj .}' % id)
        sq = sensor_graph.store.query('select ?pred ?sub where {?sub ?pred <%s> .}' % id)
        name = id
        
        returndict = {'whats_in_the_store' : '',
                      'subject' : '',
                      'predicate' : '',
                      'object:)' : ''}
        returnlist = []
        for s in fq:
            print s
            if s['predicate']['value'] in returnlist:
                name = s['object']['value']
            t = web.template('graph1.html')
            return t.render(name=name, id=id, fq=fq, sq=sq, qp=quote_plus)
            returnlist.append(returndict)
            web.header('Content-Type', 'application/json')
            return json.dumps(returndict)
        
def createEntity(self,entityType1,entityType2,entityType,identifier='e0'):
    entity = entityType(entityType1,entityType2)
    e0= entityType1
    e1 = entityType2
    return entity
        
        
        
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
            type = RDFtype_triple[2]
            print 'type'
            print type
            attrdict = {}
            for attr in RDFstore.triples((sub, None, None)):
                if not attr[1] == RDF.type:
                    attrdict[attr[1]] = attr[2]
            #if entity matches add to container
            if type == prov['Entity']:
                print 'Entity found'
                e = Entity(str(sub),attributes=attrdict)
                self.container.add(e)
            elif type == prov['Activity']:
                print 'Activity found'
                for Activity_triple in RDFstore.triples((sub, prov['starttime'], None)):
                    starttime = Activity_triple[2]
                a = Activity(str(sub),attributes=attrdict,starttime=starttime)
                self.container.add(a)
            elif type == prov['Activity']:
                print 'Activity found'
                for Activity_triple in RDFstore.triples((sub, prov['wasGeneratedBy'], None)):
                    wasGeneratedBy = Activity_triple[2]
                g = wasGeneratedBy(str(sub))
                self.container.add(g)
            elif type == prov['wSBA']:
                print 'wSBA found'
                for Activity_triple in RDFstore.triples((sub, prov['wasStartedByActivity'], None)):
                    wasStartedByActivity = Activity_triple[2]
                sba = wasStartedByActivity(str(sub))
                self.container.add(sba)
            elif type == prov['wSB']:
                print 'wSB found'
                for Activity_triple in RDFstore.triples((sub, prov['wasStartedBy'], None)):
                    wasStartedBy = Activity_triple[2]
                sb = wasStartedBy(str(sub))
                self.container.add(sb)
            elif type == prov['wAW']:
                print 'wAW found'
                for Relation_triple in RDFstore.triples((sub, prov['wasAssociatedWith'], None)):
                    wasAssociatedWith = Relation_triple[2]
                aw = wasAssociatedWith(str(sub))
                self.container.add(aw)
            elif type == prov['wDF']:
                print 'wDF found'
                for Relation_triple in RDFstore.triples((sub, prov['wasDerivedFrom'], None)):
                    wasDerivedFrom = Relation_triple[2]
                df = wasDerivedFrom(str(sub))
                self.container.add(df)  
            elif type == prov['aOBO']:
                print 'aOBO'
                for Relation_triple in RDFstore.triples((sub,prov['actedOnBehalfOf'], None)):
                    actedOnBehalfOf = Relation_triple[2]
                ob = actedOnBehalfOf(str(sub))
                self.container.add(ob)
            elif type == prov['wAT']:
                print 'wAT'
                for Relation_triple in RDFstore.triples((sub,prov['wasAttributedTo'], None)):
                    wasAttributedTo = Relation_triple[2]
                at = wasAttributedTo(str) 
                self.container.add(at)
            elif type == URIRef['wAT']:
                print 'wAT'
                for Relation_triple in RDFstore.triples((sub,prov['wasAttributedTo'], None)):
                    wasAttributedTo = Relation_triple[0]
                at = wasAttributedTo(str(sub))
                self.container.add(at)
            else:
                for Relation_triple in RDFstore.triples((URIRef['wAT'],RDF.type,prov['wasAttributedTo'])):
                    wasAttributedTo = Relation_triple[1]
                at = wasAttributedTo(str)
                self.container.add(at)
class test:
    def GET(self):
        builder = PROVBuilder()
        builder.traverseStore(sensor_graph.store)
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
    


