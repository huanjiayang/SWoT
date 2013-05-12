import os
import sys
import atexit
import time
import zipfile

import web
import json
from web.contrib.template import render_mako

index_dir = os.path.dirname(__file__)
ebooks_dir = os.path.join(index_dir, 'ebooks')



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

class update:
    def GET(self):
        print "running guestbook()"
        return "TODO"#render.guestbook()
    def POST(self):
        post_data = web.input(bookname="")
        bookname = post_data.get('bookname')
        py_dir = os.path.join(ebooks_dir, bookname)
        sys.path.insert(0, py_dir)
        import execute
        result = execute.execute_func()
        sys.path.remove(sys.path[0])
        return result

class ebooklist:
    def GET(self):        
        # get the ebook list
        ebooklist= []
        
        # construct a few ebooks here for test, but in your final program
        # these ebooks and their information should be obtained automatically by listing the ebook
        # directory 
        #ebooklist.append( {"title":"this is ebook1 title", "url":"/ebooks/ebook1/"})
        #ebooklist.append( {"title":"this is ebook2 title", "url":"/ebooks/ebook2/"})
        
        for book_name in os.listdir(ebooks_dir):
            if book_name.startswith('.'):
                continue
            ebooklist.append({"title": book_name, "url":"/ebooks/"+book_name+"/"})
               
        # return the list as JSON      
        web.header('Content-Type', 'application/json')
        return json.dumps(ebooklist)


class startreading:
    def GET(self,ebookname):

        ebookcontent = ''
        ebook_uri = '%s/%s/page.html' % (ebooks_dir, ebookname)
        rpshtmlfile = open(ebook_uri)
        ebookcontent = rpshtmlfile.read()
        #rpshtmlpage += "<!--" + debug + "-->"
        rpshtmlfile.close()
        
        vars = {
                'ebook_title': ebookname,
                'ebook_content': ebookcontent
            }
        return render.bookreader(**vars)
    
urls = (
    '/', 'index',
    '/bookreader/', 'bookreader',
    '/update/', 'update',
    '/ebooklist/', 'ebooklist',
    '/ebooks/(.*)/', 'startreading',
    '/(.*)','others',
)


app = web.application(urls, globals())
render = render_mako(
           directories=['template'],
           input_encoding='utf-8',
           output_encoding='utf-8',
           )

if __name__ == "__main__":
    app.run()


