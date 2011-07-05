#include<stdio.h>
#include"trie.c"
#include"mydns.c"
int main(){
	init(20);
	printf("here1\n");
	put("12",3);
	printf("here2\n");
	put("14",5);
	printf("here3\n");
	put("9",6);
	printf("%d %d %d\nhere\n",get("11"),get("14"),get("12"));
	removebykey("14");
	printf("%d\n",get("14"));
	removebykey("12");
	removebykey("12");
	printf("%d\n",get("12"));
	printf("%d\n",get("9"));
	return 0;
}
