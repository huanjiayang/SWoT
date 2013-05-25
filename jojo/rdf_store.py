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

result_uuid = str(uuid.uuid1())
store.add((ns_ebook[result_uuid],RDF.type,ns_ebook['result']))
store.add((ns_ebook[result_uuid],ns_ebook['source'],Literal['output.txt']))
store.add((ns_ebook[result_uuid],ns_ebook['value'],Literal[result]))

calculate_uuid = str(uuid.uuid1())
store.add((ns_ebook[calculate_uuid],RDF.type,ns_ebook['calculate']))
store.add((ns_ebook[calculate_uuid],ns_prov['starttime'],Literal[t1]))
store.add((ns_ebook[calculate_uuid],ns_prov['endtime'],Literal[t2]))

ebook_execute = '%s/%s/execution.py' % (ebooks_dir, ebookname)
store.add((ebook_execute,RDF.type,ns_ebook['input']))

software_uuid = str(uuid.uuid1())
store.add((software_id,RDF.type,ns_ebook['software']))

store.add((ns_ebook[result_uuid],ns_prov['wasDerivedFrom'],ebook_file_URI))
store.add((ns_ebook[result_uuid],ns_prov['wasGeneratedBy'],ns_ebook[calculate_uuid]))
store.add((ns_ebook[result_uuid],ns_prov['wasDerivedFrom'],ebook_execute))
store.add((ns_ebook[result_uuid],ns_prov['wasStartedBy'],ns_ebook[software_uuid]))
#store.add((server,ns_prov['wasAttributedTo'],software))
#store.add((software,ns_prov['wasStartedBy'],calculate))

store.serialize(destination='history.n3',format='n3')



