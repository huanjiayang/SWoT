import os
import sys
import atexit
import time
import zipfile

import web
import json
from web.contrib.template import render_mako

from rdflib import URIRef,Literal, Namespace, Graph, ConjunctiveGraph, RDF
import rdflib
from rdfextras import *

index_dir = os.path.dirname(__file__)
ebooks_dir = os.path.join(index_dir, 'ebooks')

ns_ebook = Namespace("http://jiaojiaojiang.com/ebook/")
ns_dcterms = Namespace("http://purl.org/dc/terms/")



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
    def GET(self,ebookname):
        print "running update()"
#        return "TODO"#render.guestbook()
        #post_data = web.input(bookname="")
        #bookname = post_data.get('bookname')
        py_dir = os.path.join(ebooks_dir, ebookname)
        sys.path.insert(0, py_dir)
        import execute
        result = execute.execute_func()
        
        
        f=open('%s/%s/output.txt'% (ebooks_dir, ebookname),'a')
        s=str(result)
        t=str(time.strftime('%d-%m-%y %A %X',time.localtime(time.time())))
        f.write(s+"\n"+t+"\n"+"\n")
        
        sys.path.remove(sys.path[0])
        #return result
        output_position = 'update_result'
        rps = []
        rps.append({'finished':True,'result':str(result),'position' : output_position})
        web.header('Content-Type', 'application/json')
        return json.dumps(rps)

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
        ebook_page = '%s/%s/page.html' % (ebooks_dir, ebookname)
        rpshtmlfile = open(ebook_page)
        ebookcontent = rpshtmlfile.read()
        #rpshtmlpage += "<!--" + debug + "-->"
        rpshtmlfile.close()

     #   ebook_n3 = '%s/%s/ebook.n3' % (ebooks_dir, ebookname)
     #   ebook_n3_file = open(ebook_n3)
        
     #   temp_ebook_graph = Graph()        
     #   temp_ebook_graph.parse(ebook_n3_file, format="n3")        
        
     #   for triple in temp_ebook_graph.triples((None, RDF.type, ns_ebook['EbookFile'])):
     #       ebook_file_URI = triple[0]
     #       print ebook_file_URI
        
        vars = {
                'ebook_title': ebookname,
                'ebook_content': ebookcontent,
                'ebookname' : ebookname
            }
        return render.bookreader(**vars)

class getebookinfo:
    def GET(self,ebookname):
 
        ebook_n3 = '%s/%s/ebook.n3' % (ebooks_dir, ebookname)
        ebook_n3_file = open(ebook_n3)
        
        temp_ebook_graph = Graph()        
        temp_ebook_graph.parse(ebook_n3_file, format="n3")        
        
        for triple in temp_ebook_graph.triples((None, RDF.type, ns_ebook['EbookFile'])):
            ebook_file_URI = triple[0]
            print ebook_file_URI
            
        for triple in temp_ebook_graph.triples((ebook_file_URI, ns_dcterms['created'], None)):
            ebook_created = triple[2]
        for triple in temp_ebook_graph.triples((ebook_file_URI, ns_dcterms['creator'], None)):
            ebook_creator = triple[2]
        for triple in temp_ebook_graph.triples((ebook_file_URI, ns_dcterms['description'], None)):
            ebook_description = triple[2]
        for triple in temp_ebook_graph.triples((ebook_file_URI, ns_dcterms['language'], None)):
            ebook_language = triple[2]
        for triple in temp_ebook_graph.triples((ebook_file_URI, ns_dcterms['title'], None)):
            ebook_title = triple[2]
        
        ebook_info = []
        ebook_info.append({"title" : ebook_title,
                           "created" : ebook_created,
                           "creator" : ebook_creator,
                           "description" : ebook_description,
                           "language" : ebook_language
                           })
        
        web.header('Content-Type', 'application/json')
        return json.dumps(ebook_info)
        
        

urls = (
    '/', 'index',
    '/bookreader/', 'bookreader',
    '/update/(.*)/', 'update',
    '/ebooklist/', 'ebooklist',
    '/ebooks/(.*)/info/', 'getebookinfo',
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

