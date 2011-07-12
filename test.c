#include "client.c"
int main(){
	freopen("host_list_10000","r",stdin);
	char ip[16]="\0";
	char *host="baidu.com\0";
	getip(host,ip,0);
	printf("ip=%s\n",ip);
	freopen("host_list_10000","r",stdin);
	char c[100];
	int i=0;
	for(;i<100;++i){
		memset(c,0,sizeof(c));
		gets(c);
		memset(ip,0,sizeof(ip));
		getip(c,ip,0);
		printf("i=%d  ip=%s\n",i+1,ip);
	}
	return 0;
}
