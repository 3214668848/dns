#include<stdio.h>
#include"hash.c"
#include"mydns.c"
#include"dnsquery.c"
int main(){
	hash_init(20);
	printf("here1\n");
	hash_put("12",3);
	printf("here2\n");
	hash_put("14",5);
	printf("here3\n");
	hash_put("9",6);
	printf("%d %d %d\nhere\n",hash_get("11"),hash_get("14"),hash_get("12"));
	hash_removebykey("14");
	printf("%d\n",hash_get("14"));
	hash_removebykey("12");
	hash_removebykey("12");
	printf("%d\n",hash_get("12"));
	printf("%d\n",hash_get("9"));
	hash_put("9",57);
	hash_put("9",100);
	printf("%d\n",hash_get("9"));
	hash_removebykey("9");
	printf("%d\n",hash_get("9"));
	return 0;
}
