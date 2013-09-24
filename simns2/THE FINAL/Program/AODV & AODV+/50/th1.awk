BEGIN{
 init=0; 
 start_time=0;
 
 j=0;
 total_size=0;
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
