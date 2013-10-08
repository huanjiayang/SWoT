import web, MySQLdb,DB_connect
from web.contrib.template import render_mako
from lxml import etree


class index:
    def GET(self):
        #return 'test'
        #todos = db.select('value')
        return render.index()

class query:
    def GET(self):
        input = web.input()
        xml_input = input.xml
        seid = xml_input.split('<key>')[1].split('</key>')[0]

        new_conn = DB_connect.Check()   
        result = new_conn.getSensorInfo(seid)  
        print result
        
        # create XML 
        response = etree.Element('Response')
        
        # another child with text
        submitter = etree.Element('Submitter')
        sub_name = etree.Element('name')
        sub_name.text = 'MyServer'
        submitter.append(sub_name)
        
        response.append(submitter)
        
        query = etree.Element('queries')
        for s in result.keys():
            #print s
            #print result[s]
            data = etree.Element('data')
            key = etree.Element('key')
            key.text = s
            data.append(key)
            value = etree.Element('value')
            value.text = result[s]
            data.append(value)
            query.append(data)
        
        response.append(query)
        
        # pretty string
        xml_response = etree.tostring(response, pretty_print=True)
        
        web.header('Content-Type', 'text/xml')
        return xml_response
  
class list:
    def GET(self):
        new_conn = DB_connect.Check() 
        new_conn.set_up()   
        result = new_conn.readfile()  
        
        # create XML 
        response = etree.Element('Response')
        
        # another child with text
        submitter = etree.Element('Submitter')
        sub_name = etree.Element('name')
        sub_name.text = 'MyServer'
        submitter.append(sub_name)
        
        response.append(submitter)
        
        query = etree.Element('queries')
        for s in result:
            data = etree.Element('data')
            key = etree.Element('key')
            key.text = s
            data.append(key)
            query.append(data)
        
        response.append(query)
        
        # pretty string
        xml_response = etree.tostring(response, pretty_print=True)
        
        web.header('Content-Type', 'text/xml')
        return xml_response
        #return result


class error:
    def GET(self):
        print "received an unknown url"
        return "The requested URL doesn't exist."
    
        
urls = (
    '/', 'index',
    '/query/', 'query',
    '/list/', 'list',
    
    '/(.*)','error',
)

db = web.database(dbn='mysql', user='root', pw='o5kFXf98', db='pygateway')

render = render_mako(
           directories=['.'],
           input_encoding='utf-8',
           output_encoding='utf-8',
           )


if __name__ == "__main__":
    app = web.application(urls, globals())
    app.run()