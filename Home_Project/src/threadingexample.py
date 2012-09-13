import web
from threading import Thread

class SerialData(Thread):
    def __init__(self):
        Thread.__init__(self)
        self.started = False
        self.done = False
        
    def start(self):
        if self.started is False:
            Thread.start(self)
            self.started = True
    
    def run(self):
        while self.done is False:
            print('SerialData is running') # put serial port listening codes here
        
    def addtoStore(self, msg,msg_timestamp):
        pass # put the addtoStore function as a class function for SerialData
    


class HS_Network:
    def GET(self):
        return 'Web server is also running'


if __name__ == "__main__":
#Links specific classes to URLs on the web server
    urls = (
    '/homesensorcom/', 'HS_Network', #Links URL homesensor.com to HS_Network Class
        )
    
    sData = SerialData()
    sData.start()
    
    app = web.application(urls, globals()) #Run the web server.
    app.run()