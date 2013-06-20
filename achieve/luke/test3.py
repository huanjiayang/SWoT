import time, datetime 
import itertools

data = ['Node,8766,Temp,23,Humidity,65,Light,4']

data = str(data)
data = data.replace("\n","").replace("\r","")
msglist = data.split(";")

for msg in msglist:
    
        
    msg = msg.split(",")
             
    print "data: ", msg
    msg_timestamp = str(datetime.datetime.now())
    # Create a counter starting at 10
    counter = itertools.count(10)
    print msg_timestamp, counter
   # addtoStore(msg, msg_timestamp, counter)