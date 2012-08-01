'''
Created on 19 Jun 2012

@author: AYODELE-M.AKINGBULU
'''

# Import packages
import web #Generate content creation and for web server implementation
import re
import doctest
from rdflib.store import Store, NO_STORE, VALID_STORE
from rdflib import Namespace, BNode, Literal, RDF, URIRef
import urllib
#import rdflib as db
from rdflib import plugin
from Prov_Store import *
from math import log #Logarithims function for performing conversions
from time import time # To generate time stamps
import json # Generate JavaScript Object Notation
import SocketServer 
import os 
import commands 
import logging
import sys

#sys.argv[1:] = [ip, port]



labels = file('sensor_labels.txt', 'r').readlines() #Load graph labels from a file
#store = plugin.get('mystore', Store)('mystore') 
render = web.template.render('templates/')






            


#Get Label reads in sensor names, determines the type name of the sensor
#to generate graphs on page properly
def get_label(sensorid, node_id, fieldtype):
    for line in labels:
        node_id, label, fieldtype = line.split(',', 2)
        print 'line: '
        print str(line)
        if int(id) == int(sensorid):
            return labels
        

#Get the sensor type from the snesorlabels.txt file, this allows the script
#to process the data properly for a specific type of sensor.
def get_type(sensorid,node_id):
    for line in labels:
        node_id, label, fieldtype = line.split(',', 2)
        if int(id) == int(sensorid):
            return fieldtype.rstrip('\r\n')
        
        
        

class Sensor(object):
    def GET(self, sensorid):
        args = web.input()
        start = int(args.get('start', time() - 10000))
        db = rdflib.store('mystore')
        db = web.database('mystore')
        cur = db.cursor()
        cur.execute('select ?')
        
        results = []
        for node_id, sensorid, ts, reading in cur.fetchall(): 
            if get_type(sensorid) == 'TEMP':
                reading = round((reading), 2) #Convert Data
            results.append((int(ts * 1000), reading)) #Write results to array
            
        
            
        return json.dumps({'sensor': sensorid, 'label': get_label(sensorid), 'data': results})
    
    
class HS_Location:
    def GET(self, HS_Location):
        store = plugin.get('mystore', rdflib.store.Store)('mystore')
        store.open("mystore", create=False)
        store = open(store, "rb")
        cur = store.cursor() 
        
    #query triples in store
        cur.execute('select ?pred ?obj where {<%s> ?pred ?obj .}' % id)
       
        return json.dumps(cur.fetchone())

# Link classes to url on the server
urls = ('/homesensor.com/(.*)', 'Sensor'
        '/Sensor/', "Sensor", #Links Sensor class to /sensor
        '/HS_location', "HS_Location" # Links HS_Location class to /location
        )

#Main Method, calls appropriate classes
if __name__ == "__main__":
    
# To run the web server to start serving the web pages with data    
    app = web.application(urls, globals())
    app.run()
    
    
    