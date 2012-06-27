import rdflib
import rdfextras
from rdflib.graph import Graph, ConjunctiveGraph
from rdflib.namespace import Namespace
from rdflib.term import Literal
from rdflib.term import URIRef
from rdflib import plugin
from rdflib.store import Store, NO_STORE, VALID_STORE
from datetime import date, time, datetime

import datetime
from pyprov.model.type import *
from pyprov.model.relation import *
from pyprov.model.bundle import *



# Define Namespace
hs = Namespace("http://homesensor.com#")
prov = Namespace("http://www.w3.org/ns/prov-dm/")

S_Network = hs['SensorNetwork']

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
sensorHum1URI = 'uri:uuid:sensorHUM01'
sensorHum1URI = 'uri:uuid:sensorHUM02'
sensorHum1URI = 'uri:uuid:sensorHUM03'


createtime = 'datetime...'

#
S_Network = URIRef("http://homesensor.com/")
Readings = hs['Reading']

Communication = hs("Communication")
Discovery = hs("discovery")
S_Node = hs("S_Node")
Sensor = hs("Sensor")

Communication = URIRef("http://homesensor.com/Activity/")
Discovery = URIRef("http://homesensor.com/Activity/")
S_Node = URIRef("http://homesensor.com/S_node/")
Sensor = URIRef("http://homsensor.com/Sensor/")


graph=Graph(store='Sleepycat',identifier='test')
graph.open("provfolder", create=False)



class WSNPROV:
    
    def __init__(self):
        self.provgraph = PROVContainer()
        
    def addNetwork(self,sensornetworkURI,createtime,person):
#        graph.add((sensornetworkURI, RDF.type, S_Network))
#        graph.add((sensornetworkURI, dcterms['create'], createtime))
#        graph.add((sensornetworkURI, prov['wasGeneratedBy'], person))
        
        a0 = Agent(sensornetworkURI,attribute={})
        e0 = Entity()
        g0 = Activity()
        d0=wasDerivedFrom(e0, activity=a0,generation=g0,attributes=None)
        f0=wasGeneratedBy(e0,g0,time=None,attributes=None)
        
        self.provgraph.add(e0)
    
    def convertDataToPROV(self,rawdata):
        pass
    
    
    def createWGB(self,Agent=None,Activity,Entity,identifier=None,time=None,attributes=None):
        """Define WGB relation with self as context"""
        wGB = wasGeneratedBy(Activity,Entity,identifier,time,attributes)
        
        attrdict = {}
        return wGB
   
    def createWAW(self,identifier=None,Agent,Activity,attributes):
        """Define WAW relation with self as context"""
        wAW = wasAssociatedWith(Agent,Activity,Entity,identifier)
        return wAW
    
    def createWSBA(self,identifier=None,started,starter,attributes):
        """Define WSBA relation with self as context"""
        wSBA = wasStartedByActivity(Activity,identifier,started,starter)
        a1 = starter
        a2 = started
        return wSBA
    
    def createWSB(self,identifier=None,Activity=None,Entity=None,time=None,attributes=None):
        """Define WSB relation with self as context"""
        wSB = wasStartedBy(Agent,Activity,Entity,identifier,time,attributes)
        return wSB
    
    def createdAOBO(self,Agent2,Agent1,Activity,identifier,attributes):
        """Define AOBO relation with self as context"""
        aOBO = actedOnBehalfOf(Agent2,Agent1,Activity,identifier,attributes) 
        ag1 = Agent1
        ag2 = Agent2
        return aOBO
    
    def createdWAT(self,Entity,Agent,identifier=None,attributes):
        """Define WAT relation with self as context"""
        wAT = wasAttributedTo(Agent,Entity,identifier=None,attributes)
        return wAT
    
    def createdWDF(self,Activity,identifier=None,Entity1,Entity2):
        """Define WDF relation with self as context"""
        wDF = wasDerivedFrom(Agent,Activity,Entity1,Entity2,identifier)
        e1 = Entity1
        e2 = Entity2
        return wDF
    
    
    def createEntity(self,attributes=None,identifier=None):
        
        pass
    
    def createActivity(self, identifier=None, starttime, endtime, attributes=None):
        
        starttime = starttime
        endtime = endtime
        pass
    
    def createAgent(self,identifier=None,attributes=None):
        pass
    
    def _toRDF(self):
        
        tripledict = {'subject01' : {'predicate01_01' : 'object01_01',},
                       
                      'subject02' : {'pred02_01' : 'obj02_01'}}
        return tripledict
    
    def Prov_toRDF(self):
        
        tripledict = {'sensornetworkURI' : {'wasGeneratedBy' : 'S_Network',
                                            'wasAssociatedWith' : 'S_Network',
                                            'actedOnbehalfOf' : 'S_Network'},
                                             
                      'sensornode1URI' : {'pred02_01' : 'obj02_01'},
                      'sensortemp1URI' : {'pred02_01' : 'obj02_01'},
                      'sensorlight1URI' : {'pred02_01' : 'obj02_01'}}
        
        return tripledict
    
    
    def RDF_toStore(self):
        
        
        
        
        