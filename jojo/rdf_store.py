import logging
import uuid

# Optional: Configure how we want rdflib logger to log messages
_logger = logging.getLogger("rdflib")
_logger.setLevel(logging.DEBUG)
_hdlr = logging.StreamHandler()
_hdlr.setFormatter(logging.Formatter('%(name)s %(levelname)s: %(message)s'))
_logger.addHandler(_hdlr)

from rdflib import Graph, Literal, BNode, RDF
from rdflib.namespace import FOAF, DC

pns_prov = PROVNamespace("prov","http://www.w3.org/ns/prov-dm/")

ns_prov = Namespace("http://www.w3.org/ns/prov-dm/")
ns_ebook = Namespace("http://jiaojiaojiang.com/ebook/")

store = Graph()

# Bind a few prefix, namespace pairs for pretty output
store.bind("dc", DC)
store.bind("foaf", FOAF)
store.bind("ns_prov",ns_prov)
store.bind("ebook",ns_ebook)


#store.add((userinput,ns_prov:['wasAssociatedWith'],user))
#store.add((input,ns_prov:['wasGeneratedBy'],userinput))
#store.add((result,ns_prov:['wasDerivedFrom'],input))

resultid = "urn:uuid:" + uuid.uuid1()
store.add((resultid,RDF.type,ns_ebook["result"]))
store.add((ns_ebook["result01"],ns_ebook["source"],Literal['output.txt']))


store.add((ns_ebook["result"],ns_prov['wasDerivedFrom'],books))

store.add((ns_ebook["cal01"],RDF.type,ns_ebook["calculate"]))
store.add((ns_ebook["cal01"],ns_prov["starttime"],Literal[t]))

store.add((ns_ebook["result01"],ns_prov['wasGeneratedBy'],ns_ebook["cal01"]))


store.add((server,ns_prov['wasAttributedTo'],software))
store.add((software,ns_prov['wasStartedBy'],calculate))

store.serialize(destination='',format='n3')

# Iterate over triples in store and print them out.
print "--- printing raw triples ---"
for s, p, o in store:
    print s, p, o

# For each foaf:Person in the store print out its mbox property.
print "--- printing mboxes ---"
for person in store.subjects(RDF.type, FOAF["Person"]):
    for mbox in store.objects(person, FOAF["mbox"]):
        print mbox

# Serialize the store as RDF/XML to the file foaf.rdf.
store.serialize("foaf.rdf", format="pretty-xml", max_depth=3)

# Let's show off the serializers

print "RDF Serializations:"

# Serialize as XML
print "--- start: rdf-xml ---"
print store.serialize(format="pretty-xml")
print "--- end: rdf-xml ---\n"

# Serialize as Turtle
print "--- start: turtle ---"
print store.serialize(format="turtle")
print "--- end: turtle ---\n"

# Serialize as NTriples
print "--- start: ntriples ---"
print store.serialize(format="nt")
print "--- end: ntriples ---\n"