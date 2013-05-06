'''
Created on 6 Jul 2012

@author: AYODELE-M.AKINGBULU
'''

import rdflib
import rdfextras
from rdflib.graph import Graph, ConjunctiveGraph
from rdflib.namespace import Namespace
from rdflib.term import Literal
from rdflib.term import URIRef
from rdflib import plugin
from rdflib.store import Store, NO_STORE, VALID_STORE
from datetime import date, time, datetime
import json
import datetime
from pyprov.model.type import *
from pyprov.model.relation import *
from pyprov.model.bundle import *



V, T = 0, 1



URI = 'uri:uuid:sensnetworkayodele001'
DC = Namespace('http://purl.org/dc/elements/1.1/')
FOAF = Namespace('http://xmlns.com/foaf/0.1/')
HS = Namespace('http://homesensor.com/#')
RDFS = Namespace('"http://www.w3.org/2000/01/rdf-schema#')
prov = Namespace("http://www.w3.org/ns/prov-dm/")
LIT = 'tag:infomesh.net,2001-08-07:Literal'
sensornetworkURI = 'uri:uuid:sensnetworkayodele001'

#tripledict = {}
#starttime = datetime.datetime(2012, 7, 6, 5, 4, 3)
#ag0 = Agent(sensornetworkURI)
#a0 = Activity("a0",starttime=starttime,attributes=tripledict)
#e0 = Entity()
#g0 = wasGeneratedBy(e0,a0,identifier="g0",time=None,attributes=tripledict)
#d0 = wasDerivedFrom(e0, activity=a0,generation=g0,attributes=None)
#f0 = wasGeneratedBy(e0,g0,time=None,attributes=None)
#w0 = wasAssociatedWith()
#s0 = wasStartedByActivity(a0)
#b0 = wasStartedBy(ag0,Activity=a0,Entity=e0,identifier="b0",time=None,attributes=tripledict)






class RDFSTORE:
    ''' Initialization method that creates indexes for triplestore, each index holds a permutation
    of the three triples stored in the graph for easy iteration '''
    def __init__(self, configuration=None, identifier=None, createtime=None, person=None):
        self._spo = {}
        self._pos = {}
        self._osp = {}
        
        
        
    def _ToTriples(self,p0):   
        tripledict = p0._toRDF()
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
                        self.storename.add((sub_rdfuri,pred_rdfuri,obj_rdfuri))
                else:        
                    obj_rdfuri = self.PROVQName_URIRef(tripledict[sub][pred])
                    rdftriplesdict[sub_rdfuri][pred_rdfuri] = obj_rdfuri
                    self.store.add((sub_rdfuri,pred_rdfuri,obj_rdfuri))
              
                    t = RDFSTORE()      
                    t._ToTriples()
            return rdftriplesdict

        
   
        
        
        
    def add(self, (sub, pred, obj)):
        ''' Add method permutates subject,object and predicate to match ordering of each index 
        (Adds triple to the graph)'''
        self._addToIndex(self._spo, sub, pred, obj)
        self._addToIndex(self._pos, pred, obj, sub)
        self._addToIndex(self._osp, obj, sub, pred)
        
    
    def _addToIndex(self,index,a,b,c):
        ''' Add terms to index creating, creating a dictionary and set if terms are not present in index '''
        if a not in index: 
            index[a] = {b:set([c])}
        else:
            if b not in index[a]: 
                index[a][b] = set([c])
            else: index[a][b].add(c)
            
    
            


    def triples(self, (sub, pred, obj)):
        """
        Generator over the triple store.
        Returns triples that match the given triple pattern. 
        """
        # check which terms are present in order to use the correct index:
        try:
            if sub != None: 
                if pred != None:
                    # sub pred obj
                    if obj != None:
                        if obj in self._spo[sub][pred]: yield (sub, pred, obj)
                    # sub pred None
                    else:
                        for retObj in self._spo[sub][pred]: yield (sub, pred, retObj)
                else:
                    # sub None obj
                    if obj != None:
                        for retPred in self._osp[obj][sub]: yield (sub, retPred, obj)
                    # sub None None
                    else:
                        for retPred, objSet in self._spo[sub].items():
                            for retObj in objSet:
                                yield (sub, retPred, retObj)
            else:
                if pred != None:
                    # None pred obj
                    if obj != None:
                        for retSub in self._pos[pred][obj]:
                            yield (retSub, pred, obj)
                    # None pred None
                    else:
                        for retObj, subSet in self._pos[pred].items():
                            for retSub in subSet:
                                yield (retSub, pred, retObj)
                else:
                    # None None obj
                    if obj != None:
                        for retSub, predSet in self._osp[obj].items():
                            for retPred in predSet:
                                yield (retSub, retPred, obj)
                    # None None None
                    else:
                        for retSub, predSet in self._spo.items():
                            for retPred, objSet in predSet.items():
                                for retObj in objSet:
                                    yield (retSub, retPred, retObj)
        # KeyErrors occur if a query term wasn't in the index, so we yield nothing:
        except KeyError:
            pass
            
    def value(self, sub=None, pred=None, obj=None):
        for retSub, retPred, retObj in self.triples((sub, pred, obj)):
            if sub is None: return retSub
            if pred is None: return retPred
            if obj is None: return retObj
            break
        return None
    
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
            
#Save triples in graph
    def save(self, store):
        self.graph.serialize(store, format='n3')
        store = open(store, "wb")
        writer = store.writer(store)
        for sub, pred, obj in self.rdftriples((None, None, None)):
            writer.writerow([sub.encode("UTF-8"), pred.encode("UTF-8"), \
            obj.encode("UTF-8")])
            store.close()  
            
            
#This outputs the store as Notation3 (and N-Triples)       
def n3(self,rdftriples):
    n3 = ''
    for triple in self.data:
        for term in triple:
            if term[T] == URI:
                n3 += '<' + term[V] + '>'
            elif term[T] == FOAF:
                n3 += '_:' + term[V]
            elif term[T] == LIT:
                n3 += '"' + term[V] + '"'
            elif term[T] == HS:
                n3 += '<' + term[V] + '>'
                n3 += ' '
                n3 += '.\n'
#return n3






 
#Convert PROVnamespace to RDFlib URIREF           
def PROVQName_URIRef(provqname):
    if isinstance(provqname,PROVQname):
        return rdflib.URIRef(provqname.name)
    else:
        return provqname    
    
    
    
    
if __name__ == "__main__":
    g = RDFSTORE()
    g.add(("blade_runner", "name", "Blade Runner"))
    g.add(("blade_runner", "name", "Blade Runner"))
    g.add(("blade_runner", "release_date", "June 25, 1982"))
    g.add(("blade_runner", "directed_by", "Ridley Scott"))
    
    print list(g.triples((None, None, None)))
    print list(g.triples(("blade_runner", None, None)))
    print list(g.triples(("blade_runner", "name", None)))
    print list(g.triples(("blade_runner", "name", "Blade Runner")))
    print list(g.triples(("blade_runner", None, "Blade Runner")))
    print list(g.triples((None, "name", "Blade Runner")))
    print list(g.triples((None, None, "Blade Runner")))

    print list(g.triples(("foo", "name", "Blade Runner")))
    print list(g.triples(("blade_runner", "foo", "Blade Runner")))
    print list(g.triples(("blade_runner", "name", "foo")))   
        

