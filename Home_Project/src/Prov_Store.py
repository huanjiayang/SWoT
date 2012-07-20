import rdflib
import rdfextras
from rdflib.graph import Graph
from rdflib.namespace import Namespace
from rdflib.term import Literal
from rdflib.term import URIRef
from rdflib import plugin
from rdflib.store import Store, NO_STORE, VALID_STORE
from datetime import date, time, datetime
from pyprov.model.type import *
from pyprov.model.relation import *
from pyprov.model.bundle import *
import pyprov



# Define Namespace
hs = Namespace("http://homesensor.com#")
prov = Namespace("http://www.w3.org/ns/prov-dm/")
dc = Namespace('http://purl.org/dc/elements/1.1/')
FOAF = Namespace("foaf","http://xmlns.com/foaf/0.1/")

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
        
    def addNetwork(self,sensornetworkURI,createtime,person,wasDerivedFrom,wasGeneratedBy):
#        graph.add((sensornetworkURI, RDF.type, S_Network))
#        graph.add((sensornetworkURI, dcterms['create'], createtime))
#        graph.add((sensornetworkURI, prov['wasGeneratedBy'], person))
        tripledict = {}
        starttime = datetime.datetime(2012, 7, 6, 5, 4, 3)
        ag0 = Agent(sensornetworkURI,attribute={})
        a0 = Activity("a0",starttime=starttime,attributes=tripledict)
        e0 = Entity()
        g0 = wasGeneratedBy(e0,a0,identifier="g0",time=None,attributes=tripledict)
        d0 = wasDerivedFrom(e0, activity=a0,generation=g0,attributes=None)
        f0 = wasGeneratedBy(e0,g0,time=None,attributes=None)
        w0 = wasAssociatedWith()
        s0 = wasStartedByActivity(a0)
        b0 = wasStartedBy(ag0,Activity=a0,Entity=e0,identifier="b0",time=None,attributes=tripledict)
        e1 = Entity(FOAF['Foo'],attributes=tripledict)
        
        self.provgraph.add(e0,w0,a0,d0,f0,s0,b0,g0,e1)
    
    def convertDataToPROV(self,rawdata):
        pass
    
    
    def createWGB(self,Agent=None,Activity,Entity,identifier=None,time=None,attributes=None,wasGeneratedBy):
        """Define WGB relation with self as context"""
        wGB = wasGeneratedBy(Activity,Entity,identifier,time,attributes)
        
        attrdict = {}
        return wGB
   
    def createWAW(self,identifier=None,Agent,Activity,attributes,wasAssociatedWith):
        """Define WAW relation with self as context"""
        wAW = wasAssociatedWith(Agent,Activity,Entity,identifier)
        return wAW
    
    def createWSBA(self,identifier=None,started,starter,attributes,wasStartedByActivity):
        """Define WSBA relation with self as context"""
        wSBA = wasStartedByActivity(Activity,identifier,started,starter)
        a1 = starter
        a2 = started
        return wSBA
    
    def createWSB(self,identifier=None,Activity=None,Entity=None,time=None,attributes=None,wasStartedBy):
        """Define WSB relation with self as context"""
        wSB = wasStartedBy(Agent,Activity,Entity,identifier,time,attributes)
        return wSB
    
    def createdAOBO(self,Agent2,Agent1,Activity,identifier,attributes,actedOnBehalfOf,responsible):
        """Define AOBO relation with self as context"""
        aOBO = actedOnBehalfOf(Agent2,Agent1,Activity,identifier,attributes) 
        ag1 = Agent1
        ag2 = Agent2
        URIRef = Activity
        rdf.type = actedOnBehalfOf
        Literal  = identifier
        return aOBO
    
    def createdWAT(self,Entity,Agent,identifier=None,attributes,wasAttributedTo):
        """Define WAT relation with self as context"""
        wAT = wasAttributedTo(Agent,Entity,identifier=None,attributes)
        return wAT
    
    def createdWDF(self,Activity,identifier=None,Entity1,Entity2,wasDerivedFrom):
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
 
    def _ToTriples(self,attributes=None,identifier=None,e1,ag0,a0,g0,d0,f0,w0,s0,b0):
           
        tripledict = e1._toRDF()
        for sub in tripledict.keys():
            print str(sub)
            for pred in tripledict[sub].keys():
                print str(pred)
        if isinstance(tripledict[sub][pred],list):
            for obj in tripledict[sub][pred]:
                print 'object: ' + str(obj) + '     '
        else:        
            print str(tripledict[sub][pred])
        
        tripledict = ag0._toRDF()
        for sub in tripledict.keys():
            print str(sub)
            for pred in tripledict[sub].keys():
                print str(pred)
        if isinstance(tripledict[sub][pred],list):
            for obj in tripledict[sub][pred]:
                print 'object: ' + str(obj) + '     '
        else:        
            print str(tripledict[sub][pred])
            
        tripledict = a0._toRDF()
        for sub in tripledict.keys():
            print str(sub)
            for pred in tripledict[sub].keys():
                print str(pred)
        if isinstance(tripledict[sub][pred],list):
            for obj in tripledict[sub][pred]:
                print 'object: ' + str(obj) + '     '
        else:        
            print str(tripledict[sub][pred])
            
        tripledict = g0._toRDF()
        for sub in tripledict.keys():
            print str(sub)
            for pred in tripledict[sub].keys():
                print str(pred)
        if isinstance(tripledict[sub][pred],list):
            for obj in tripledict[sub][pred]:
                print 'object: ' + str(obj) + '     '
        else:        
            print str(tripledict[sub][pred])
            
        tripledict = d0._toRDF()
        for sub in tripledict.keys():
            print str(sub)
            for pred in tripledict[sub].keys():
                print str(pred)
        if isinstance(tripledict[sub][pred],list):
            for obj in tripledict[sub][pred]:
                print 'object: ' + str(obj) + '     '
        else:        
            print str(tripledict[sub][pred])
            
        tripledict = f0._toRDF()
        for sub in tripledict.keys():
            print str(sub)
            for pred in tripledict[sub].keys():
                print str(pred)
        if isinstance(tripledict[sub][pred],list):
            for obj in tripledict[sub][pred]:
                print 'object: ' + str(obj) + '     '
        else:        
            print str(tripledict[sub][pred])
            
        tripledict = w0._toRDF()
        for sub in tripledict.keys():
            print str(sub)
            for pred in tripledict[sub].keys():
                print str(pred)
        if isinstance(tripledict[sub][pred],list):
            for obj in tripledict[sub][pred]:
                print 'object: ' + str(obj) + '     '
        else:        
            print str(tripledict[sub][pred])
            
        tripledict = s0._toRDF()
        for sub in tripledict.keys():
            print str(sub)
            for pred in tripledict[sub].keys():
                print str(pred)
        if isinstance(tripledict[sub][pred],list):
            for obj in tripledict[sub][pred]:
                print 'object: ' + str(obj) + '     '
        else:        
            print str(tripledict[sub][pred])
            
        tripledict = b0._toRDF()
        for sub in tripledict.keys():
            print str(sub)
            for pred in tripledict[sub].keys():
                print str(pred)
        if isinstance(tripledict[sub][pred],list):
            for obj in tripledict[sub][pred]:
                print 'object: ' + str(obj) + '     '
        else:        
            print str(tripledict[sub][pred])
            
     
#Function to convert sub,pred,obj to URIRef data type     
    def _toPROVRDF(self):
        
        
#Convert PROVnamespace to RDFlib URIREF           
     def PROVQName_URIRef(self,provqname,PROVQname):
         if isinstance(provqname,PROVQname):
            return rdflib.URIRef(provqname.name)
         else:
          return provqname       
    
    
    #Load triples into graph
    def load(self, store, load):
        store = plugin.get('Sleepycat', rdflib.store.Store)('rdfstore')
        store.open("ay_folder", create=False)
        store = open(store, "rb")
        for line in file:
            file.write(line + '\n')
        load = store.load(store)
        for sub, pred, obj in load:
            sub = unicode(sub, "UTF-8")
            pred = unicode(pred, "UTF-8")
            obj = unicode(obj, "UTF-8")
            self.add((sub, pred, obj))
            store.close()
        
        
         
        

        
        
        