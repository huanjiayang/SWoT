from type import *


class Relation(Record):

    def __init__(self, identifier=None, attributes=None, account=None):
        Record.__init__(self, identifier, attributes, account)

        self._json = {}
        self._provcontainer = {}
        self._idJSON = None
        self._attributelist = [self.identifier,self.account,self.attributes]
    
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
        
        return self.rdftriples
        
    
# Entity to Entity relations

class wasDerivedFrom(Relation):
    
    def __init__(self, generatedentity, usedentity, identifier=None, activity=None, generation=None, usage=None, attributes=None, account=None):
        #TODO Enforce mandatory attributes as required by PROV-DM
        Relation.__init__(self,identifier,attributes,account)
        self.prov_type = PROV_REC_DERIVATION
        self.generatedentity = generatedentity
        self.usedentity = usedentity
        self.activity = activity
        self.generation = generation
        self.usage = usage
        self._attributelist.extend([self.generatedentity,self.usedentity,self.activity,self.generation,self.usage])
        
    def get_record_attributes(self):
        record_attributes = {}
        record_attributes['generatedEntity'] = self.generatedentity
        record_attributes['usedEntity'] = self.usedentity
        if self.activity is not None:
            record_attributes['activity'] = self.activity
        if self.generation is not None:
            record_attributes['generation'] = self.generation
        if self.usage is not None:
            record_attributes['usage'] = self.usage
        return record_attributes


    def to_provJSON(self,nsdict):
        Relation.to_provJSON(self,nsdict)
        self._json[self._idJSON]['prov:generatedentity']=self.generatedentity._idJSON
        self._json[self._idJSON]['prov:usedentity']=self.usedentity._idJSON
        if self.activity is not None:
            self._json[self._idJSON]['prov:activity']=self.activity._idJSON
        if self.generation is not None:
            self._json[self._idJSON]['prov:generation']=self.generation._idJSON
        if self.usage is not None:
            self._json[self._idJSON]['prov:usage']=self.usage._idJSON
        self._provcontainer['wasDerivedFrom']=self._json
        return self._provcontainer
    
    def _toRDF(self):
        Relation._toRDF(self)
        self.rdftriples[self.identifier][prov['generatedentity']]=self.generatedentity.identifier
        self.rdftriples[self.identifier][prov['usedentity']]=self.usedentity.identifier
        if self.activity is not None:
            self.rdftriples[self.identifier][prov['activity']]=self.activity.identifier
        if self.generation is not None:
            self.rdftriples[self.identifier][prov['generation']]=self.generation.identifier
        if self.usage is not None:
            self.rdftriples[self.identifier][prov['usage']]=self.usage.identifier
        self.rdftriples[self.identifier][rdf['type']] = prov['wasDerivedFrom']
        return self.rdftriples
        
                        

class wasRevisionOf(Relation):
    
    def __init__(self, newer, older, responsibility=None, identifier=None, attributes=None, account=None):
        Relation.__init__(self, identifier, attributes, account)
#        self.prov_type = PROV_REC_GENERATION
        self.newer=newer
        self.older=older
        self.responsibility=responsibility
        self._attributelist.extend([self.newer,self.older])
        if self.responsibility is not None:
            self._attributelist.extend([self.responsibility])
        
    def get_record_attributes(self):
        record_attributes = {}
        record_attributes['newer'] = self.newer
        record_attributes['older'] = self.older
        if self.responsibility is not None:
            record_attributes['responsibility'] = self.responsibility
        return record_attributes
    
    def to_provJSON(self,nsdict):
        Relation.to_provJSON(self,nsdict)
        self._json[self._idJSON]['prov:newer']=self.newer._idJSON
        self._json[self._idJSON]['prov:older']=self.older._idJSON
        if self.responsibility is not None:
            self._json[self._idJSON]['prov:responsibility']=self.responsibility._idJSON
        self._provcontainer['wasRevisionOf']=self._json
        return self._provcontainer
    
    def _toRDF(self):
        Relation._toRDF(self)
        self.rdftriples[self.identifier][prov['newer']] = self.newer.identifier
        self.rdftriples[self.identifier][prov['older']] = self.older.identifier
        if self.responsibility is not None:
            self.rdftriples[self.identifier][prov['responsibility']]=self.responsibility.identifier
        self.rdftriples[self.identifier][rdf['type']] = prov['wasRevisionOf']
        return self.rdftriples
    


class wasQuotedFrom(Relation):
    
    def __init__(self, quote, quoted, quoterAgent=None, quotedAgent=None, identifier=None, attributes=None, account=None):
        Relation.__init__(self, identifier, attributes, account)
#        self.prov_type = PROV_REC_GENERATION
        self.quote=quote
        self.quoted=quoted
        self.quoterAgent=quoterAgent
        self.quotedAgent=quotedAgent
        self._attributelist.extend([self.quote,self.older])
        if self.quoterAgent is not None:
            self._attributelist.extend([self.quoterAgent])
        if self.quotedAgent is not None:
            self._attributelist.extend([self.quotedAgent])
        
    def get_record_attributes(self):
        record_attributes = {}
        record_attributes['quote'] = self.quote
        record_attributes['quoted'] = self.quoted
        if self.quoterAgent is not None:
            record_attributes['quoterAgent'] = self.quoterAgent
        if self.quotedAgent is not None:
            record_attributes['quotedAgent'] = self.quotedAgent
        return record_attributes
    
    def to_provJSON(self,nsdict):
        Relation.to_provJSON(self,nsdict)
        self._json[self._idJSON]['prov:quote']=self.quote._idJSON
        self._json[self._idJSON]['prov:quoted']=self.quoted._idJSON
        if self.quoterAgent is not None:
            self._json[self._idJSON]['prov:quoterAgent']=self.quoterAgent._idJSON
        if self.quotedAgent is not None:
            self._json[self._idJSON]['prov:quotedAgent']=self.quotedAgent._idJSON
        self._provcontainer['wasQuotedFrom']=self._json
        return self._provcontainer
    
    def _toRDF(self):
        Relation._toRDF(self)
        self.rdftriples[self.identifier][prov['quote']]=self.quote.identifier
        self.rdftriples[self.identifier][prov['quoted']]=self.quoted.identifier
        if self.quoterAgent is not None:
            self.rdftriples[self.identifier][prov['quoterAgent']]=self.quoterAgent.identifier
        if self.quoterAgent is not None:
            self.rdftriples[self.identifier][prov['quotedAgent']]=self.quotedAgent.identifier
        self.rdftriples[self.identifier][rdf['type']]=prov['wasQuotedFrom']
        return self.rdftriples


class hadOriginalSource(Relation):
    
    def __init__(self, entity, source, identifier=None, attributes=None, account=None):
        Relation.__init__(self, identifier, attributes, account)
#        self.prov_type = PROV_REC_GENERATION
        self.entity=entity
        self.source=source
        self._attributelist.extend([self.entity,self.source])
        
    def get_record_attributes(self):
        record_attributes = {}
        record_attributes['entity'] = self.entity
        record_attributes['source'] = self.source
        return record_attributes
    
    def to_provJSON(self,nsdict):
        Relation.to_provJSON(self,nsdict)
        self._json[self._idJSON]['prov:entity']=self.entity._idJSON
        self._json[self._idJSON]['prov:source']=self.source._idJSON
        self._provcontainer['hadOriginalSource']=self._json
        return self._provcontainer
    
    def _toRDF(self):
        Relation._toRDF(self)
        self.rdftriples[self.identifier][prov['entity']]=self.entity.identifier
        self.rdftriples[self.identifier][prov['source']]=self.source.identifier
        self.rdftriples[self.identifier][rdf['type']]= prov['hadOriginalSource']
        return self.rdftriples

class alternateOf(Relation):
    
    def __init__(self,subject,alternate,identifier=None,attributes=None,account=None):
        Relation.__init__(self,identifier,attributes,account)
        self.prov_type = PROV_REC_ALTERNATE
        self.subject = subject
        self.alternate = alternate
        self._attributelist.extend([self.subject,self.alternate])
        
    def get_record_attributes(self):
        record_attributes = {}
        record_attributes['subject'] = self.subject
        record_attributes['alternate'] = self.alternate
        return record_attributes


    def to_provJSON(self,nsdict):
        Relation.to_provJSON(self,nsdict)
        self._json[self._idJSON]['prov:subject']=self.subject._idJSON
        self._json[self._idJSON]['prov:alternate']=self.alternate._idJSON
        self._provcontainer['alternateOf']=self._json
        return self._provcontainer
    
    def _toRDF(self):
        Relation._toRDF(self)
        self.rdftriples[self.identifier][prov['subject']]=self.subject.identifier
        self.rdftriples[self.identifier][prov['alternate']]=self.alternate.identifier
        self.rdftriples[self.identifier][rdf['type']]= prov['alternateOf']
        return self.rdftriples
    

class specializationOf(Relation):
    
    def __init__(self,subject,specialization,identifier=None,attributes=None,account=None):
        Relation.__init__(self,identifier,attributes,account)
        self.prov_type = PROV_REC_SPECIALIZATION
        self.subject = subject
        self.specialization = specialization
        self._attributelist.extend([self.subject,self.specialization])
        
    def get_record_attributes(self):
        record_attributes = {}
        record_attributes['subject'] = self.subject
        record_attributes['specialization'] = self.specialization
        return record_attributes

    def to_provJSON(self,nsdict):
        Relation.to_provJSON(self,nsdict)
        self._json[self._idJSON]['prov:subject']=self.subject._idJSON
        self._json[self._idJSON]['prov:specialization']=self.specialization._idJSON
        self._provcontainer['specializationOf']=self._json
        return self._provcontainer
    
    def _toRDF(self):
        Relation._toRDF(self)
        self.rdftriples[self.identifier][prov['subject']]=self.subject.identifier
        self.rdftriples[self.identifier][prov['specialization']]=self.specialization.identifier
        self.rdftriples[self.identifier][rdf['type']]= prov['specializationOf']
        return self.rdftriples
               

#Entity to Activity relations
  
class wasGeneratedBy(Relation):
    
    def __init__(self, entity, activity, identifier=None, time=None, attributes=None, account=None):
        Relation.__init__(self, identifier, attributes, account)
        self.prov_type = PROV_REC_GENERATION
        self.entity=entity
        self.activity=activity
        self.time = time
        self._attributelist.extend([self.entity,self.activity,self.time])
        
    def get_record_attributes(self):
        record_attributes = {}
        record_attributes['entity'] = self.entity
        record_attributes['activity'] = self.activity
        if self.time is not None:
            record_attributes['time'] = self.time
        return record_attributes
    
    def to_provJSON(self,nsdict):
        Relation.to_provJSON(self,nsdict)
        self._json[self._idJSON]['prov:entity']=self.entity._idJSON
        self._json[self._idJSON]['prov:activity']=self.activity._idJSON
        if self.time is not None:
            self._json[self._idJSON]['prov:time']=self._convert_value_JSON(self.time,nsdict)
        self._provcontainer['wasGeneratedBy']=self._json
        return self._provcontainer
    
    def _toRDF(self):
        Relation._toRDF(self)
        self.rdftriples[self.identifier][prov['entity']]=self.entity.identifier
        self.rdftriples[self.identifier][prov['activity']]=self.activity.identifier
        if self.time is not None:
            self.rdftriples[self.identifier][prov['time']]=self.time
        self.rdftriples[self.identifier][rdf['type']]=prov['wasGeneratedBy']
        return self.rdftriples


class wasInvalidatedBy(Relation):

    def __init__(self, entity, activity, identifier=None, time=None, attributes=None, account=None):
        Relation.__init__(self, identifier, attributes, account)
        self.prov_type = PROV_REC_INVALIDATION
        self.entity=entity
        self.activity=activity
        self.time = time
        self._attributelist.extend([self.entity,self.activity,self.time])
        
    def get_record_attributes(self):
        record_attributes = {}
        record_attributes['entity'] = self.entity
        record_attributes['activity'] = self.activity
        if self.time is not None:
            record_attributes['time'] = self.time
        return record_attributes
    
    def to_provJSON(self,nsdict):
        Relation.to_provJSON(self,nsdict)
        self._json[self._idJSON]['prov:entity']=self.entity._idJSON
        self._json[self._idJSON]['prov:activity']=self.activity._idJSON
        if self.time is not None:
            self._json[self._idJSON]['prov:time']=self._convert_value_JSON(self.time,nsdict)
        self._provcontainer['wasInvalidatedBy']=self._json
        return self._provcontainer
    
    def _toRDF(self):
        Relation._toRDF(self)
        self.rdftriples[self.identifier][prov['entity']]=self.entity.identifier
        self.rdftriples[self.identifier][prov['activity']]=self.activity.identifier
        if self.time is not None:
            self.rdftriples[self.identifier][prov['time']]=self.time
        self.rdftriples[self.identifier][rdf['type']]=prov['wasInvalidatedBy']
        return self.rdftriples
    
    


# Entity to Agent relation

class wasAttributedTo(Relation):
    
    def __init__(self, entity, agent, identifier=None, attributes=None, account=None):
        Relation.__init__(self, identifier, attributes, account)
#        self.prov_type = PROV_REC_GENERATION
        self.entity=entity
        self.agent=agent
        self._attributelist.extend([self.entity,self.agent])
        
    def get_record_attributes(self):
        record_attributes = {}
        record_attributes['entity'] = self.entity
        record_attributes['agent'] = self.agent
        return record_attributes
    
    def to_provJSON(self,nsdict):
        Relation.to_provJSON(self,nsdict)
        self._json[self._idJSON]['prov:entity']=self.entity._idJSON
        self._json[self._idJSON]['prov:agent']=self.agent._idJSON
        self._provcontainer['wasAttributedTo']=self._json
        return self._provcontainer
    
    def _toRDF(self):
        Relation._toRDF(self)
        self.rdftriples[self.identifier][prov['entity']]=self.entity.identifier
        self.rdftriples[self.identifier][prov['agent']]=self.agent.identifier
        self.rdftriples[self.identifier][rdf['type']]=prov['wasAttributedTo']
        return self.rdftriples



# Activity to Entity relation

class Used(Relation):
    
    def __init__(self,activity,entity,identifier=None,time=None,attributes=None,account=None):
        Relation.__init__(self,identifier,attributes,account)
        self.prov_type = PROV_REC_USAGE
        self.entity=entity
        self.activity=activity
        self.time = time
        self._attributelist.extend([self.entity,self.activity,self.time])
        
    def get_record_attributes(self):
        record_attributes = {}
        record_attributes['entity'] = self.entity
        record_attributes['activity'] = self.activity
        if self.time is not None:
            record_attributes['time'] = self.time
        return record_attributes

        
    def to_provJSON(self,nsdict):
        Relation.to_provJSON(self,nsdict)
        self._json[self._idJSON]['prov:entity']=self.entity._idJSON
        self._json[self._idJSON]['prov:activity']=self.activity._idJSON
        if self.time is not None:
            self._json[self._idJSON]['prov:time']=self._convert_value_JSON(self.time,nsdict)
        self._provcontainer['used']=self._json
        return self._provcontainer
    
    def _toRDF(self):
        Relation._toRDF(self)
        self.rdftriples[self.identifier][prov['activity']]=self.activity.identifier
        self.rdftriples[self.identifier][prov['entity']]=self.entity.identifier
        if self.time is not None:
            self.rdftriples[self.identifier][prov['time']]=self.time
        self.rdftriples[self.identifier][rdf['type']]=prov['Used']
        return self.rdftriples
    

class wasStartedBy(Relation):
    
    def __init__(self,activity,entity,identifier=None,attributes=None,account=None):
        Relation.__init__(self,identifier,attributes,account)
        self.prov_type = PROV_REC_START
        self.activity=activity
        self.entity=entity
        self._attributelist.extend([self.entity,self.activity])
        
    def get_record_attributes(self):
        record_attributes = {}
        record_attributes['entity'] = self.entity
        record_attributes['activity'] = self.activity
        return record_attributes

    def to_provJSON(self,nsdict):
        Relation.to_provJSON(self,nsdict)
        self._json[self._idJSON]['prov:activity']=self.activity._idJSON
        self._json[self._idJSON]['prov:entity']=self.entity._idJSON
        self._provcontainer['wasStartedBy']=self._json
        return self._provcontainer
    
    def _toRDF(self):
        Relation._toRDF(self)
        self.rdftriples[self.identifier][prov['activity']]=self.activity.identifier
        self.rdftriples[self.identifier][prov['entity']]=self.entity.identifier
        self.rdftriples[self.identifier][rdf['type']]=prov['wasStartedBy']
        return self.rdftriples
    

class wasEndedBy(Relation):
    
    def __init__(self,activity,entity,identifier=None,attributes=None,account=None):
        Relation.__init__(self,identifier,attributes,account)
        self.prov_type = PROV_REC_END
        self.activity=activity
        self.entity=entity
        self._attributelist.extend([self.entity,self.activity])
        
    def get_record_attributes(self):
        record_attributes = {}
        record_attributes['entity'] = self.entity
        record_attributes['activity'] = self.activity
        return record_attributes
        
    def to_provJSON(self,nsdict):
        Relation.to_provJSON(self,nsdict)
        self._json[self._idJSON]['prov:activity']=self.activity._idJSON
        self._json[self._idJSON]['prov:entity']=self.entity._idJSON
        self._provcontainer['wasEndedBy']=self._json
        return self._provcontainer
    
    def _toRDF(self):
        Relation._toRDF(self)
        self.rdftriples[self.identifier][prov['activity']]=self.activity.identifier
        self.rdftriples[self.identifier][prov['entity']]=self.entity.identifier
        self.rdftriples[self.identifier][rdf['type']]=prov['wasEndedBy']
        return self.rdftriples
        

# Activity to Activity relations

class wasStartedByActivity(Relation):
    
    def __init__(self, started, starter, identifier=None, attributes=None, account=None):
        Relation.__init__(self, identifier, attributes, account)
#        self.prov_type = PROV_REC_GENERATION
        self.started=started
        self.starter=starter
        self._attributelist.extend([self.started,self.starter])
        
    def get_record_attributes(self):
        record_attributes = {}
        record_attributes['started'] = self.started
        record_attributes['starter'] = self.starter
        return record_attributes
    
    def to_provJSON(self,nsdict):
        Relation.to_provJSON(self,nsdict)
        self._json[self._idJSON]['prov:started']=self.started._idJSON
        self._json[self._idJSON]['prov:starter']=self.starter._idJSON
        self._provcontainer['wasStartedByActivity']=self._json
        return self._provcontainer
    
    def _toRDF(self):
        Relation._toRDF(self)
        self.rdftriples[self.identifier][prov['started']]=self.started.identifier
        self.rdftriples[self.identifier][prov['starter']]=self.starter.identifier
        self.rdftriples[self.identifier][rdf['type']]=prov['wasStartedByActivity']
        return self.rdftriples
    

class wasInformedBy(Relation):
    
    def __init__(self, informed, informant, identifier=None, attributes=None, account=None):
        Relation.__init__(self, identifier, attributes, account)
#        self.prov_type = PROV_REC_GENERATION
        self.informed=informed
        self.informant=informant
        self._attributelist.extend([self.informed,self.informant])
        
    def get_record_attributes(self):
        record_attributes = {}
        record_attributes['informed'] = self.informed
        record_attributes['informant'] = self.informant
        return record_attributes
    
    def to_provJSON(self,nsdict):
        Relation.to_provJSON(self,nsdict)
        self._json[self._idJSON]['prov:informed']=self.informed._idJSON
        self._json[self._idJSON]['prov:informant']=self.informant._idJSON
        self._provcontainer['wasInformedBy']=self._json
        return self._provcontainer
    
    def _toRDF(self):
        Relation._toRDF(self)
        self.rdftriples[self.identifier][prov['informed']]=self.informed.identifier
        self.rdftriples[self.identifier][prov['informant']]=self.informant.identifier
        self.rdftriples[self.identifier][rdf['type']]=prov['wasInformedBy']
        return self.rdftriples

# Activity to Agent relation

class wasAssociatedWith(Relation):
    
    def __init__(self, activity, agent, identifier=None, attributes=None, account=None):
        Relation.__init__(self,identifier,attributes,account)
        self.prov_type = PROV_REC_ACTIVITY_ASSOCIATION
        self.activity=activity
        self.agent=agent
        self._attributelist.extend([self.agent,self.activity])
        
    def get_record_attributes(self):
        record_attributes = {}
        record_attributes['agent'] = self.agent
        record_attributes['activity'] = self.activity
        return record_attributes

    def to_provJSON(self,nsdict):
        Relation.to_provJSON(self,nsdict)
        self._json[self._idJSON]['prov:activity']=self.activity._idJSON
        self._json[self._idJSON]['prov:agent']=self.agent._idJSON
        self._provcontainer['wasAssociatedWith']=self._json
        return self._provcontainer
    
    def _toRDF(self):
        Relation._toRDF(self)
        self.rdftriples[self.identifier][prov['activity']]=self.activity.identifier
        self.rdftriples[self.identifier][prov['agent']]=self.agent.identifier
        self.rdftriples[self.identifier][rdf['type']]=prov['wasAssociatedWith']
        return self.rdftriples

#Agent to Agent relation

class actedOnBehalfOf(Relation):
    
    def __init__(self, subordinate, responsible, identifier=None, attributes=None, account=None):
        Relation.__init__(self, identifier, attributes, account)
        self.prov_type = PROV_REC_RESPONSIBILITY
        self.subordinate = subordinate
        self.responsible = responsible
        self._attributelist.extend([self.subordinate,self.responsible])
        
    def get_record_attributes(self):
        record_attributes = {}
        record_attributes['subordinate'] = self.subordinate
        record_attributes['responsible'] = self.responsible
        return record_attributes

    def to_provJSON(self,nsdict):
        Relation.to_provJSON(self,nsdict)
        self._json[self._idJSON]['prov:subordinate']=self.subordinate._idJSON
        self._json[self._idJSON]['prov:responsible']=self.responsible._idJSON
        self._provcontainer['actedOnBehalfOf']=self._json
        return self._provcontainer
    
    def _toRDF(self):
        Relation._toRDF(self)
        self.rdftriples[self.identifier][prov['subordinate']]=self.subordinate.identifier
        self.rdftriples[self.identifier][prov['responsible']]=self.responsible.identifier
        self.rdftriples[self.identifier][rdf['type']]=prov['actedOnBehalfOf']
        return self.rdftriples

# Records to Note relation

class hasAnnotation(Relation):
    
    def __init__(self, record, note, identifier=None, attributes=None, account=None):
        Relation.__init__(self, identifier, attributes, account)
        self.prov_type = PROV_REC_ANNOTATION
        self.record=record
        self.note=note
        self._attributelist.extend([self.record, self.note])

    def to_provJSON(self,nsdict):
        Relation.to_provJSON(self,nsdict)
        self._json[self._idJSON]['prov:record']=self.record._idJSON
        self._json[self._idJSON]['prov:note']=self.note._idJSON
        self._provcontainer['hasAnnotation']=self._json
        return self._provcontainer

    def _toRDF(self):
        Relation._toRDF(self)
        self.rdftriples[self.identifier][prov['record']]=self.record.identifier
        self.rdftriples[self.identifier][prov['note']]=self.note.identifier
        self.rdftriples[self.identifier][rdf['type']]=prov['hasAnnotation']
        return self.rdftriples

'''

class tracedTo(Relation):
    
    def __init__(self, entity, ancestor, identifier=None, attributes=None, account=None):
        Relation.__init__(self, identifier, attributes, account)
#        self.prov_type = PROV_REC_GENERATION
        self.entity=entity
        self.ancestor=ancestor
        self._attributelist.extend([self.entity,self.ancestor])
        
    def get_record_attributes(self):
        record_attributes = {}
        record_attributes['entity'] = self.entity
        record_attributes['ancestor'] = self.ancestor
        return record_attributes
    
    def to_provJSON(self,nsdict):
        Relation.to_provJSON(self,nsdict)
        self._json[self._idJSON]['prov:entity']=self.entity._idJSON
        self._json[self._idJSON]['prov:ancestor']=self.ancestor._idJSON
        self._provcontainer['tracedTo']=self._json
        return self._provcontainer


    
class wasSummaryOf(Relation):
    
    def __init__(self, summarizedEntity, fullEntity, identifier=None, attributes=None, account=None):
        Relation.__init__(self, identifier, attributes, account)
#        self.prov_type = PROV_REC_GENERATION
        self.summarizedEntity=summarizedEntity
        self.fullEntity=fullEntity
        self._attributelist.extend([self.summarizedEntity,self.agent])
        
    def get_record_attributes(self):
        record_attributes = {}
        record_attributes['summarizedEntity'] = self.summarizedEntity
        record_attributes['fullEntity'] = self.fullEntity
        return record_attributes
    
    def to_provJSON(self,nsdict):
        Relation.to_provJSON(self,nsdict)
        self._json[self._idJSON]['prov:summarizedEntity']=self.summarizedEntity._idJSON
        self._json[self._idJSON]['prov:fullEntity']=self.fullEntity._idJSON
        self._provcontainer['wasSummaryOf']=self._json
        return self._provcontainer
    
'''
