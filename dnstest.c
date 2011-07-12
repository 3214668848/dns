#include"dns.h"
#include"mydns.c"
#include"dnsquery.c"
int main(){
    struct sockaddr_in dns_serv;
    dns_serv.sin_family=AF_INET;
    dns_serv.sin_port=htons(53);
    //dns_serv.sin_addr.s_addr=inet_addr("211.87.232.85");
	dns_serv.sin_addr.s_addr=inet_addr("211.87.232.98");
    int sk=socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(53);
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
  //  if(bind(sk,(struct sockaddr *)&addr,sizeof(addr))<0){
 //       printf("bind failed...\n");
  //      return;
//    }
	//printf("bind success\n");
	struct udp_packet *p=(struct udp_packet *)malloc(sizeof(struct udp_packet));
	//dns_query_package(p,"www.baidu.com\0",0);
	int ret=0,i=0;
	freopen("host_list_10000","r",stdin);
	char c[100];
	for(;i<10000;++i){
		memset(c,0,sizeof(c));
		gets(c);
		dns_query_package(p,c,0);
	int j=0;
	for(;j<10;++j)
   		ret=sendto(sk,&p->dns_hdr,p->len,0,(struct sockaddr*)&dns_serv,sizeof(addr));
		printf("i=%d send %d  -> %s\n",i,ret,c);
	}
	memset(p,0,sizeof(struct udp_packet));
	int size=sizeof(addr);
	int rret=0;
	for(i=0;;++i){
//		rret=recvfrom(sk,&p->dns_hdr,sizeof(struct udp_packet),0,(struct sockaddr*)&addr,&size);
		rret=recvfrom(sk,&p->dns_hdr,sizeof(struct udp_packet),0,(struct sockaddr*)&addr,&size);
		printf("%d\n",i);
		char *a1,*a2;
//		printf("%s %d\n",inet_ntoa(p->src_ip),p->src_port);
		decode(p,a1,a2);
	}
	return 0;
}
