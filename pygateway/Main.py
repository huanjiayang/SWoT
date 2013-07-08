import os,web,time
class transfile:
    def GET (self):
        
    def PUSH (self):
        
    def POST (self):    
        
    def DELETE (self):
        
Input = web.input()
passfile = Input.fh          
fname = Input.filename
if passfile:
    _fl = open (os.path.join(ssPath,dlPath,fname),'w')
    _fl.write(passfile)
    _fl.close()
raise web.redirect()