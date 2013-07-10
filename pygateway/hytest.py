import web
from web.contrib.template import render_mako


class SomePage:
    def GET(self):
        print "running index()"
        return "this is a test"


urls = (
        '/', 'SomePage'
        )


render = render_mako(
           directories=['.'],
           input_encoding='utf-8',
           output_encoding='utf-8',
           )
    
if __name__ == "__main__":
    app = web.application(urls, globals())
    app.run()