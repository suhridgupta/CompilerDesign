#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.c"

#define CMP(X,Y) (!strcmp(X,Y))
#define MAX_LEN 1000
#define MAX_HASH_SIZE 100

char token[MAX_LEN];
int token_len = 0;
char prev_data_type[MAX_LEN];
int scope = 0;

typedef struct hash{
	char name[MAX_LEN];
	char type[MAX_LEN];
	int size;
	char scope;
	int num_args;
	char args[MAX_LEN];
	char ret_type[MAX_LEN];
	struct hash *next;
}Hash;

Hash *createNode(char *name,char *type,int size,char scope,int num_args,char *args,char ret_type[]){
	Hash *newNode = (Hash*)malloc(sizeof(Hash));
	strcpy(newNode->name,name);
	strcpy(newNode->type,type);
	newNode->size = size;
	newNode->scope = scope;
	newNode->num_args = num_args;
	strcpy(newNode->args,args);
	strcpy(newNode->ret_type,ret_type);
	newNode->next = NULL;
	return newNode;
}

Hash* insertElement(Hash *H,char *name,char *type,int size,char scope,int num_args,char *args,char ret_type[]){
	Hash *newNode = createNode(name,type,size,scope,num_args,args,ret_type);
	Hash *temp = H;
	if(H == NULL){
		H = newNode;
		return H;
	}
	while(temp->next!=NULL)
		temp = temp->next;
	temp->next = newNode;
	return H;
}

void printHash(Hash *H){
	if(H == NULL){
		return;
	}
	while(H!=NULL){
		printf("< ");
		printf("%s ",H->name);
		if(strcmp(H->type,"NULL")!=0)
			printf("%s ",H->type);
		if(H->size!=-1)
			printf("%d ",H->size);
		printf("%c ",H->scope);
		if(H->num_args!=0)
			printf("%d ",H->num_args);
		if(strcmp(H->args,"NULL")!=0)
		printf("<%s> ",H->args);
		if(strcmp(H->ret_type,"NULL")!=0)
		printf("%s ",H->ret_type);
		printf(">\n");
		H = H->next;
	}
}

int searchHash(Hash *H,char *name){
	Hash *temp = H;
	int compare=0;
	while(temp!=NULL){
		if(strcmp(temp->name,name) == 0){
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

void getNextToken(FILE *pf){
	token_len = 0;
	char c;
	c = fgetc(pf);
	while(isspace(c)){
		c = fgetc(pf);
	}
	if(c == '<'){
		c = fgetc(pf);
		while(c != '>'){
			token[token_len++] = c;
			c = fgetc(pf);
		}
	}
	token[token_len++] = '\0';
}

int isKeyword(){
	char keyword[10][MAX_LEN] = {"for","if","else","while","do","break","switch","case","return","continue"};
	int i;
	for(i=0;i<10;i++)
		if(CMP(token,keyword[i]))
			return 1;
	return 0;
}

int isDatatype(){
	char datatype[5][MAX_LEN] = {"int","float","double","char","void"};
	int i;
	for(i=0;i<5;i++)
		if(CMP(token,datatype[i]))
			return 1;
	return 0;
}

int isFunctionOrIdentifier(){
	if(isalpha(token[0]) || token[0] == '_')
		return 1;
	return 0;
}

int isNumber(){
	if(isdigit(token[0]))
		return 1;
	return 0;
}

int getDataTypeValue(){
	if(CMP(prev_data_type,"int"))
		return 4;
	if(CMP(prev_data_type,"float"))
		return 6;
	if(CMP(prev_data_type,"double"))
		return 8;
	if(CMP(prev_data_type,"char"))
		return 1;
	return 0;
}

int main(){
	getLexFile();
	FILE *pf;
	pf = fopen("LexProg.c","r");
	if(pf == NULL){
		printf("Error opening file\n");
		exit(0);
	}
	Hash *H[MAX_HASH_SIZE];
	int hash_func = 10,i,result;

	for(i=0;i<hash_func;i++){
		H[i] = NULL;
	}
	getNextToken(pf);
	while(!CMP(token,"$")){
		if(CMP(token,"{"))
			scope++;
		if(CMP(token,"}"))
			scope--;
		if(isDatatype())
			strcpy(prev_data_type,token);
		else if(!isKeyword() && isFunctionOrIdentifier()){
			char id_func_name[MAX_LEN];
			strcpy(id_func_name,token);
			getNextToken(pf);
			if(CMP(token,"(")){
				getNextToken(pf);
				int num_args = 0;
				char *args = (char*)malloc(sizeof(MAX_LEN));
				int arg_len = 0;
				while(!CMP(token,")")){
					if(CMP(token,",")){
						if(num_args == 0)
							num_args = 2;
						else
							num_args++;
					}
					for(i=0;i<token_len-1;i++){
						args[arg_len+i] = token[i];
					}
					arg_len+=token_len-1;
					args[arg_len++] = ' ';
					getNextToken(pf);
				}
				args[arg_len++] = '\0';
				if(searchHash(H[id_func_name[0]%hash_func],id_func_name) == 0){
					H[id_func_name[0]%hash_func] = insertElement(H[id_func_name[0]%hash_func],id_func_name,"FUNC",-1,scope==0?'G':'L',num_args,args,prev_data_type);
				}
			}
			else if(CMP(token,"[")){
				char array_size[MAX_LEN];
				int array_len = 0;
				getNextToken(pf);
				strcpy(array_size,token);
				if(searchHash(H[id_func_name[0]%hash_func],id_func_name) == 0){
					H[id_func_name[0]%hash_func] = insertElement(H[id_func_name[0]%hash_func],id_func_name,prev_data_type,getDataTypeValue()*atoi(array_size),scope==0?'G':'L',0,"NULL","NULL");
				}
			}
			else{
				if(searchHash(H[id_func_name[0]%hash_func],id_func_name) == 0){
					H[id_func_name[0]%hash_func] = insertElement(H[id_func_name[0]%hash_func],id_func_name,prev_data_type,getDataTypeValue(),scope==0?'G':'L',0,"NULL","NULL");
				}
			}	
		}
		getNextToken(pf);
	}
	for(i=0;i<hash_func;i++){
		printHash(H[i]);
	}
	return 0;
}