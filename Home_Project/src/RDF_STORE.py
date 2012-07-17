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

from pyprov.model.type import *


class RDFSTORE:
    ''' Initialization method that creates indexes for triplestore, each index holds a permutation
    of the three triples stored in the graph for easy iteration '''
    def __init__(self):
        self._spo = {}
        self._pos = {}
        self._osp = {}
        
        
        
    def add(self, (subject,predicate,obj)):
        ''' Add method permutates subject,object and predicate to match ordering of each index '''
        self._addToIndex(self._spo, (subject, predicate, obj))
        self._addToIndex(self._pos, (predicate, obj, subject))
        self._addToIndex(self._osp, (obj, subject, predicate))
        
    
    def RDF_toStore(self,store,sub,pred,obj):
        ''' Add terms to index creating, creating a dictionary and set if terms are not present in index '''
        if sub not in store: 
            store[sub] = {pred:set([obj])}
        else:
            if pred not in store[sub]: 
                store[sub][pred] = set([obj])
            else: store[sub][pred].add(obj)
            

#Load triples into graph
    def load(self, filename, reader):
        mystore = plugin.get('Sleepycat', Store)('mystore')
        mystore.open("ay_folder", create=False)
        mystore = open("ay_folder", "wb")
        for line in file:
            file.write(line + '\n')
        reader = mystore.reader(file)
        for sub, pred, obj in mystore:
            sub = unicode(sub, "UTF-8")
            pred = unicode(pred, "UTF-8")
            obj = unicode(obj, "UTF-8")
            self.add((sub, pred, obj))
            mystore.close()
            
#Save triples in graph
    def save(self, filename):
        mystore = open(filename="ay_folder", "wb")
        writer = mystore.writer(mystore)
        for sub, pred, obj in self.triples((None, None, None)):
            writer.writerow([sub.encode("UTF-8"), pred.encode("UTF-8"), \
            obj.encode("UTF-8")])
            mystore.close()         
        
        
def PROVQName_URIRef(provqname):
    if isinstance(provqname,PROVQname):
        return rdflib.URIRef(provqname.name)
    else:
        return provqname
        