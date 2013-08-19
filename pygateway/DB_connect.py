import os,sys,MySQLdb
split_Dataline = []
if os.path.isfile('DataContainer.txt')== True :
    f = open("DataContainer.txt")
    Dataline = f.readlines()
    for x in xrange (0,len(Dataline)):
        split_Dataline = Dataline[x]
        value = split_Dataline.split(',')
        sen_nod = value[0]
        node = sen_nod.split(' ')
        s_id = node[0]
        s_type = node[1]
        print s_id
        
        net_roll = value[1]
        net = net_roll.split(' ')
        n_id = net[0]
        n_roll = net[1]
        print n_id
        
        transducer = value[2]
        trans= transducer.split(' ')
        t_id = trans[0]
        t_type = trans[1]
        print t_id
        
        sensor_net = value[3]
        sen_net = sensor_net.split(' ')
        sn_id = sen_net[0]
        sn_type = sen_net[1]
        print sn_id
        
        Data = value[4]
        item = Data.split(' ')
        temp = item[0]
        time = item[1]
        info = item[2]
    #print value[0]
    f.close() 
try:
    db_con = MySQLdb.connect(host='localhost',user='root',passwd='o5kFXf98',port=3306)
    pointer =db_con.cursor()
    databases = pointer.execute('show databases')
    print databases
    pointer.execute('create database if not exists pygateway')
    pointer.execute('USE pygateway')
    print "success"
    #if not exists(pointer.execute('select * fo')):
    #if exists (select * from pygateway.value where id = object_id(N'[pygateway].[value]') and OBJECTPROPERTY(id, N'IsView') = 1)
    pointer.execute('create table value(SNID char(10), SNTY char(10), NETIP char(15), NETROLL char(10))')
    pointer.execute('insert into value values(%s,%s)',(s_id,s_type))
    print "success"
    db_con.commit()
    pointer.close()
    db_con.close()
    
except MySQLdb.Error,e:
     print "Mysql Error %d: %s" % (e.args[0], e.args[1])