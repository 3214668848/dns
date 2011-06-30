#include<stdio.h>
#include<string.h>
typedef int value_type;
int size;
struct trie_node{
	struct trie_node* next[36];
	value_type value;
	int count;
}*root;
void init(int n){
	size=3*n/2;
	root=malloc(sizeof(struct trie_node));
	memset(root,0,sizeof(struct trie_node));
}
int char2num(char c){
	if(c>='a'&&c<='z')
		return c-'a';
	if(c>='A'&&c<='Z')
		return c-'A';
	return 26+c-'0';
}
void put(char* host,value_type value){
	struct trie_node* temp=root;
	struct trie_node* newnode=0;	
	while (*host){
		if((*host)!='.'){
			if(!temp->next[char2num(*host)]){
				newnode=malloc(sizeof(struct trie_node));
				memset(newnode,0,sizeof(struct trie_node));
				temp->next[char2num(*host)]=newnode;
				temp->count++;
			}
			temp=temp->next[char2num(*host)];
		}
		host++;	
	}
	temp->value=value;
}
value_type get(char* host){
	struct trie_node* temp=root;
	while (*host){
		if((*host)!='.'){
			if(!temp->next[char2num(*host)])
				return 0;
			temp=temp->next[char2num(*host)];
		}
		host++;	
	}
	return temp->value;
}
int removebykey(char* host){
	return myremove(root,host);
}
int myremove(struct trie_node* now,char* host){
	while((*host)=='.')
		host++;
	if(!now)
		return 0;
	if(!(*host)){
		return 1;
	}
	int temp=myremove(now->next[char2num(*host)],host+1);
	if(temp==0)
		return 0;
	if(!now->next[char2num(*host)]->count){
		free(now->next[char2num(*host)]);
		now->next[char2num(*host)]=0;
	}
	if(!now->next[char2num(*host)]&&now->count)
		now->count--;
	return 1;
}
