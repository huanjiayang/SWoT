BEGIN{
 init=0; 
 start_time=0;
 
 j=0;
 total_size=0;
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
  if(init==0)
  {
   start_time=time;
   init=1;
  }
 }

 if(event=="r" && trace_type=="AGT"  && pkt_type=="cbr")
 {
  end_time[i]=time;
  total_pkt_size[i+1]=total_pkt_size[i]+pkt_size; 
  i++;
 }
}

 END{
 for(j = 1 ; j < i ; ++j){
	throughput=(float)total_pkt_size[j]/(end_time[j]-start_time)*8/1000;
	
}
printf("%.6f\t%.5f\n",end_time[j],throughput);
}
