import web

class RESTful:
 
    methods = ("list", "get", "delete", "delete_collection")
 
    def __getattr__(self, name):
        if name in self.methods and "headers" in web.ctx:
            raise web.badrequest()
        else:
            raise AttributeError
 
    def GET(self, resource_id=None):
        if resource_id is None:
            return self.list()
        else:
            return self.get(resource_id)
 
    def DELETE(self, resource_id=None):
        if resource_id is None:
            return self.delete_collection()
        else:
            return self.delete(resource_id)