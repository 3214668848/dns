#include <stdio.h>
int main(){
	freopen("host_list_10000","r",stdin);
	char c[100];
	int i=0;
	for(;i<5000;++i){
		memset(c,0,sizeof(c));
		gets(c);
		printf("%s\n",c);
	}
	return 0;
}
