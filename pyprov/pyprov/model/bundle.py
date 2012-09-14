from type import *
from relation import *

class Bundle():
    
    def __init__(self):
        self._provcontainer = {}
        self._elementlist = []
        self._relationlist = []
        self._namespacedict = {}
        self._implicitnamespace = {'prov':'http://www.w3.org/ns/prov-dm/',
                                   'xsd' :'http://www.w3.org/2001/XMLSchema-datatypes#'}
        self._accountlist = []
        self._elementkey = 0
        self._relationkey = 0
        self._auto_ns_key = 0
        if self.identifier is None:
            self.identifier = PROVQname("default", localname="default")
        self._idJSON = None
   
    def add(self,record):
        if record is not None:
            if isinstance(record,PROVType):
                self._validate_record(record)
                if record.account is None:
                    self._elementlist.append(record)
                    record.account = self
                elif not record.account.identifier == self.identifier:
                    record.account.add(record)
                elif not record in self._elementlist:
                    self._elementlist.append(record)
            elif isinstance(record,Relation):
                self._validate_record(record)
                if record.account is None:
                    self._relationlist.append(record)
                    record.account = self
                elif not record.account.identifier == self.identifier:
                    record.account.add(record)
                elif not record in self._elementlist:
                    self._relationlist.append(record)
            elif isinstance(record,Account):
                if record.parentaccount is None:
                    self._accountlist.append(record)
                    record.parentaccount = self
                elif not record.parentaccount.identifier == self.identifier:
                    record.account.add(record)
                elif not record in self._accountlist:
                    self._accountlist.append(record)

    def empty(self):
        self._provcontainer = {}
        self._elementlist = []
        self._relationlist = []
        self._namespacedict = {}
        self._accountlist = []
        self._elementkey = 0
        self._relationkey = 0
        self._auto_ns_key = 0
        self._idJSON = None
            
    def to_provJSON(self,nsdict):
        self._generate_idJSON(nsdict)
        for element in self._elementlist:
            if isinstance(element,Agent):
                if not 'agent' in self._provcontainer.keys():
                    self._provcontainer['agent']=[]
                self._provcontainer['agent'].append(element.identifier)
            jsondict = element.to_provJSON(nsdict)
            for key in jsondict:
                if not key in self._provcontainer.keys():
                    self._provcontainer[key]={}
                self._provcontainer[key].update(jsondict[key])
        for relation in self._relationlist:
            jsondict = relation.to_provJSON(nsdict)
            for key in jsondict:
                if not key in self._provcontainer.keys():
                    self._provcontainer[key]={}
                self._provcontainer[key].update(jsondict[key])
        for account in self._accountlist:
            if not 'account' in self._provcontainer.keys():
                self._provcontainer['account']={}
            if not account._idJSON in self._provcontainer['account'].keys():
                self._provcontainer['account'][account._idJSON]={}
            self._provcontainer['account'][account._idJSON].update(account.to_provJSON(nsdict))
        return self._provcontainer

    def _generate_idJSON(self,nsdict):
        for element in self._elementlist:
            if element.identifier is None:
                element._idJSON = self._generate_elem_identifier()
            else:
                print "generate idJSON for %s" % str(element.identifier)
                element._idJSON = element.identifier.qname(nsdict)
        for relation in self._relationlist:
            if relation.identifier is None:
                relation._idJSON = self._generate_rlat_identifier()
            else:
                print "generate idJSON for %s" % str(relation.identifier)
                relation._idJSON = relation.identifier.qname(nsdict)
        for account in self._accountlist:
            print "generate idJSON for %s" % str(account.identifier)
            account._idJSON = account.identifier.qname(nsdict)
            account._generate_idJSON(nsdict)
                    
    def add_namespace(self, prefix, uri):
        #TODO: add prefix validation here
        if prefix is "default":
            raise PROVGraph_Error("The namespace prefix 'default' is a reserved by provpy library")
        else:
            self._namespacedict[prefix] = uri
#            self._apply_namespace(prefix, url)

    def get_namespaces(self):
        return self._namespacedict 
    
    def _generate_rlat_identifier(self):
        identifier = "_:RLAT"+str(self._relationkey)
        self._relationkey = self._relationkey + 1
        if self._validate_id(identifier) is False:
            identifier = self._generate_rlat_identifier()
        return identifier

    def _generate_elem_identifier(self):
        identifier = "_:ELEM"+str(self._elementkey)
        self._elementkey = self._elementkey + 1
        if self._validate_id(identifier) is False:
            identifier = self._generate_elem_identifier()
        return identifier
        
    def _validate_id(self, identifier):
        valid = True
        for element in self._elementlist:
            if element._idJSON == identifier:
                valid = False
        for relation in self._relationlist:
            if relation._idJSON == identifier:
                valid = False
        return valid

    def add_entity(self,identifier=None,attributes=None,account=None):
        if self._validate_id(identifier) is False:
            raise PROVGraph_Error('Identifier conflicts with existing assertions')
        else:
            element=Entity(identifier,attributes,account)
            self.add(element)
            return element
    
    def add_activity(self,identifier=None,starttime=None,endtime=None,attributes=None,account=None):
        if self._validate_id(identifier) is False:
            raise PROVGraph_Error('Identifier conflicts with existing assertions')
        else:
            element=Activity(identifier,starttime,endtime,attributes,account=account)
            self.add(element)
            return element
    
    def add_agent(self,identifier=None,attributes=None,account=None):
        if self._validate_id(identifier) is False:
            raise PROVGraph_Error('Identifier conflicts with existing assertions')
        else:
            element=Agent(identifier,attributes,account=account)
            self.add(element)
            return element
    
    def add_note(self,identifier=None,attributes=None,account=None):
        if self._validate_id(identifier) is False:
            raise PROVGraph_Error('Identifier conflicts with existing assertions')
        else:
            element=Note(identifier,attributes,account=account)
            self.add(element)
            return element

    def add_wasGeneratedBy(self,entity,activity,identifier=None,time=None,attributes=None,account=None):
        if identifier is not None:
            if self._validate_id(identifier) is False:
                raise PROVGraph_Error('Identifier conflicts with existing assertions')
        else:
            relation=wasGeneratedBy(entity,activity,identifier,time,attributes,account=account)
            self.add(relation)
            return relation

    def add_used(self,activity,entity,identifier=None,time=None,attributes=None,account=None):
        if identifier is not None:
            if self._validate_id(identifier) is False:
                raise PROVGraph_Error('Identifier conflicts with existing assertions')
        else:
            relation=Used(activity,entity,identifier,time,attributes,account=account)
            self.add(relation)
            return relation

    def add_wasAssociatedWith(self,activity,agent,identifier=None,attributes=None,account=None):
        if identifier is not None:
            if self._validate_id(identifier) is False:
                raise PROVGraph_Error('Identifier conflicts with existing assertions')
        else:
            relation=wasAssociatedWith(activity,agent,identifier,attributes,account=account)
            self.add(relation)
            return relation

    def add_wasStartedBy(self,activity,agent,identifier=None,attributes=None,account=None):
        if identifier is not None:
            if self._validate_id(identifier) is False:
                raise PROVGraph_Error('Identifier conflicts with existing assertions')
        else:
            relation=wasStartedBy(activity,agent,identifier,attributes,account=account)
            self.add(relation)
            return relation

    def add_wasEndedBy(self,activity,agent,identifier=None,attributes=None,account=None):
        if identifier is not None:
            if self._validate_id(identifier) is False:
                raise PROVGraph_Error('Identifier conflicts with existing assertions')
        else:
            relation=wasEndedBy(activity,agent,identifier,attributes,account=account)
            self.add(relation)
            return relation

    def add_actedOnBehalfOf(self,subordinate,responsible,identifier=None,attributes=None,account=None):
        if identifier is not None:
            if self._validate_id(identifier) is False:
                raise PROVGraph_Error('Identifier conflicts with existing assertions')
        else:
            relation=actedOnBehalfOf(subordinate,responsible,identifier,attributes,account=account)
            self.add(relation)
            return relation
      
    def add_wasDerivedFrom(self,generatedentity,usedentity,identifier=None,activity=None,generation=None,usage=None,attributes=None,account=None):
        if identifier is not None:
            if self._validate_id(identifier) is False:
                raise PROVGraph_Error('Identifier conflicts with existing assertions')
        else:
            relation=wasDerivedFrom(generatedentity,usedentity,identifier,activity,generation,usage,attributes,account=account)
            self.add(relation)
            return relation
    
    def add_alternateOf(self,subject,alternate,identifier=None,attributes=None,account=None):
        if identifier is not None:
            if self._validate_id(identifier) is False:
                raise PROVGraph_Error('Identifier conflicts with existing assertions')
        else:
            relation=alternateOf(subject,alternate,identifier,attributes,account=account)
            self.add(relation)
            return relation

    def add_specializationOf(self,subject,specialization,identifier=None,attributes=None,account=None):
        if identifier is not None:
            if self._validate_id(identifier) is False:
                raise PROVGraph_Error('Identifier conflicts with existing assertions')
        else:
            relation=specializationOf(subject,specialization,identifier,attributes,account=account)
            self.add(relation)
            return relation
    
    def add_hasAnnotation(self,record,note,identifier=None,attributes=None,account=None):
        if identifier is not None:
            if self._validate_id(identifier) is False:
                raise PROVGraph_Error('Identifier conflicts with existing assertions')
        else:
            relation=hasAnnotation(record,note,identifier,attributes,account=account)
            self.add(relation)
            return relation

    def add_account(self,identifier,parentaccount=None):
        acc = Account(identifier,parentaccount)
        self.add(acc)

    def _validate_record(self,record):
        pass # Put possible record validation here

    def _validate_qname(self,qname):
        pass # Put possible Qname validation here
    
    def get_records(self):
        records = []
        records.extend(self._elementlist)
        records.extend(self._relationlist)
        records.extend(self._accountlist)
        return records
            

class PROVContainer(Bundle):
    
    def __init__(self,defaultnamespace=None):
        self.defaultnamespace=defaultnamespace
        self.identifier = None
        Bundle.__init__(self)
        self._visitedrecord = []
        self._nsdict = {}
        
    def set_default_namespace(self,defaultnamespace):
        self.defaultnamespace = defaultnamespace
        
    def get_default_namespace(self):
        return self.defaultnamespace

    def _create_nsdict(self):
        self._auto_ns_key = 0
        if not self.defaultnamespace is None:
            self._nsdict = {'default':self.defaultnamespace}
        self._nsdict.update(self._implicitnamespace)
        self._nsdict.update(self._namespacedict)
        self._visitedrecord = []
        self._merge_namespace(self)
        for prefix,namespacename in self._nsdict.items():
            if namespacename == self.defaultnamespace:
                if not prefix == "default":
                    del self._nsdict[prefix]
        return self._nsdict

    def _merge_namespace(self,obj):
        self._visitedrecord.append(obj)
        if isinstance(obj,Bundle):
            for prefix,namespacename in obj._namespacedict.items():
                tempqname = PROVQname(namespacename,prefix,namespacename,None)
                self._merge_namespace(tempqname)
            for element in obj._elementlist:
                for attr in element._attributelist:
                    if not attr in self._visitedrecord:
                        self._merge_namespace(attr)
            for relation in obj._relationlist:
                for attr in relation._attributelist:
                    if not attr in self._visitedrecord:
                        self._merge_namespace(attr)
            for account in obj._accountlist:
                if not account in self._visitedrecord:
                    self._merge_namespace(account)
        if isinstance(obj,PROVQname):
            if not obj.prefix is None:
                if obj.prefix in self._nsdict.keys():
                    if not obj.namespacename == self._nsdict[obj.prefix]:
                        if not self._nsdict["default"] == obj.namespacename:
                            newprefix = self._generate_prefix()
                            self._nsdict[newprefix] = obj.namespacename
                elif not obj.namespacename in self._nsdict:
                    self._nsdict[obj.prefix] = obj.namespacename
        if isinstance(obj,list):
            for item in obj:
                self._merge_namespace(item)
        if isinstance(obj,dict):
            for key,value in obj.items():
                self._merge_namespace(key)
                self._merge_namespace(value)

    def _generate_prefix(self):
        prefix = "ns" + str(self._auto_ns_key)
        self._auto_ns_key = self._auto_ns_key + 1
        if prefix in self._nsdict.keys():
            prefix = self._generate_prefix()
        return prefix 

    def to_provJSON(self):
        nsdict = self._create_nsdict()
        Bundle.to_provJSON(self,nsdict)
        self._provcontainer['prefix']={}
        for prefix,url in nsdict.items():
            self._provcontainer['prefix'][prefix]=url

        if self.defaultnamespace is not None:
            if not "default" in self._provcontainer['prefix'].keys():
                self._provcontainer['prefix']['default']=self.defaultnamespace
            else:
                pass # TODO: what if a namespace with prefix 'default' is already defined
        return self._provcontainer
    
    def _toRDF(self):
        Bundle._toRDF(self)


class Account(Record,Bundle):
    
    def __init__(self, identifier, asserter, parentaccount=None, attributes=None):
        if identifier is None:
            raise PROVGraph_Error("The identifier of PROV account record must be given as a string or an PROVQname")
        Record.__init__(self, identifier, attributes, parentaccount)
        self.prov_type = PROV_REC_ACCOUNT

        Bundle.__init__(self)
            
        if isinstance(asserter,PROVQname):
            self.asserter = asserter
        elif isinstance(asserter, (str, unicode)):
            self.asserter = PROVQname(identifier, localname=identifier)
        else:
            raise PROVGraph_Error("The asserter of PROV account record must be given as a string or an PROVQname")
        
        self.asserter = asserter
        self._record_attributes = asserter
        self.parentaccount=parentaccount
        
    def get_record_attributes(self):
        record_attributes = {}
        record_attributes['asserter'] = self.asserter
        return record_attributes
    
    def get_asserter(self):
        return self.asserter
    
    def to_provJSON(self,nsdict):
        Bundle.to_provJSON(self,nsdict)
        self._provcontainer['asserter']=self.asserter.qname(nsdict)
        for attribute,value in self.attributes.items():
            attrtojson = attribute
            if isinstance(attribute, PROVQname):
                attrtojson = attribute.qname(nsdict)
            valuetojson = value
            if isinstance(value, PROVQname):
                valuetojson = value.qname(nsdict)
            self._provcontainer[attrtojson] = valuetojson
        for attribute in self._provcontainer.keys():
            if isinstance(attribute, PROVQname):
                attrtojson = attribute.qname(nsdict)
                self._provcontainer[attrtojson] = self._provcontainer[attribute]
                del self._provcontainer[attribute]
        return self._provcontainer
    
    def _toRDF(self):
        Bundle._toRDF(self)
        self.rdftriples[self.identifier] = {}
        if attributes is not None:
            for attr in self.attributes.keys():
                self.rdftriples[self.identifier][attr] = self.attributes[attr]
        return self.rdftriples