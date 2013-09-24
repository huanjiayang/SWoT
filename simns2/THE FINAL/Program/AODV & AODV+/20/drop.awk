BEGIN {
   
 num_drop=0;
  num_send=0;
}
{
 event=$1;   #�¼�����
 time=$2;   #�¼�������ʱ��
 node_id=$3;   #�����¼��Ľڵ��ID
 trace_type=$4;  #Trace����
 pkt_id=$6;   #�����id
 pkt_type=$7;   #���������
 pkt_size=$8;   #����Ĵ�С(�ֽ�) 



 if(event=="s" && trace_type=="AGT"  && pkt_type=="cbr")
 {
  num_send++;
   timeArr[i]=time; 
   lossrate[i]=(float)(num_drop/num_send); 
   i++; 
  }
 
 if(event=="D")
  num_drop++;
}
END{
  printf("#number of packet sent:%d,lost:%d\n",num_send,num_drop); 
  printf("#lost rate of packets:%f\n",num_drop/num_send); 
  for(j=0;j<i;j++) 
    printf("%f\t%f\n",timeArr[j],lossrate[j]); 
}
