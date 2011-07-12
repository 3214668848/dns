#include"../dns.h"

typedef int hash_valuetype;
typedef struct _node{
	int value;
	struct _node* next;
} node;
int size;
int* table;
int* table2;
node** value_table;
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
void hash_init(int n){
	size=3*n/2;
	table=(int *)malloc(size*sizeof(int));
	table2=(int *)malloc(size*sizeof(int));
	value_table=(node**)malloc(size*sizeof(node*));
	memset(table,-1,size*sizeof(int));
	memset(table2,-1,size*sizeof(int));
}
void hash_put(char* host,hash_valuetype value){
	int key=BKDRHash(host);
	int key2=APHash(host);
	int h=key%size;
	while(table[h]!=-1&&(table[h]!=key||table2[h]!=key2))
		h=(h+1)%size;
	table[h]=key;
	table2[h]=key2;
	node *new=(node*)malloc(sizeof(node));
	sizeof(new,0,sizeof(node));
	new->value=value;
	new->next=value_table[h];
	value_table[h]=new;		
}
hash_valuetype hash_get(char* host){
	int key=BKDRHash(host);
	int key2=APHash(host);
	int h=key%size;
	while(table[h]!=-1&&(table[h]!=key||table2[h]!=key2))
		h=(h+1)%size;
	if(table[h]==-1)
		return 0;
	return value_table[h]->value;
}
int hash_removebykey(char* host){
	int key=BKDRHash(host);
	int key2=APHash(host);
	int h=key%size;
	while(table[h]!=-1&&(table[h]!=key||table2[h]!=key2))
		h=(h+1)%size;
	if(table[h]==-1)
		return 0;
	node *old=value_table[h];
	value_table[h]=value_table[h]->next;
	free(old);
	if(value_table[h]==0){
		table[h]=-1;
		table2[h]=-1;
	}
	return 1;
}

