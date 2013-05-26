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
        result_graph = PROVContainer()     
        user_data = web.input()
#        result.append({'attr':'value',
 #                      'attr111':'value111',
   #                    'attr444':'value444'})
        for x in user_data:
            if (x == 'id'):
                print 'id = ' + user_data.id
                for elmt in InsGraph._elementlist:
                        if elmt.value_type == MT['Humidity']:
                            result_graph.add(elmt)
            elif (x == 'time'):
                print 'time = ' + user_data.time
                for elmt in InsGraph._elementlist:
                        if elmt.identifier == MT['8766']:
                            result_graph.add(elmt)
            elif (x == 'temperature'):
                print 'temperature = ' + user_data.temperature
                for elmt in InsGraph._elementlist:
                        if elmt.identifier == MT['8766S0201']:
                            result_graph.add(elmt) 
            elif (x == 'humidity'):
                print 'humidity = ' + user_data.humidity
                for elmt in InsGraph._elementlist:
                        if elmt.identifier == MT['8766S0202']:
                            result_graph.add(elmt)          
        
        #result = msg_process(msg01)
        result = json.dumps(result_graph.to_provJSON(),indent=4)
        web.header('Content-Type', 'application/json')
        return result
        #result_graph=None
    
if __name__ == "__main__":
    app = web.application(urls, globals())
    app.run()