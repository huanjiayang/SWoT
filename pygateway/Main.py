import os,web,time,sys,DB_connect,httplib
from web.contrib.template import render_mako
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

class Getfile:
    def GET(self):
        form = searchform()
        return render.formtest(form)
    def PUSH(self):
        pass
    def POST(self):    
        form = searchform()
        if not form.validates():
            return render.formtest(form)
        else:
            word = form['word'].get_value()
            print "You are searching %s for the word %s" % (form['site'].get_value(), word)
            raise web.redirect('/%s') % word
    def DELETE(self):
        pass  
    
class SomePage:
    def GET(self):
        print "running index()"
        return "This page is for user to get the information from sensor network!"


urls = (
        '/', 'SomePage'
        )


render = render_mako(
           directories=['.'],
           input_encoding='utf-8',
           output_encoding='utf-8',
           )
    
if __name__ == "__main__":
    app = web.application(urls, globals())
    app.run()