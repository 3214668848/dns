#include<stdio.h>
#include<string.h>
int size;
int* table;
int* value_table;
int BKDRHash(char *str){
	int seed=29;
	int hash=0;
	while (*str)
		hash=hash*seed+(*str++); 
	return (hash & 0x7FFFFFFF);
}
void init(int n){
	size=3*n/2;
	table=(int *)malloc(size*sizeof(int));
	value_table=(int *)malloc(size*sizeof(int));
	memset(table,-1,size*sizeof(int));
}
void put(char* host,int value){
	int key=BKDRHash(host);
	int h=key%size;
	while(table[h]!=-1&&table[h]!=key)
		h=(h+1)%size;
	table[h]=key;
	value_table[h]=value;		
}
int get(char* host){
	int key=BKDRHash(host);
	int h=key%size;
	while(table[h]!=-1&&table[h]!=key)
		h=(h+1)%size;
	if(table[h]==-1)
		return -1;
	return value_table[h];
}
int removebykey(char* host){
	int key=BKDRHash(host);
	int h=key%size;
	while(table[h]!=-1&&table[h]!=key)
		h=(h+1)%size;
	if(table[h]==-1)
		return 0;
	table[h]=-1;
	return 1;
}
