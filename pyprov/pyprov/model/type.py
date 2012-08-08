import datetime
#from lxml.cssselect import ident

PROV_REC_ENTITY                 = 1
PROV_REC_ACTIVITY               = 2
PROV_REC_AGENT                  = 3
PROV_REC_NOTE                   = 9
PROV_REC_ACCOUNT                = 10
PROV_REC_GENERATION             = 11
PROV_REC_USAGE                  = 12
PROV_REC_ACTIVITY_ASSOCIATION   = 13
PROV_REC_START                  = 14
PROV_REC_END                    = 15
PROV_REC_RESPONSIBILITY         = 16
PROV_REC_DERIVATION             = 17
PROV_REC_ALTERNATE              = 18
PROV_REC_SPECIALIZATION         = 19
PROV_REC_ANNOTATION             = 99
PROV_REC_INVALIDATION           = 101

PROV_RECORD_TYPES = (
    (PROV_REC_ENTITY,               u'Entity'),
    (PROV_REC_ACTIVITY,             u'Activity'),
    (PROV_REC_AGENT,                u'Agent'),
    (PROV_REC_NOTE,                 u'Note'),
    (PROV_REC_ACCOUNT,              u'Account'),
    (PROV_REC_GENERATION,           u'Generation'),
    (PROV_REC_USAGE,                u'Usage'),
    (PROV_REC_ACTIVITY_ASSOCIATION, u'ActivityAssociation'),
    (PROV_REC_START,                u'Start'),
    (PROV_REC_END,                  u'End'),
    (PROV_REC_RESPONSIBILITY,       u'Responsibility'),
    (PROV_REC_DERIVATION,           u'Derivation'),
    (PROV_REC_ALTERNATE,            u'Alternate'),
    (PROV_REC_SPECIALIZATION,       u'Specialization'),
    (PROV_REC_ANNOTATION,           u'Annotation'),
)

class PROVIdentifier(object):
    
    def __init__(self,name):
        self.name = name
        
    def uri(self):
        return self.name

class PROVQname(PROVIdentifier):
    
    def __init__(self,name,prefix=None,namespacename=None,localname=None):
        PROVIdentifier.__init__(self, name)
        self.namespacename = namespacename
        self.localname = localname
        self.prefix = prefix
        
    def __str__(self):
        return self.name
    
    def __eq__(self,other):
        if not isinstance(other,PROVQname):
            return False
        else:
            return self.name == other.name
    
    def __hash__(self):
        return id(self)
    
    def qname(self,nsdict):
        rt = self.name
        for prefix,namespacename in nsdict.items():
            if self.namespacename == namespacename:
                if prefix <> 'default':
                    if self.localname is not None:
                        rt = ":".join((prefix, self.localname))
                else:
                    rt = self.localname
        if not self.namespacename in nsdict.values():
            if self.prefix is not None:
                rt = ":".join((self.prefix, self.localname))
        return rt
    
    def to_provJSON(self,nsdict):
        qname = self.qname(nsdict)
        if self.name == qname:
            rt = [qname,"xsd:anyURI"]
        else:
            rt = [qname,"xsd:QName"]
        return rt


class PROVNamespace(PROVIdentifier):
    
    def __init__(self,prefix,namespacename):
        self.prefix = prefix
        self.namespacename = str(namespacename)
        
    def __getitem__(self,localname):
        return PROVQname(self.namespacename+localname,self.prefix,self.namespacename,localname)

        
xsd = PROVNamespace("xsd",'http://www.w3.org/2001/XMLSchema-datatypes#')
prov = PROVNamespace("prov",'http://www.w3.org/ns/prov-dm/')
rdf = PROVNamespace("rdf","http://www.w3.org/TR/rdf-schema/#")


class Record(object):

    def __init__(self, identifier=None, attributes=None, account=None):
        if identifier is not None:
            if isinstance(identifier, PROVQname):
                self.identifier = identifier
            elif isinstance(identifier, (str, unicode)):
                self.identifier = PROVQname(identifier, localname=identifier)
            else:
                raise PROVGraph_Error("The identifier of PROV record must be given as a string or an PROVQname")
        else:
            self.identifier = None
        
        
        self._record_attributes = {}

        #TODO Remove the following code. It is here to maintain compatibility with the current JSON export code            
        if attributes is None:
            self.attributes = {}
        else:
            self.attributes = attributes

        self.account = account
        
        self.rdftriples = {}
        
    def __str__(self):
        if self.identifier is not None:
            return str(self.identifier)
        #TODO should we return None here?
        return None
        
#    def __getattr__(self, attr):
#        if attr in self._attributes:
#            return self._attributes[attr]
#        # If no attribute could be found, raise the standard error
#        raise AttributeError, attr
    
    def _get_type_JSON(self,value):
        datatype = None
        if isinstance(value,str) or isinstance(value,bool):
            datatype = None
        if isinstance(value,float):
            datatype = xsd["float"]
        if isinstance(value,datetime.datetime):
            datatype = xsd["dateTime"]
        if isinstance(value,int):
            datatype = xsd["integer"]
        if isinstance(value,list):
            datatype = prov["array"]
        return datatype
        
    def _convert_value_JSON(self,value,nsdict):
        valuetojson = value
        if isinstance(value,PROVLiteral): 
            valuetojson=value.to_provJSON(nsdict)
        elif isinstance(value,PROVQname):
            valuetojson=value.to_provJSON(nsdict)
        else:
            datatype = self._get_type_JSON(value)
            if datatype is not None:
                if not datatype == prov["array"]:
                    valuetojson=[str(value),datatype.qname(nsdict)]
                else:
                    newvalue = []
                    islist = False
                    for item in value:
                        if isinstance(item,list):
                            islist = True
                        newvalue.append(self._convert_value_JSON(item, nsdict))
                    if islist is False:
                        valuetojson=[newvalue,datatype.qname(nsdict)]
        return valuetojson
    
    def get_prov_type(self):
        return self.prov_type

    def get_record_id(self):
        return self.identifier
    
    def get_record_attributes(self):
        return dict()
    
    def get_other_attributes(self):
        # It might be needed to return an immutable copy to avoid accidental modifications
        return self.attributes
    
    def get_all_attributes(self):
        attributes = self.get_record_attributes()
        attributes.update(self.attributes) 
        return attributes
    
    def _toRDF(self):
        self.rdftriples[self.identifier] = {}
        if self.attributes is not None:
            for attr in self.attributes.keys():
                self.rdftriples[self.identifier][attr] = self.attributes[attr]
                
        return self.rdftriples


class PROVType(Record):
    
    def __init__(self, identifier=None, attributes=None, account=None):
        if identifier is None:
            raise PROVGraph_Error("An PROVType is always required to have an identifier")
        Record.__init__(self, identifier, attributes, account)
        
        self._json = {}
        self._provcontainer = {}
        self._idJSON = None
        self._attributelist = [self.identifier, self.account, self.attributes]
        
    def to_provJSON(self,nsdict):
        if isinstance(self.identifier,PROVQname):
            self._idJSON = self.identifier.qname(nsdict)
        elif self.identifier is None:
            if self._idJSON is None:
                self._idJSON = 'NoID'
        self._json[self._idJSON]=self.attributes.copy()
        for attribute in self._json[self._idJSON].keys():
            if isinstance(attribute, PROVQname):
                attrtojson = attribute.qname(nsdict)
                self._json[self._idJSON][attrtojson] = self._json[self._idJSON][attribute]
                del self._json[self._idJSON][attribute]
        for attribute,value in self._json[self._idJSON].items():
            valuetojson = self._convert_value_JSON(value,nsdict)
            if valuetojson is not None:
                self._json[self._idJSON][attribute] = valuetojson
        return self._json
    
    def _toRDF(self):
        Record._toRDF(self)
        
        # Add here the codes that are specific for this class (in this case PROVType)
        
        return self.rdftriples


class Entity(PROVType):

    def __init__(self, identifier=None, attributes=None, account=None):
        PROVType.__init__(self, identifier, attributes, account)
        self.prov_type = PROV_REC_ENTITY
        
    def to_provJSON(self,nsdict):
        PROVType.to_provJSON(self,nsdict)
        self._provcontainer['entity']=self._json
        return self._provcontainer
    
    def _toRDF(self):
        PROVType._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = prov['Entity']
        return self.rdftriples
    

class Activity(PROVType):
    
    def __init__(self, identifier=None, starttime=None, endtime=None, attributes=None, account=None):
        PROVType.__init__(self, identifier, attributes, account)
        self.prov_type = PROV_REC_ACTIVITY
        
        self.starttime=starttime
        self.endtime=endtime
        self._attributelist.extend([self.starttime,self.endtime])
        
    def get_record_attributes(self):
        record_attributes = {}
        if self.starttime is not None:
            record_attributes['startTime'] = self.starttime
        if self.endtime is not None:
            record_attributes['endTime'] = self.endtime
        return record_attributes
        
    def to_provJSON(self,nsdict):
        PROVType.to_provJSON(self,nsdict)
        if self.starttime is not None:
            self._json[self._idJSON]['prov:starttime']=self._convert_value_JSON(self.starttime,nsdict)
        if self.endtime is not None:
            self._json[self._idJSON]['prov:endtime']=self._convert_value_JSON(self.endtime,nsdict)
        self._provcontainer['activity']=self._json
        return self._provcontainer
    
    def to_RDF(self):
        PROVType.to_RDF(self)
        self.rdftriples[self.identifier][rdf['type']] = prov['Activity']
        if self.starttime is not None:
            self.rdftriples[self.identifier][prov['starttime']] = self.starttime
        if self.endtime is not None:
            self.rdftriples[self.identifier][prov['endtime']] = self.endtime
        #self.rdftriples[self.identifier][rdf['type']] = prov['Activity']
        return self.rdftriples

class Agent(Entity):

    def __init__(self, identifier=None, attributes=None, account=None):
        Entity.__init__(self, identifier, attributes, account)
        self.prov_type = PROV_REC_AGENT
        
    def to_provJSON(self,nsdict):
        PROVType.to_provJSON(self,nsdict)
        self._provcontainer['entity']=self._json
        #TODO: How to mark an Agent?
        return self._provcontainer
    
    def to_RDF(self):
        Entity.to_RDF(self)
        self.rdftriples[self.identifier][rdf['type']] = prov['Agent']
        return self.rdftriples
        
        
        

class Note(PROVType):

    def __init__(self, identifier=None, attributes=None, account=None):
        PROVType.__init__(self, identifier, attributes, account)
        self.prov_type = PROV_REC_NOTE
        
    def to_provJSON(self,nsdict):
        PROVType.to_provJSON(self,nsdict)
        self._provcontainer['note']=self._json
        return self._provcontainer
    
    def _toRDF(self):
        PROVType._toRDF(self)
        self.rdftriples[self.identifier][rdf['type']] = prov['Note']
        return self.rdftriples



    

class PROVLiteral():
    
    def __init__(self, value, datatype):
        self.value = value
        self.datatype = datatype
        self._json = []
        
    def __str__(self):
        return 'Not supported yet'
        
    def to_provJSON(self,nsdict):
        self._json = []
        if isinstance(self.value, PROVQname):
            self._json.append(self.value.qname(nsdict))
        else:
            self._json.append(self.value)
        if isinstance(self.datatype, PROVQname):
            self._json.append(self.datatype.qname(nsdict))
        else:
            self._json.append(self.datatype)
        return self._json



    

class PROVGraph_Error(Exception):
    def __init__(self, error_message):
        self.error_message = error_message
    def __str__(self):
        return repr(self.error_message)
