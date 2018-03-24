#include <stdio.h>
#include "lexer.c"
#include <ctype.h>
#include <string.h>

#define CMP(X,Y) (!strcmp(X,Y))

FILE *pf;

char token[100];
int token_len = 0;

void program();
void declarations();
void data_type();
void identifier_list();
void identifier_list1();
void assign_stat();
void assign_stat1();

int isID(){
	if(isalpha(token[0]))
		return 1;
	return 0;
}

void getNextToken(){
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

void failure(){
	printf("Failure\n");
	while(!CMP(token,"$")){
		puts(token);
		getNextToken();
	}
	exit(0);
}

void program(){
	getNextToken();
	if(CMP(token,"main")){
		getNextToken();
		if(CMP(token,"(")){
			getNextToken();
			if(CMP(token,")")){
				getNextToken();
				if(CMP(token,"{")){
					getNextToken();
					declarations();
					assign_stat();
					if(CMP(token,"}")){
						getNextToken();
					}
					else{
						failure();
					}
				}
				else{
					failure();
				}
			}
			else{
				failure();
			}
		}
		else{
			failure();
		}
	}
	if(CMP(token,"$")){
		printf("Success\n");
		exit(0);
	}
	else{
		failure();
	}
}

void declarations(){
	if(CMP(token,"int") || CMP(token,"char")){
		data_type();
		identifier_list();	
		if(CMP(token,";")){
			getNextToken();
			declarations();
		}
	}
	else if(isID(token)){
		return;
	}
	else{
		failure();
	}
}

void data_type(){
	if(CMP(token,"int") || CMP(token,"char")){
		getNextToken();
	}
	else{
		failure();
	}
}

void identifier_list(){
	if(isID(token)){
		getNextToken();
		identifier_list1();
	}
	else{
		failure();
	}
}

void identifier_list1(){
	if(CMP(token,",")){
		getNextToken();
		identifier_list();
	}
	else if(CMP(token,";")){
		return;
	}
	else
		failure();
}

void assign_stat(){
	if(isID(token)){
		getNextToken();
		if(CMP(token,"=")){
			getNextToken();
			assign_stat1();
		}
		else{
			failure();
		}
	}
	else{
		failure();
	}
}

void assign_stat1(){
	if(isID(token) || isdigit(token[0])){
		getNextToken();
	}
	if(CMP(token,";")){
		getNextToken();
	}
	else{
		failure();
	}
}

int main(){
	getLexFile();
	pf = fopen("LexProg.c","r");
	if(pf == NULL){
		printf("Error opening file\n");
		exit(0);
	}
	program();
	return 0;
}