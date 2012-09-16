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


sensornetworkURI = 'uri:uuid:sensnetworkayodele001'
sensornode1URI = 'uri:uuid:sensornode01'
sensornode2URI = 'uri:uuid:sensornode02'
sensornode3URI = 'uri:uuid:sensornode03'
sensortemp1URI = 'uri:uuid:sensorTEMP01'
sensortemp2URI = 'uri:uuid:sensorTEMP02'
sensortemp3URI = 'uri:uuid:sensorTEMP03'
sensorlight1URI = 'uri:uuid:sensorLIGHT01'
sensorlight2URI = 'uri:uuid:sensorLIGHT02'
sensorlight3URI = 'uri:uuid:sensorLIGHT03'
sensorhum1URI = 'uri:uuid:sensorHUM01'
sensorhum2URI = 'uri:uuid:sensorHUM02'
sensorhum3URI = 'uri:uuid:sensorHUM03'



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



    
    



#labels = file('sensor_labels.txt', 'r').readlines() #Load graph labels from a file
render = web.template.render('templates/')



        
        

        
        
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
            returndict['whats_in_the_store'] = returndict['whats_in_the_store'] #+ str(s) + str(p) + str(o)
            returndict['subject'] = str(s)
            returndict['predicate'] = str(p)
            returndict['object:)'] = str(o)
        
        tttdict = {   'account': {   'acc0': {   'accountattr': 'accattrvalue',
                               'asserter': 'asserter_name',
                               'entity': {   'e2': {   }, 'en': {   }}}},
    'activity': {   'activity': {   'prov:starttime': [   '2012-09-14 08:23:51',
                                                    'xsd:dateTime'],
                              'recipeLink': 'sensor_readings'},
                    'a1': {   }},
    'agent': {   'ag': {   'prov:identifier': [   'mac_address',
                                                    'xsd:sensor_id'],
                              'recipeLink': 'sensor_readings'},
                    'a1': {   }},
    'entity': {   'e0': {   'creator': ['foaf:Alice', 'xsd:QName'],
                            'path': '/shared/crime.txt',
                            'type': 'File'},
                  
    'prefix': {   'dcterms': 'http://purl.org/dc/terms/',
                  'default': 'http://www.example.com/',
                  'foaf': 'http://xmlns.com/foaf/0.1/',
                  'ns0': 'http://www.test.org/',
                  'ns1': 'http://www.example2222.com/',
                  'prov': 'http://www.w3.org/ns/prov-dm/',
                  'test': 'http://www.example.org/',
                  'xsd': 'http://www.w3.org/2001/XMLSchema-datatypes#'},
     'wasAttributedTo': {   '_:RLAT1': {   'prov:entity': 'sensor_reading1',
                                                'prov:agent': 'sensor1'}},
     'wasAssociatedWith': {   '_:RLAT1': {   'prov:activity': 'Activity4',
                                                'prov:agent': 'sensor1'}},
     'actedOnBehalfOf': {   '_:RLAT1': {   'prov:surbordinate': 'sensor1',
                                                'prov:responsible': 'sensor_node1'}},
   
    'wasDerivedFrom': {   '_:RLAT0': {   'prov:activity': 'a0',
                                         'prov:generatedentity': 'e0',
                                         'prov:generation': 'wGB0',}},
    
    'wasGeneratedBy': {   'wGB0': {   'fct': 'create',
                                    'prov:activity': 'Activity4',
                                    'prov:entity': 'sensor1'}},
    'wasStartedByActivity': {   '_:RLAT1': {   'prov:started': 'Network_Organization',
                                               'prov:starter': 'Discovery'}}},
    
    
      
        
        returnlist.append(returndict)
            
        web.header('Content-Type', 'application/json')
        return json.dumps(tttdict)
    
    

           
                
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
    


