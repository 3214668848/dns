#include<stdio.h>
#include<string.h>
typedef int value_type;
int size;
int* table;
int* table2;
value_type* value_table;
int BKDRHash(char *str){
	int seed=29;
	int hash=0;
	while (*str)
		hash=hash*seed+(*str++); 
	return (hash & 0x7FFFFFFF);
}
int APHash(char *str){
	int hash = 0;
	int i;
	for (i=0;*str;i++)
		if ((i&1)==0)
			hash^=((hash<<7)^(*str++)^(hash>>3));
		else
			hash^=(~((hash<<11)^(*str++)^(hash>>5)));
	return (hash & 0x7FFFFFFF);
}
void init(int n){
	size=3*n/2;
	table=(int *)malloc(size*sizeof(int));
	table2=(int *)malloc(size*sizeof(int));
	value_table=(value_type *)malloc(size*sizeof(value_type));
	memset(table,-1,size*sizeof(int));
	memset(table2,-1,size*sizeof(int));
}
void put(char* host,value_type value){
	int key=BKDRHash(host);
	int key2=APHash(host);
	int h=key%size;
	while(table[h]!=-1&&table[h]!=key&&table2[h]!=key2)
		h=(h+1)%size;
	table[h]=key;
	table2[h]=key2;
	value_table[h]=value;		
}
value_type get(char* host){
	int key=BKDRHash(host);
	int key2=APHash(host);
	int h=key%size;
	while(table[h]!=-1&&table[h]!=key&&table2[h]!=key2)
		h=(h+1)%size;
	if(table[h]==-1)
		return 0;
	return value_table[h];
}
int removebykey(char* host){
	int key=BKDRHash(host);
	int key2=APHash(host);
	int h=key%size;
	while(table[h]!=-1&&table[h]!=key&&table2[h]!=key2)
		h=(h+1)%size;
	if(table[h]==-1)
		return 0;
	table[h]=-1;
	table2[h]=-1;
	return 1;
}
