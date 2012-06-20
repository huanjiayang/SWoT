'''
Created on 19 Jun 2012

@author: AYODELE-M.AKINGBULU
'''

# Import packages
import web
from time import time
import rdflib
import json



# Link classes to url on the server
urls = ('/homesensor.com/(.*)', 'Sensor')

class index:
    def GET(self):
        return "Hello, world!"

# Link classes to url on the server
urls = ('/homesensor.com/(.*)', 'Sensor')


#Main Method, calls appropriate classes
if __name__ == "__main__":
    



# To run the web server to start serving the web pages with data    
    app = web.application(urls, globals())
    app.run()
    
    