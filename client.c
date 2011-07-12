#include"dns.h"
#include"mydns.c"
#include"dnsquery.c"
int recvfromTimeOut(int socket, long sec, long usec) {  
    struct timeval timeout;  
    timeout.tv_sec = sec;  
    timeout.tv_usec = usec;   
    fd_set fds;  
    FD_ZERO(&fds);  
    FD_SET(socket, &fds);  
    // Return value:  
    // -1: error occured  
    // 0: timed out  
    // >0: data ready to be read  
    return select(FD_SETSIZE, &fds, 0, 0, &timeout);  
} 
int getip(char* host,char* ip,short int id){
	if(!id){
		srand(time(NULL));
		id=rand()%32766+1;
	}
	struct sockaddr_in dns_serv;
    dns_serv.sin_family=AF_INET;
    dns_serv.sin_port=htons(53);
	dns_serv.sin_addr.s_addr=inet_addr("8.8.8.8");
    int sk=socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(53);
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
	struct udp_packet *p=(struct udp_packet *)malloc(sizeof(struct udp_packet));
	dns_query_package(p,host,0);
	int ret=0;
   	ret=sendto(sk,&p->dns_hdr,p->len,0,(struct sockaddr*)&dns_serv,sizeof(addr));
	if(ret<0){
		printf("error sendto\n");
		close(sk);
		return 0;
	}
	memset(p,0,sizeof(struct udp_packet));
	int size=sizeof(addr);
	int rret=0;
	int recv_len = recvfromTimeOut(sk, 0, 10000000);
    if (recv_len == 0 || recv_len < 0) {  
		printf("time out\n");
        close(sk);  
        return 0;  
    }  
    else {  
       recv_len = recvfrom(sk,&p->dns_hdr,sizeof(struct udp_packet),0,(struct sockaddr*)&addr,&size);
       if (recv_len < 0) {  
           printf("error recvfrom\n");   
           close(sk);  
           return 0;  
       }  
       decode(p,host,ip);
    }  
    close(sk);  
    return 1; 
}
