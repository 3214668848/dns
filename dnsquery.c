#include"dns.h"
void encode_domain_name(char *name,char encoded_name[BUF_SIZE]){
  int i,j,k,n;
  k=0;i=0;
  while(name[i]){
	 for(j=0;name[i+j]&&name[i+j]!='.';j++);
	 encoded_name[k++]=j;
	 for(n=0;n<j;n++)
		encoded_name[k++]=name[i+n];
	 i+=j+1;
	 if(!name[i-1])break;
  }
  encoded_name[k++]=0;
}
void dns_query_package(struct udp_packet *udp_pkt,char* host,short int id){
	if(!id){
		srand(time(NULL));
		id=rand()%32766+1;
	}
	memset(udp_pkt,0,sizeof(struct udp_packet));
	printf("%s\n",host);
	char name[BUF_SIZE];
	encode_domain_name(host,name);
	printf("%d %s\n",strlen(name),name);
	struct dns_header head;
	head.dns_id=htons(id);
	head.dns_flags=htons(0x0100);
	head.dns_no_questions=htons(1);
 	head.dns_no_answers=0;
  	head.dns_no_authority=0;
	head.dns_no_additional=0;
	memcpy((void*)&udp_pkt->dns_hdr,(void*)&head,sizeof(struct dns_header));
	memcpy((void*)&udp_pkt->dns_hdr+sizeof(struct dns_header),(void*)&name,strlen(name));
	struct tmp{
		short int type;
		short int class;
	}temp;
	temp.type=htons(1);
	temp.class=htons(1);
	memcpy((void*)&udp_pkt->dns_hdr+(sizeof(struct dns_header)+strlen(name)),(void*)&temp,4);
	int tt=(sizeof(struct dns_header)+strlen(name)+4);
	printf("here\n");
	udp_pkt->len=tt;
	udp_pkt->src_port=PORT;
	printf("end\n");
	//inet_aton("1.1.1.1",&udp_pkt->src_ip);
}
