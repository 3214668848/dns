#include "dns.h"
void sig_child(int signo);
int udp_sock_open(struct in_addr ip, int port);
int udp_packet_read(int sockfd, struct udp_packet *udp_pkt);
int dns_is_query(struct udp_packet *udp_pkt);
int dns_get_opcode(struct udp_packet *udp_pkt);
void dns_send_reply(int sockfd, struct udp_packet *pkt,char *query);
void decode_domain_name(char name[BUF_SIZE]);
int main(int argc, char **argv)
{
    int sockfd;
    struct in_addr ip;
    char query_string[BUF_SIZE];
    int numread;
    struct udp_packet pkt;
    ip.s_addr = INADDR_ANY;
    sockfd = udp_sock_open(ip,PORT);
    signal(SIGCHLD,sig_child);
	//signal(SIGCHLD, SIG_IGN);
    while(1)
    {
		printf("wait read here\n");
        numread = udp_packet_read(sockfd,&pkt);
		printf("read finish\n");
        if(numread < 0)
        {
            printf("no data ...\n");
            continue;
        }
        if(numread < sizeof(struct dns_header)+1)
        {
            printf("got packet with invalid size of %d \n",numread);
            continue;
        }
        printf("Dns query from %s port %d\n", inet_ntoa(pkt.src_ip), pkt.src_port);
        if(fork() == 0)break;
    }
    strcpy(query_string, pkt.buf);
    decode_domain_name(query_string);
    printf("query: %s\n", query_string);
    if (dns_is_query(&pkt))
    {
        switch (dns_get_opcode(&pkt))
        {
			case 0:
				dns_send_reply( sockfd, &pkt, query_string);
            	break;
        	default:
            	printf("unsupported opcode: %d\n", dns_get_opcode(&pkt));
            	break;
        }
    }
    printf("Finish.\n\n");
    return 0;
}
int udp_sock_open(struct in_addr ip, int port)
{
    int fd;
    struct sockaddr_in sa;
    memset((void *)&sa, 0, sizeof(sa));//clear it out
    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(fd < 0)
    {
        printf("Could not create socket");
        exit(1);
    }
    sa.sin_family = AF_INET;
    memcpy((void *)&sa.sin_addr, (void *)&ip, sizeof(struct in_addr));
    sa.sin_port = htons(port);
    if (bind(fd, (struct sockaddr *)&sa, sizeof(struct sockaddr)) < 0)
    {
        printf("Could not bind to port");
        exit(1);
    }
    printf("open finish\n");
    return(fd);
}
int udp_packet_write(int sockfd, struct udp_packet *udp_pkt)
{
    struct sockaddr_in sa;
    memset((void *)&sa, 0, sizeof(sa));//clear it out
    sa.sin_addr.s_addr = udp_pkt->src_ip.s_addr;
    sa.sin_port = htons(udp_pkt->src_port);
    sa.sin_family = AF_INET;
    return(sendto(sockfd, &udp_pkt->dns_hdr, udp_pkt->len, 0,
                  (struct sockaddr *)&sa, sizeof(sa)));
}
int udp_packet_read(int sockfd, struct udp_packet *udp_pkt)
{
    int numread;
    struct sockaddr_in sa;
    int salen;
    salen = sizeof(sa);
    if ((numread = recvfrom(sockfd, &udp_pkt->dns_hdr, sizeof(udp_pkt->buf),0,
                            (struct sockaddr *)&sa, &salen)) < 0)
    {
        printf("udp_read_read: recvfrom error\n");
        return -1;
    }
    memcpy((void *)&udp_pkt->src_ip, (void *)&sa.sin_addr, sizeof(struct in_addr));
    udp_pkt->src_port = ntohs(sa.sin_port);
    udp_pkt->len = numread;
    return numread;
}
int dns_is_query(struct udp_packet *udp_pkt)
{
//    return (!((udp_pkt->buf[ strlen(udp_pkt->buf) +2 ] & 0x80)/0x80));
 	return (!((udp_pkt->dns_hdr.dns_flags & 0x80)/0x80));
}
int dns_get_opcode(struct udp_packet *udp_pkt)
{
//    return (((udp_pkt->buf[ strlen(udp_pkt->buf) +2 ]) & 0x78)/8);
	 return ((udp_pkt->dns_hdr.dns_flags & 0x78)/8);
}
int dns_set_flags(int err, struct udp_packet *udp_pkt)
{
    /* Keep opcode and rd flags set all others to 0 */
    udp_pkt->dns_hdr.dns_flags=udp_pkt->dns_hdr.dns_flags & 0x7900;
    /* Set QR to response and RA to recursion available */
    udp_pkt->dns_hdr.dns_flags=udp_pkt->dns_hdr.dns_flags | 0x8080;
    if (err)
    {
        udp_pkt->dns_hdr.dns_flags=udp_pkt->dns_hdr.dns_flags | 0x0005;//Set RCODE to refused. 0x0002=Server failure
		printf("server refused query\n");
    }
    return 0;
}
void packet_append(struct udp_packet *pkt, void *data, int data_len)
{
    memcpy((void*)&pkt->dns_hdr + pkt->len, data, data_len);
    pkt->len += data_len;
}
void dns_send_reply(int sockfd, struct udp_packet *pkt, char *query)
{
    struct dns_answer dns_ans;
    struct in_addr ip;
    struct hostent *host;
    char result[BUF_SIZE];
    host = gethostbyname(query);
    if(!host)
    {
        printf("host not found\n");
        dns_set_flags(1, pkt);//failure
        udp_packet_write(sockfd, pkt);
        return;
    }
    else
    {
        memcpy(&ip.s_addr, host->h_addr, host->h_length);
        printf("host:%s=> got ip: %s\n",query,inet_ntoa(ip));
    }
    dns_set_flags(0, pkt); 
    pkt->dns_hdr.dns_no_answers  = htons(1);
    dns_ans.dns_name = htons(0xc00c);
    dns_ans.dns_type = htons(1);   //Host name
    dns_ans.dns_class = htons(1);  //inet
    dns_ans.dns_time_to_live = htons(60);
    dns_ans.dns_data_len = htons(4);
    packet_append(pkt, &dns_ans, sizeof(dns_ans));
    packet_append(pkt, &ip, sizeof(ip));
    udp_packet_write(sockfd, pkt);

}
void sig_child(int signo)
{
    while(wait3(NULL, WNOHANG, 0) > 0);
}
void decode_domain_name(char query[BUF_SIZE])//\03www\05linux\03com\0=>www.linux.com
{
    char temp[BUF_SIZE];
    int i, k, len, j;

    i = 0;
    k = 0;
    while(query[i])
    {
        len = query[i];
        i++;
        for(j = 0; j<len ; j++)
            temp[k++] = query[i++];
        temp[k++] = '.';
    }
    temp[k-1] = 0;
    strcpy(query, temp);
}
