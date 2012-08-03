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
        


        
class HS_Network:
    def GET(self):
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
    
    
    def VIEW(self):
        returndict = {'whats_in_the_store' : '',
                      'subject' : '',
                      'predicate' : '',
                      'object:)' : ''}
        returnlist = []
        for s in sensor_graph.store:
           
    
    
    
    
    
    
    
if __name__ == "__main__":
#Links specific classes to URLs on the web server
    urls = (
    '/homesensorcom/', 'HS_Network', #Links URL homesensor.com to HS_Network Class
    '/sensor','sensor'
        )
    app = web.application(urls, globals()) #Run the web server.
    app.run()