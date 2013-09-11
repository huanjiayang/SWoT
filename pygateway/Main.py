import os,web,time,sys,DB_connect,httplib
from web.contrib.template import render_mako
from RESTful import RESTful
 
class Resource(RESTful):
 
    def list(self):
        return "list resources", format
 
    def get(self, resource_id):
        return "retrieved resource", resource_id
 
    def delete(self, resource_id):
        return "deleted resource", resource_id
urls = (
    r'/resources(?:/(?P<resource_id>[0-9]+))?','Resource'
        )

render = render_mako(
           directories=['.'],
           input_encoding='utf-8',
           output_encoding='utf-8',
           )
    
if __name__ == "__main__":
    app = web.application(urls, globals())
    app.run()