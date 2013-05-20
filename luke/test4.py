import web
import json

urls = (
        '/', 'SomePage'
        )

class SomePage:
    def GET(self):
        user_data = web.input(id="no data")
        return "<h1>" + user_data.id + "</h1>"
    
if __name__ == "__main__":
    app = web.application(urls, globals())
    app.run()