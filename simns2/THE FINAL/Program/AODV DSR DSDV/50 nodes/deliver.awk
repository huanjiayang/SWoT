BEGIN{
	highest_packet_id = 0;
	sends = 0;
	receives = 0;
	routing_packets = 0;
	first_received_time = 0;
	first = 0;
}
{
	action = $1;
	time = $2;
	packet_id = $6;
	trace = $4;
	type = $7;
	if(action == "s" || action == "r" || action == "f"){
		if(action == "s" && trace == "AGT" && type == "cbr")
			sends++;
		if(packet_id > highest_packet_id){
			highest_packet_id = packet_id;
		}
		if(start_time[packet_id] == 0)
		start_time[packet_id] = time;
		if(action == "r" && trace == "AGT" && type == "cbr"){
			if(first == 0){
				first_received_time = time;
				first =1;
			}
			receives++;
			total_pkt_size[paket_id]=total_pkt_size[i]+pkt_size; 
			end_time[packet_id] = time;
		}
	}else{
			end_time[packet_id] = -1;
	}

}
END{
	printf("%f\n",(float)(receives/sends));
}
