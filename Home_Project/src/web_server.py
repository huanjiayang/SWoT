'''
Created on 19 Jun 2012

@author: AYODELE-M.AKINGBULU
'''

# Import packages
import web #Generate content creation and for web server implementation
import re
import uuid
from rdflib.store import Store, NO_STORE, VALID_STORE
from RDF_STORE import *
from math import log #Logarithims function for performing conversions
from time import time # To generate time stamps
import json # Generate JavaScript Object Notation
import SocketServer 
import os 
import commands 
import logging




labels = file('sensor_labels.txt', 'r').readlines() #Load graph labels from a file


#Get Label reads in sensor names, determines the type name of the sensor
#to generate graphs on page properly
def get_label(sensorid, node_id, fieldtype):
    for line in labels:
        node_id, label, fieldtype = line.split(',', 2)
        if int(id) == int(sensorid):
            return label
        

#Get the sensor type from the snesorlabels.txt file, this allows the script
#to process the data properly for a specific type of sensor.
def get_type(sensorid,node_id):
    for line in labels:
        node_id, label, fieldtype = line.split(',', 2)
        if int(id) == int(sensorid):
            return fieldtype.rstrip('\r\n')
        
        
        

class Sensor(object):
    def GET(self, sensorid, convert_temp):
        args = web.input()
        start = int(args.get('start', time() - 86400))
        db = rdflib.store('rdfstore')
        cur = db.cursor()
        cur.execute('SELECT * FROM motedata WHERE ts > ? AND sensorid=? ORDER BY ts LIMIT 100', (start, sensorid))
        results = []
        for node_id, sensorid, ts, reading in cur.fetchall(): 
            if get_type(sensorid) == 'TEMP':
                reading = round(convert_temp(reading), 2) #Convert Data
            results.append((int(ts * 1000), reading)) #Write results to array
        return json.dumps({'sensor': sensorid, 'label': get_label(sensorid), 'data': results})
    
    
class HS_Location:
    def GET(self, HS_Location):
        store = plugin.get('Sleepycat', rdflib.store.Store)('rdfstore')
        store.open("ay_folder", create=False)
        store = open(store, "rb")
        cur = store.cursor() 
        
    #Grab the most recent location from the database
        cur.execute('SELECT ts,latitude,longitude FROM location ORDER BY id DESC LIMIT 1')
        #Return last known location to the browser as a JSON dump, this is processed by the
        #Google Maps API javascript
        return json.dumps(cur.fetchone())

# Link classes to url on the server
urls = ('/homesensor.com/(.*)', 'Sensor'
        '/sensor/(?P<sensorid>[0-9]+)', Sensor, #Links Sensor class to /sensor
        '/location', HS_Location # Links HS_Location class to /location
        )

#Main Method, calls appropriate classes
if __name__ == "__main__":
    
# To run the web server to start serving the web pages with data    
    app = web.application(urls, globals())
    app.run()
    
    