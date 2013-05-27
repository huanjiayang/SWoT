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
            if (x == 'sensor_id'):
                print 'sensor_id = ' + user_data.sensor_id
                for elmt in InsGraph._elementlist:
                        if elmt.sensor_id == MT[user_data.sensor_id]:
                            result_graph.add(elmt)
                            #if result_graph == None:
                                #return 'data not found'
            if (x == 'identifier'):
                print 'identifier = ' + user_data.identifier
                for elmt in InsGraph._elementlist:
                        if elmt.identifier == MT[user_data.identifier]:
                            result_graph.add(elmt)
            if (x == 'value_type'):
                print 'value_type = ' + user_data.value_type
                for elmt in InsGraph._elementlist:
                        if elmt.value_type == MT[user_data.value_type]:
                            result_graph.add(elmt) 
            if (x == 'sensor_node_id'):
                print 'sensor_node_id = ' + user_data.sensor_node_id
                for elmt in InsGraph._elementlist:
                        if elmt.sensor_node_id == MT[user_data.sensor_node_id]:
                            result_graph.add(elmt)          
        
        #result = msg_process(msg01)
        result = json.dumps(result_graph.to_provJSON(),indent=4)
        web.header('Content-Type', 'application/json')
        return result
        #result_graph=None
    
if __name__ == "__main__":
    app = web.application(urls, globals())
    app.run()