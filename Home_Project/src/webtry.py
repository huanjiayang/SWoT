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


labels = file('sensor_labels.txt', 'r').readlines() #Load graph labels from a file


def get_type(self,sensorid,node_id):
        for line in labels:
            node_id, label, fieldtype = line.split(',', 2)
            if int(id) == int(sensorid):
                return fieldtype.rstrip('\r\n')
            
#Get Label reads in sensor names, determines the type name of the sensor
#to generate graphs on page properly
def get_label(sensorid, node_id, fieldtype):
    for line in labels:
        node_id, label, fieldtype = line.split(',', 2)
        print 'line: '
        print str(line)
        if int(id) == int(sensorid):
            return labels
        


        
class HS_Network:
    def GET(self, HS_Network, store):
        store = plugin.get('mystore', rdflib.store.Store)('mystore')
        store.open("mystore", create=False)
        store = open(store, "rb")
        cur = store.cursor() 
        
    #query triples in store
        cur.execute('select ?pred ?obj where {<%s> ?pred ?obj .}' % id)
       
        return json.dumps(cur.fetchone())
    
    

if __name__ == "__main__":
#Links specific classes to URLs on the web server
    urls = (
    '/', 'HS_Network' #Links Sensor class to /sensor
    
        )
    app = web.application(urls, globals()) #Run the web server.
    app.run()