#include "dns.h"
int dns_is_query(struct udp_packet *udp_pkt){
	return (!((udp_pkt->dns_hdr.dns_flags&0x8000)/0x8000));
}
void decode_domain_name(char* query){
    char temp[BUF_SIZE];
    int i,k,len,j;
    i=0;k=0;
    while(query[i]){
        len=query[i];
        i++;
        for(j=0;j<len;j++)
            temp[k++]=query[i++];
        temp[k++]='.';
    }
    temp[k-1]=0;
    strcpy(query,temp);
}
int decode(struct udp_packet *udp_pkt,char* host,char* ip){
	int ret=dns_is_query(udp_pkt);
	char* query_string=(char*)malloc(BUF_SIZE*sizeof(char));
	strcpy(query_string,udp_pkt->buf);
    decode_domain_name(query_string);
    printf("query: %s\n",query_string);
	int l=strlen(query_string);
	host=(char*)malloc(l*sizeof(char));
	memcpy(host,query_string,l*sizeof(char));
	ip=0;
	if(!ret){
		struct in_addr myip;
		memcpy((void*)&myip,(void*)&udp_pkt->buf+(sizeof(struct dns_answer)+strlen(udp_pkt->buf)+5),sizeof(struct in_addr));
		memset(query_string,0,sizeof(query_string));
		query_string=inet_ntoa(myip);
		ip=(char*)malloc(strlen(query_string));
		memcpy(ip,query_string,strlen(query_string)*sizeof(char));
		printf("ip: %s\n",ip);
	}
	return ret;
}
void packet_append(struct udp_packet *pkt,void *data,int data_len){
    memcpy((void*)&pkt->dns_hdr+pkt->len,data,data_len);
    pkt->len+=data_len;
}
int dns_set_flags(int err,struct udp_packet *udp_pkt){
    udp_pkt->dns_hdr.dns_flags=udp_pkt->dns_hdr.dns_flags&0x7900;
    udp_pkt->dns_hdr.dns_flags=udp_pkt->dns_hdr.dns_flags|0x8080;
    if (err){
        udp_pkt->dns_hdr.dns_flags=udp_pkt->dns_hdr.dns_flags|0x0005;
		printf("server refused query\n");
    }
    return 0;
}
void code(struct udp_packet *udp_pkt,int mark,char* ip){
	struct in_addr myip;
	inet_aton(ip,&myip);
	//inet_aton("1.1.1.1",&udp_pkt->src_ip);
	if(mark){
		
	}else{
		struct dns_answer dns_ans;
		dns_set_flags(0,udp_pkt); 
    	udp_pkt->dns_hdr.dns_no_answers=htons(1);
    	dns_ans.dns_name=htons(0xc00c);
    	dns_ans.dns_type=htons(1);
    	dns_ans.dns_class=htons(1);
    	dns_ans.dns_time_to_live=htons(60);
    	dns_ans.dns_data_len=htons(4);
    	packet_append(udp_pkt,&dns_ans,sizeof(dns_ans));
    	packet_append(udp_pkt,&ip,sizeof(ip));
	}
}
