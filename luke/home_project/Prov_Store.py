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
class Mystore():
    
    def __init__(self, configuration, id):
        self.store = rdflib.Graph(store='Sleepycat',identifier=id)
        self.store.open(configuration, create=True) 
        self.store.bind('prov','http://www.w3.org/ns/prov-dm/')
           

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
                    
        #print rdftriplesdict
        return rdftriplesdict

#Convert PROV URIREF to RDFlib URIREF            
    def PROVQName_URIRef(self,provqname):
        if isinstance(provqname,PROVQname):
            return rdflib.URIRef(provqname.name)
        else:
            return provqname             
        
    #match subject to predicates and objects
    def addPROVInstance(self, d0):
        #self=rdflib.Graph(store='Sleepycat',identifier='store')
        #self.open("myfolder", create=False)
        rdftriplesdict = self._ToTriples(d0)
        maindict = {}
        for sub in rdftriplesdict.keys():
            for pred in rdftriplesdict[sub].keys():
                if isinstance(rdftriplesdict[sub][pred],list):
                    for obj in rdftriplesdict[sub][pred]:
                        self.store.add((sub,pred,obj))
                else:
                    self.store.add((sub,pred,rdftriplesdict[sub][pred]))
                
            return maindict
   

              
               

# -------------------- class ends -----------------------------------------------
# -------------------- execution begins -----------------------------------------

# Define Namespace
hs = Namespace("http://homesensor.com#")
prov = Namespace("http://www.w3.org/ns/prov-dm/")
DC = Namespace('http://purl.org/dc/elements/1.1/')
FOAF = Namespace("foaf","http://xmlns.com/foaf/0.1/")
RDF = rdflib.Namespace("http://www.w3.org/TR/rdf-schema/#")
S_Network = hs['SensorNetwork']


