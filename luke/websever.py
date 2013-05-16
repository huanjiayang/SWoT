import web
from instantiation import *

urls = (
        '/', 'index'
        )

class index:
    def GET(self):
        import instantiation
        result = instantiation.json.dumps(InsGraph.to_provJSON(),indent=4)
        return result

if __name__ == "__main__":
    app = web.application(urls, globals())
    app.run()