
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
