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
        return render.index()

class project:   
    def GET(self):
        print "running project()"
        return render.project()


class publication:   
    def GET(self):
        print "running publication()"
        return render.publication()
    
    
class activity:   
    def GET(self):
        print "running activity()"
        return render.activity()

class interest:   
    def GET(self):
        print "running interest()"
        return render.interest()

class news:
    def GET(self,news_item):
        print "running news, getting news item: " + news_item
        if news_item == "phd":
            return render.phd()
        else:
            return "The requested news doesn't exist."

class others:
    def GET(self,para):
        print "accepting unknown url"
        return "The requested URL doesn't exist."

class guestbook():
    def GET(self):
        print "running guestbook()"
        return render.guestbook()

    
urls = (
    '/', 'index',
    '/project/', 'project',
    '/publication/', 'publication',
    '/activity/', 'activity',
    '/interest/', 'interest',
    '/news/(.*)/','news',
    '/guestbook/','guestbook',
    '/(.*)','others',
)

app = web.application(urls, globals())
render = render_mako(
           directories=['html_templates'],
           input_encoding='utf-8',
           output_encoding='utf-8',
           )


if __name__ == "__main__": app.run()


