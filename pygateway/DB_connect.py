import os,sys,MySQLdb
class Check:
    def set_up(self):       
        try:
            db_con = MySQLdb.connect(host='localhost',user='root',passwd='o5kFXf98',
                                     port=3306)
            pointer =db_con.cursor()
            pointer.execute('show databases')
            for databases in pointer.fetchall():
                databases[0]      
            if 'pygateway' in databases[0]: 
                print "Database 'pygateway' already exists!"
            else:     
                pointer.execute('create database pygateway')    
            pointer.execute('USE pygateway')
            #print "success"
            num = pointer.execute('show tables')
            #print num 
            if num ==0: 
                print 'There is no table in this database!'
                print "Start to create a new table 'value' "
                pointer.execute('create table value(SNID char(10),SNTY char(10),'
                'NETIP char(15), ROLL char(10),TRID char(10),MAC char(17),SEID char(10),'
                'SETY char(10),TEMP char(10),TIME char(10),INF char(10))') 
            else:
                num
                for tables in pointer.fetchall():
                    tables[0]
                    #print tables[0]
                if 'value' not in tables[0]:
                    print "Start to create a new table 'value' "
                    pointer.execute('create table value(SNID char(10),SNTY char(10),'
                    'NETIP char(15), ROLL char(10),TRID char(10),MAC char(17),SEID char(10),'
                    'SETY char(10),TEMP char(10),TIME char(30),INF char(10))')
                else:
                    print "Table 'value' already exists!"      
                db_con.commit()
                pointer.close()
                db_con.close()               
        except MySQLdb.Error,e:
            print "Mysql Error %d: %s" % (e.args[0], e.args[1])

    def getSensorInfo(self,seid): 
        try:
            result = {}
            attrlist= []
            valuelist =[]
            db_con = MySQLdb.connect(host='localhost',user='root',passwd='o5kFXf98',
                                     port=3306)
            pointer =db_con.cursor()   
            pointer.execute('use pygateway') 
            pointer.execute('describe value')
            for attr in pointer.fetchall():
                attrlist.append(attr[0])
                
                 
            tabnum = pointer.execute('select * from value where SEID = "' + seid + '"')       
            if tabnum != 0:
                tabnum 
                for inform in pointer.fetchall():
                    #print inform
                    for i in range(11):
                        result[attrlist[i]] = inform[i]
                        #sys.stdout.write("%s "%inform[i])
                    #sys.stdout.write("\n")
                    #sys.stdout.flush()
            
            db_con.commit() 
            pointer.close()
            db_con.close()
            return result
        except MySQLdb.Error,e:
            print "Mysql Error %d: %s" % (e.args[0], e.args[1])
             
    def readfile(self):
        try:
            result = []
            db_con = MySQLdb.connect(host='localhost',user='root',passwd='o5kFXf98',
                                     port=3306)
            pointer =db_con.cursor()    
            split_Dataline = []
            if os.path.isfile('DataContainer.txt')== True :
                f = open("DataContainer.txt")
                Dataline = f.readlines()
                for x in xrange (0,len(Dataline)):
                    split_Dataline = Dataline[x]
                    value = split_Dataline.split(',')
                    sen_net = value[0]
                    node = sen_net.split(' ')
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
                
                    sensor_nod = value[3]
                    sen_nod = sensor_nod.split(' ')
                    sn_id = sen_nod[0]
                    sn_type = sen_nod[1]
                
                    Data = value[4]
                    item = Data.split(' ')
                    temp = item[0]
                    time = item[1]
                    info = item[2]
                    pointer.execute('use pygateway')
                    sql = "select SEID from value where SEID = '"+sn_id+ "' "
                    same_id=pointer.execute(sql)
                    if same_id ==0 :
                        pointer.execute('insert into value values(%s,%s,%s,%s,%s,%s,%s,'
                                    '%s,%s,%s,%s)',(s_id,s_type,n_id,n_roll,t_id,t_type,
                                                    sn_id,sn_type,temp,time,info)) 
                        print "Insert data successfully!"           
                    else :
                        print "This ID has already exists!"                    
                print "Reading file finished!"
                pointer.execute('select SEID from value')
                for sensorid in pointer.fetchall():    
                    print sensorid[0]
                    result.append(sensorid[0])       
                f.close() 
         
            db_con.commit() 
            pointer.close()
            db_con.close()
            return result
    
        except MySQLdb.Error,e:
             print "Mysql Error %d: %s" % (e.args[0], e.args[1])