import os
import sys
import atexit
import time
import zipfile

import web
import json
from web.contrib.template import render_mako


class index:   
    def GET(self):
        print "running index()"
        return render.maininterface()

class test:
    def GET(self):
        print "running news, getting news item: "
        
        
        
        ebookcontent=""
        
        ebookcontent = "<h2>this is the content</h2>"
        
        vars = {
                'ebook_title': "my ebook title",
                'ebook_content': ebookcontent
            }
        return render.bookreader(**vars)

class others:
    def GET(self,para):
        print "accepting unknown url"
        return "The requested URL doesn't exist."

class update():
    def GET(self):
        print "running guestbook()"
        return render.guestbook()

    
urls = (
    '/', 'index',
    '/test/', 'test',
    '/update/', 'update',
    '/(.*)','others',
)


app = web.application(urls, globals())
render = render_mako(
           directories=['template'],
           input_encoding='utf-8',
           output_encoding='utf-8',
           )


if __name__ == "__main__": app.run()


