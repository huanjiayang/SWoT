import os,sys,MySQLdb

class FETCH:
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
        
            net_roll = value[1]
            net = net_roll.split(' ')
            n_id = net[0]
            n_roll = net[1]
        
            transducer = value[2]
            trans= transducer.split(' ')
            t_id = trans[0]
            t_type = trans[1]
        
            sensor_net = value[3]
            sen_net = sensor_net.split(' ')
            sn_id = sen_net[0]
            sn_type = sen_net[1]
        
            Data = value[4]
            item = Data.split(' ')
            temp = item[0]
            time = item[1]
            info = item[2]
        #print value[0]
        f.close() 
        
try:
    db_con = MySQLdb.connect(host='localhost',user='root',passwd='o5kFXf98',
                             port=3306)
    pointer =db_con.cursor()
    pointer.execute('show databases')
    for databases in pointer.fetchall():
        databases[0]      
    if 'pygateway' in databases[0]: 
        print "Database 'pygateway' has already exists!"
    else:     
        pointer.execute('create database pygateway')    
    pointer.execute('USE pygateway')
    #print "success"
    pointer.execute('show tables')
    for tables in pointer.fetchall():
        tables[0]
    if 'value' in tables[0]:
        print "Table 'value' has already exists!"
    else:
        pointer.execute('create table value(SNID char(10), SNTY char(10),NETIP char(15), ROLL char(10),TRID char(10),MAC char(17),SEID char(10),SETY char(10),TEMP char(10),TIME char(10),INF char(10))')            

    pointer.execute('insert into value values(%s,%s,%s,%s)',
                    (FETCH.s_id,FETCH.s_type,FETCH.n_id,FETCH.n_roll))
    print "Insert data successfully!"
    db_con.commit()
    pointer.close()
    db_con.close()
    
except MySQLdb.Error,e:
     print "Mysql Error %d: %s" % (e.args[0], e.args[1])