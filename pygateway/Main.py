
import os,web,time,sys,DB_connect
DB_connect
class Sensor:
    def id(self):
        print "This sensor's ID is:" + self 
    def type(self):
        print "The type of this sensor is:" + self

class Transducer:
    def id(self):
        print "This Transducer's ID is:" + self 
    def mac_addr(self):
        print "The Mac address of this Transducer is:" + self
        
class Netroll: 
    def ip(self):
        print "This Transducer's IP is:" + self 
    def roll(self):
        print "The roll of this equipment is:" + self
        
class Network:
    def id(self):
        print "This Network's ID is:" + self 
    def type(self):
        print "The type of this Network is" + self

class transfile:
    def GET(self):
        pass
    def PUSH(self):
        pass
    def POST(self):    
        pass
    def DELETE(self):
        pass  
Input = web.input()
passfile = Input.fh          
fname = Input.filename
if passfile:
    _fl = open (os.path.join(ssPath,dlPath,fname),'w')
    _fl.write(passfile)
    _fl.close()
raise web.redirect()

if _name_ == "_main_" :
    main()