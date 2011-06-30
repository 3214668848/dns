#include<stdio.h>
#include"hash.c"
int main(){
	init(20);
	put("12",3);
	put("14",5);
	put("9",6);
	printf("%d\n%d\nhere\n",get("11"),get("14"));
	removebykey("14");
	printf("%d\n",get("14"));
	return 0;
}
