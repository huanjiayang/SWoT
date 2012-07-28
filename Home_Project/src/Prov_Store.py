import rdflib
import rdfextras
from rdflib.graph import Graph
from rdflib.namespace import Namespace
from rdflib.term import Literal
from rdflib.term import URIRef
from rdflib import plugin
from rdflib import RDF
from rdflib.store import Store, NO_STORE, VALID_STORE
from datetime import date, time, datetime
from pyprov.model.type import *
from pyprov.model.relation import *
from pyprov.model.bundle import *
import pyprov

# --------------------- class begins ------------------------------------------- 
# pyprov instances of prov relations to RDF triples
class Mystore(Graph):
    
    def __init__(self, configuration, id):
        Graph.__init__(self, configuration, id)
        self.open("myfolder", False)
        #sensor_graph=rdflib.Graph(store='Sleepycat',identifier='store')
        #sensor_graph.open("provfolder", create=True)
        return      
        
    #match subject to predicates and objects
    def addPROVInstance(self, d0):
        sensor_graph=rdflib.Graph(store='Sleepycat',identifier='store')
        sensor_graph.open("myfolder", create=False)
        rdftriplesdict = self._ToTriples(d0)
        maindict = {}
        for sub_rdfuri in rdftriplesdict.keys():
            temp_sub = rdftriplesdict.keys()
            if temp_sub != sub_rdfuri:
                maindict[sub_rdfuri] = {}
                #self.store.add((sub_rdfuri))        

            for pred_rdfuri in rdftriplesdict[sub_rdfuri].keys():
                temp_pred = rdftriplesdict[sub_rdfuri].keys()
                if temp_pred != pred_rdfuri:
                    maindict[sub_rdfuri][pred_rdfuri] = []
                    #self.store.add((pred_rdfuri))            
                    
                for obj_rdfuri in rdftriplesdict.keys():
                    temp_obj  = rdftriplesdict[sub_rdfuri][pred_rdfuri]
                    if temp_obj != obj_rdfuri:
                        maindict[sub_rdfuri][pred_rdfuri].append(obj_rdfuri)
                        self.add((obj_rdfuri,sub_rdfuri,pred_rdfuri))
                
                
            return maindict
   
    def _ToTriples(self, d0):   
        tripledict = d0._toRDF()
        rdftriplesdict = {}
        for sub in tripledict.keys():
            sub_rdfuri = self.PROVQName_URIRef(sub)
            rdftriplesdict[sub_rdfuri] = {}
            
            for pred in tripledict[sub].keys():
                pred_rdfuri = self.PROVQName_URIRef(pred)
                if isinstance(tripledict[sub][pred],list):
                    rdftriplesdict[sub_rdfuri][pred_rdfuri] = []
                    for obj in tripledict[sub][pred]:
                        obj_rdfuri = self.PROVQName_URIRef(obj)
                        rdftriplesdict[sub_rdfuri][pred_rdfuri].append(obj_rdfuri)
                        #self.add((sub_rdfuri,pred_rdfuri,obj_rdfuri))
                        
                else:        
                    obj_rdfuri = self.PROVQName_URIRef(tripledict[sub][pred])
                    rdftriplesdict[sub_rdfuri][pred_rdfuri] = obj_rdfuri
                    #self.add((sub_rdfuri,pred_rdfuri,obj_rdfuri))  
                    
          
        return rdftriplesdict

#Convert PROV URIREF to RDFlib URIREF            
    def PROVQName_URIRef(self,provqname):
        if isinstance(provqname,PROVQname):
            return rdflib.URIRef(provqname.name)
        else:
            return provqname             
              
               
##Add triples into store
#    def load(self,store,(sub, pred, obj)):
#        store = plugin.get('Sleepycat', rdflib.store.Store)('rdfstore')
#        store.open("ay_folder", create=False)
#        store = open(store, "rb")
#        #for line in file:
#        #file.write(line + '\n')
#        file.write(graph.serialize(format="n3"))
#        load = store.load(store)
#        for sub, pred, obj in load:
#            sub = unicode(sub, "UTF-8")
#            pred = unicode(pred, "UTF-8")
#            obj = unicode(obj, "UTF-8")
#            self.add((sub, pred, obj))
#            store.close()
#            
            
#    def save(self, store):
#        self.graph.serialize(store, format='n3')
#        store = open(store, "wb")
#        writer = store.writer(store)
#        for sub, pred, obj in self.rdftriples((None, None, None)):
#            writer.writerow([sub.encode("UTF-8"), pred.encode("UTF-8"), \
#            obj.encode("UTF-8")])
#            store.close()  
#        return
# -------------------- class ends -----------------------------------------------
# -------------------- execution begins -----------------------------------------

# Define Namespace
hs = Namespace("http://homesensor.com#")
prov = Namespace("http://www.w3.org/ns/prov-dm/")
DC = Namespace('http://purl.org/dc/elements/1.1/')
FOAF = Namespace("foaf","http://xmlns.com/foaf/0.1/")
RDF = rdflib.Namespace("http://www.w3.org/TR/rdf-schema/#")

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

Communication = URIRef("http://homesensor.com/Activity/")
Discovery = URIRef("http://homesensor.com/Activity/")
S_Node = URIRef("http://homesensor.com/S_node/")
Sensor = URIRef("http://homsensor.com/Sensor/")

#--------------------------------------------------------------------------------
#store = sensor_graph.store

#person = 'ayo001'
#
#tripledict = {}
#starttime = datetime.datetime(2012, 7, 6, 5, 4, 3)
#a1 = "starter"
#a2 =   "started"
#ag0 = Agent(sensornetworkURI)
#a0 = Activity("a0",starttime=starttime,attributes=tripledict)
#e0 = Entity(identifier="e0")
#e1 = Entity(FOAF['Foo'],attributes=tripledict)
#g0 = wasGeneratedBy(e0,a0,identifier="g0",time=None,attributes=tripledict)
#u0 = Used(a0,e1,identifier="u0",time=None,attributes=tripledict)
#d0 = wasDerivedFrom(e0, e1, activity=a0,generation=g0,usage=u0,attributes=None)
#f0 = wasGeneratedBy(e0,g0,time=None,attributes=None)
#w0 = wasAssociatedWith(ag0,a0,identifier="w0",attributes=tripledict)
#s0 = wasStartedByActivity(a0,a1,a2,attributes=None)
#b0 = wasStartedBy(e0,a0,identifier="b0",attributes=None)
#        
#graph = Graph()
#graph.add(("blade_runner", "name", "Blade Runner"))
#graph.add(("blade_runner", "release_date", "June 25, 1982"))
#graph.add(("blade_runner", "directed_by", "Ridley Scott"))
#
#print list(graph.triples((None, None, None)))
#print list(graph.triples(("blade_runner", None, None)))
#print list(graph.triples(("blade_runner", "name", None)))
#print list(graph.triples(("blade_runner", "name", "Blade Runner")))
#print list(graph.triples(("blade_runner", None, "Blade Runner")))
#print list(graph.triples((None, "name", "Blade Runner")))
#print list(graph.triples((None, None, "Blade Runner")))
#
#print list(graph.triples(("foo", "name", "Blade Runner")))
#print list(graph.triples(("blade_runner", "foo", "Blade Runner")))
#print list(graph.triples(("blade_runner", "name", "foo")))
    
#sensor_graph = Mystore('Sleepycat', 'mystore')   

#sensor_graph.addPROVInstance(e0)
#sensor_graph.addPROVInstance(w0)
#sensor_graph.addPROVInstance(a0)
#sensor_graph.addPROVInstance(f0)
#sensor_graph.addPROVInstance(ag0)
##sensor_graph.addPROVInstance(s0)
#sensor_graph.addPROVInstance(b0)
#sensor_graph.addPROVInstance(d0)
#sensor_graph.addPROVInstance(e1)
#sensor_graph.addPROVInstance(g0)

## print out a.sensor_graph
