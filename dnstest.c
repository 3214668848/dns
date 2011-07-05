#include"dns.h"
#include"mydns.c"
#include"dnsquery.c"
int main(){
    struct sockaddr_in dns_serv;
    dns_serv.sin_family=AF_INET;
    dns_serv.sin_port=htons(53);
    //dns_serv.sin_addr.s_addr=inet_addr("211.87.226.11");
	dns_serv.sin_addr.s_addr=inet_addr("8.8.8.8");
    int sk=socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(53);
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    if(bind(sk,(struct sockaddr *)&addr,sizeof(addr))<0){
        printf("bind failed...\n");
        return;
    }
	printf("bind success\n");
	struct udp_packet *p=(struct udp_packet *)malloc(sizeof(struct udp_packet));
	dns_query_package(p,"google.com\0",0);
   	int ret=sendto(sk,&p->dns_hdr,p->len,0,(struct sockaddr*)&dns_serv,sizeof(addr));
	printf("send %d\n",ret);
	int size=sizeof(addr);
	int recv_len=recvfrom(sk,&p->dns_hdr,sizeof(struct udp_packet),0,(struct sockaddr*)&addr,&size);
	char *a1,*a2;
	decode(p,a1,a2);
	return 0;
}