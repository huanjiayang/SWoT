BEGIN {
   
 num_drop=0;
  num_send=0;
}
{
 event=$1;   #事件类型
 time=$2;   #事件产生的时间
 node_id=$3;   #处理事件的节点的ID
 trace_type=$4;  #Trace类型
 pkt_id=$6;   #分组的id
 pkt_type=$7;   #分组的类型
 pkt_size=$8;   #分组的大小(字节) 



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
