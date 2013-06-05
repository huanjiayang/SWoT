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
        result_graph.set_default_namespace("http://www.mytype.com/#")
        element_graph = PROVContainer()
        element_graph.set_default_namespace("http://www.mytype.com/#")
        relation_graph = PROVContainer()
        relation_graph.set_default_namespace("http://www.mytype.com/#")
        
        hasElement = False   
        user_data = web.input()
#        result.append({'attr':'value',
 #                      'attr111':'value111',
   #                    'attr444':'value444'})
        for x in user_data:
            if(x == 'show_the_database'):
                web.header('Content-Type', 'application/json')
                return json.dumps(InsGraph.to_provJSON(),indent=4)
            if(x == 'show_the_elementlist'):
                for elmt in InsGraph._elementlist:
                    element_graph.add(elmt)
                web.header('Content-Type', 'application/json')
                return json.dumps(element_graph.to_provJSON(),indent=4)
            if(x == 'show_the_relationlist'):
                for elmt in InsGraph._relationlist:
                    relation_graph.add(elmt)
                web.header('Content-Type', 'application/json')
                return json.dumps(relation_graph.to_provJSON(),indent=4)
            if (x == 'sensor_id'):
                print 'sensor_id = ' + user_data.sensor_id
                for elmt in InsGraph._elementlist:
                        if elmt.sensor_id == MT[user_data.sensor_id]:
                            a=elmt.identifier
                            result_graph.add(elmt)
                            hasElement = True
                            for elmt in InsGraph._relationlist:
                                    if elmt.__class__.__name__== 'Used':
                                        if str(elmt.activity) == str(a):
                                            result_graph.add(elmt)
                                    if elmt.__class__.__name__== 'wasGeneratedBy':
                                        if str(elmt.activity) == str(a):
                                            result_graph.add(elmt)
                                    if elmt.__class__.__name__== 'wasDerivedFrom':
                                        if str(elmt.activity) == str(a):
                                            result_graph.add(elmt)
            if (x == 'identifier'):
                print 'identifier = ' + user_data.identifier
                for elmt in InsGraph._elementlist:
                        if elmt.identifier == MT[user_data.identifier]:
                            result_graph.add(elmt)
                            hasElement = True
                for elmt in InsGraph._relationlist:
                        if elmt.identifier == MT[user_data.identifier]:
                            print elmt.identifier
                            result_graph.add(elmt)
                            hasElement = True
            if (x == 'value_type'):
                print 'value_type = ' + user_data.value_type
                for elmt in InsGraph._elementlist:
                        if elmt.value_type == MT[user_data.value_type]:
                            a=elmt.identifier
                            result_graph.add(elmt)
                            hasElement = True
                            for elmt in InsGraph._relationlist:
                                    if elmt.__class__.__name__== 'Used':
                                        if str(elmt.activity) == str(a):
                                            result_graph.add(elmt)
                                    if elmt.__class__.__name__== 'wasGeneratedBy':
                                        if str(elmt.activity) == str(a):
                                            result_graph.add(elmt)
                                    if elmt.__class__.__name__== 'wasDerivedFrom':
                                        if str(elmt.activity) == str(a):
                                            result_graph.add(elmt)
                                        
                            
            if (x == 'sensor_node_id'):
                print 'sensor_node_id = ' + user_data.sensor_node_id
                for elmt in InsGraph._elementlist:
                        if elmt.sensor_node_id == MT[user_data.sensor_node_id]:
                            result_graph.add(elmt)     
                            hasElement = True   
                a = True         
                b = True
                c = True   
                for elmt in InsGraph._relationlist:
                        if elmt.__class__.__name__ == 'wasAssociatedWith':
                            if str(elmt.agent) == str(MT[user_data.sensor_node_id]):
                                print 'relation wasAssociatedWith found'
                                if (a):
                                    result_graph.add(elmt)
                                    a = False
                                    hasElement = True
                        if elmt.__class__.__name__ == 'wasAttributedTo':
                            if str(elmt.agent) == str(MT[user_data.sensor_node_id]):
                                print 'relation wasAttributedTo found'
                                if (b):
                                    result_graph.add(elmt)
                                    b = False
                                    hasElement = True
                        if elmt.__class__.__name__ == 'actedOnBehalfOf':
                            if str(elmt.subordinate) == str(MT[user_data.sensor_node_id]):
                                print 'relation actedOnBehalfOf found'
                                if (c):
                                    result_graph.add(elmt)
                                    c = False
                                    hasElement = True
                        
        if hasElement is True:
            #result = msg_process(msg01)
            web.header('Content-Type', 'application/json')
            return json.dumps(result_graph.to_provJSON(),indent=4)
        #result_graph=None
        else:
            return 'Sorry, data not found!'
    
if __name__ == "__main__":
    app = web.application(urls, globals())
    app.run()