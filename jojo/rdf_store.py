import logging
import uuid
import rdflib
from rdflib import URIRef,Literal, Namespace, Graph, ConjunctiveGraph, RDF


ns_prov = Namespace('http://www.w3.org/ns/prov-dm/')
ns_ebook = Namespace('http://jiaojiaojiang.com/ebook/')

store = Graph()

# Bind a few prefix, namespace pairs for pretty output

store.bind('ns_prov',ns_prov)
store.bind('ebook',ns_ebook)


#store.add((userinput,ns_prov:['wasAssociatedWith'],user))
#store.add((input,ns_prov:['wasGeneratedBy'],userinput))
#store.add((result,ns_prov:['wasDerivedFrom'],input))

result_id = 'urn:uuid:' + str(uuid.uuid1())
store.add((result_id,RDF.type,ns_ebook['result']))
store.add((result_id,ns_ebook['source'],Literal['output.txt']))
store.add((result_id,ns_ebook['value'],Literal[result]))

calculate_id = 'urn:uuid' + str(uuid.uuid1())
store.add((calculate_id,RDF.type,ns_ebook['calculate']))
store.add((calculate_id,ns_prov['starttime'],Literal[t1]))
store.add((calculate_id,ns_prov['endtime'],Literal[t2]))

ebook_execute = '%s/%s/execution.py' % (ebooks_dir, ebookname)
store.add((ebook_execute,RDF.type,ns_ebook['input']))

software_id = 'urn:uuid' + str(uuid.uuid1())
store.add((software_id,RDF.type,ns_ebook['software']))

store.add((result_id,ns_prov['wasDerivedFrom'],ebook_file_URI))
store.add((result_id,ns_prov['wasGeneratedBy'],calculate_id))
store.add((result_id,ns_prov['wasDerivedFrom'],ebook_execute))
store.add((calculate_id,ns_prov['wasStartedBy'],software_id))
#store.add((server,ns_prov['wasAttributedTo'],software))
#store.add((software,ns_prov['wasStartedBy'],calculate))

store.serialize(destination='history.n3',format='n3')



