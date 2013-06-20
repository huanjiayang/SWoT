import json
import web
import instantiation
from instantiation import *

urls = (
        '/', 'index'
        )

class index:
    def GET(self):
        result = json.dumps(InsGraph.to_provJSON(),indent=4)
        web.header('Content-Type', 'application/json')
        return result

class SomePage:
    def GET(self):
        user_data = web.input()
        return "<h1>" + user_data.id + "</h1>"


if __name__ == "__main__":
    app = web.application(urls, globals())
    app.run()