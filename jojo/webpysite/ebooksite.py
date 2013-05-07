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

class bookreader:
    def GET(self):
        print "running news, getting news item: "
        
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

class ebooklist:
    def GET(self):        
        # get the ebook list
        ebooklist= []
        
        # construct a few ebooks here for test, but in your final program
        # these ebooks and their information should be obtained automatically by listing the ebook
        # directory 
        ebooklist.append( {"title":"this is ebook1 title", "url":"/ebooks/ebook1/"})
        ebooklist.append( {"title":"this is ebook2 title", "url":"/ebooks/ebook2/"})
               
        # return the list as JSON      
        web.header('Content-Type', 'application/json')
        return json.dumps(ebooklist)


class ebook1:
    def GET(self):
        print "Testing ebook1"
        render = render_mako(
                             directories=['ebooks/ebook1'],
                             input_encoding='utf-8',
                             output_encoding='utf-8',
                             )
        return render.ebook1()   
    
class ebook2:
    def GET(self):
        print "Testing ebook2"
        render = render_mako(
                             directories=['ebooks/ebook2'],
                             input_encoding='utf-8',
                             output_encoding='utf-8',
                             )
        return render.ebook2()   
    
urls = (
    '/', 'index',
    '/bookreader/', 'bookreader',
    '/update/', 'update',
    '/ebooklist/', 'ebooklist',
    '/ebooks/ebook1/', 'ebook1',
    '/ebooks/ebook2/', 'ebook2',
    '/(.*)','others',
)


app = web.application(urls, globals())
render = render_mako(
           directories=['template'],
           input_encoding='utf-8',
           output_encoding='utf-8',
           )



if __name__ == "__main__": app.run()


