'''
Created on 6 Jul 2012

@author: AYODELE-M.AKINGBULU
'''

class RDFSTORE:
    def __init__(self):
        self._subject = {}
        self._predicate = {}
        self._object = {}
        
        
        
    def add(self, (subject,predicate,obj)):
        self._addToStore(self._subject, (subject, predicate, obj))
        self._addToStore(self._predicate, (predicate, obj, subject))
        self._addToStore(self._object, (obj, subject, predicate))
        
    
    def RDF_toStore(self,store,sub,pred,obj):
        if sub not in store: 
            store[sub] = {pred:set([obj])}
        else:
            if pred not in store[sub]: 
                store[sub][pred] = set([obj])
            else: store[sub][pred].add(obj)
            