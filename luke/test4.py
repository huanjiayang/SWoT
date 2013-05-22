import web
import json
from instantiation import *
from web.contrib.template import render_mako

urls = (
        '/', 'SomePage',
        '/query/','Query'
        )

app = web.application(urls, globals())
render = render_mako(
           directories=['.'],
           input_encoding='utf-8',
           output_encoding='utf-8',
           )

class SomePage:
    def GET(self):
        print "running index()"
        return render.index()


class Query:
    def GET(self):
        user_data = web.input()
#        result.append({'attr':'value',
 #                      'attr111':'value111',
   #                    'attr444':'value444'})
        for x in user_data:
            if (x == 'id'):
                print 'id = ' + user_data.id
            elif (x == 'time'):
                print 'time = ' + user_data.time
            elif (x == 'temperature'):
                print 'temperature = ' + user_data.temperature
            elif (x == 'humidity'):
                print 'humidity = ' + user_data.humidity
            
        
        result = msg_process(msg01)
        
        web.header('Content-Type', 'application/json')
        return result
    
if __name__ == "__main__":
    app = web.application(urls, globals())
    app.run()