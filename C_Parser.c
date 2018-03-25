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
void identifier_list2();
void statement_list();
void statement();
void assign_stat();
void expn();
void eprime();
void simple_expn();
void seprime();
void term();
void tprime();
void factor();
void decision_stat();
void dprime();
void looping_stat();
void relop();
void addop();
void mulop();

int isID(){
	if(isalpha(token[0]))
		return 1;
	return 0;
}

int isNum(){
	if(isdigit(token[0]))
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
					statement_list();
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
	else if(isID(token) || CMP(token,"}")){
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
	if(isID()){
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
	else if(CMP(token,"[")){
		getNextToken();
		if(isNum()){
			getNextToken();
			if(CMP(token,"]")){
				getNextToken();
				identifier_list2();
			}
			else{
				failure();
			}
		}
		else{
			failure();
		}
	}
	else if(CMP(token,";")){
		return;
	}
	else
		failure();
}

void identifier_list2(){
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

void statement_list(){
	if(isID()){
		statement();
		statement_list();
	}
	if(CMP(token,"}")){
		return;
	}
	else{
		failure();
	}
}

void statement(){
	if(isID() && !CMP(token,"if") && !CMP(token,"while") && !CMP(token,"for")){
		assign_stat();
		if(CMP(token,";")){
			getNextToken();
		}
	}
	else if(CMP(token,"if")){
		decision_stat();
	}
	else if(CMP(token,"while") || CMP(token,"for")){
		looping_stat();
	}
	else{
		failure();
	}
}

void assign_stat(){
	if(isID()){
		getNextToken();
		if(CMP(token,"=")){
			getNextToken();
			expn();
		}
		else{
			failure();
		}
	}
	else{
		failure();
	}
}

void expn(){
	if(isID() || isNum()){
		simple_expn();
		eprime();
	}
	else{
		printf("HERE IN EXPN\n");
		failure();
	}
}

void eprime(){
	if(CMP(token,"==") || CMP(token,"!=") || CMP(token,"<=") || CMP(token,">=") || CMP(token,">") || CMP(token,"<")){
		relop();
		simple_expn();
	}
	else if(CMP(token,";") || CMP(token,")")){
		return;
	}
	else{
		failure();
	}
}

void simple_expn(){
	if(isID() || isNum()){
		term();
		seprime();
	}
	else{
		failure();
	}
}

void seprime(){
	if(CMP(token,"+") || CMP(token,"-")){
		addop();
		term();
		seprime();
	}
	if(CMP(token,";") || CMP(token,")") || CMP(token,"==") || CMP(token,"!=") || CMP(token,"<=") || CMP(token,">=") || CMP(token,">") || CMP(token,"<")){
		return;
	}
	else{
		failure();
	}
}

void term(){
	if(isID() || isNum()){
		factor();
		tprime();
	}
	else{
		failure();
	}
}

void tprime(){
	if(CMP(token,"*") || CMP(token,"/") || CMP(token,"%")){
		mulop();
		factor();
		tprime();
	}
	if(CMP(token,"+") || CMP(token,"-") || CMP(token,";") || CMP(token,")") || CMP(token,"==") || CMP(token,"!=") || CMP(token,"<=") || CMP(token,">=") || CMP(token,">") || CMP(token,"<")){
		return;
	}
	else{
		failure();
	}
}

void factor(){
	if(isID() || isNum()){
		getNextToken();
	}
	else
		failure();
}

void decision_stat(){
	if(CMP(token,"if")){
		getNextToken();
		if(CMP(token,"(")){
			getNextToken();
			expn();
			if(CMP(token,")")){
				getNextToken();
				if(CMP(token,"{")){
					getNextToken();
					statement_list();
					if(CMP(token,"}")){
						getNextToken();
						dprime();
					}
					else
						failure();
				}
				else
					failure();
			}
			else
				failure();
		}
		else
			failure();
	}
	else
		failure();
}

void dprime(){
	if(CMP(token,"else")){
		getNextToken();
		if(CMP(token,"{")){
			getNextToken();
			statement_list();
			if(CMP(token,"}")){
				getNextToken();
			}
			else
				failure();
		}
		else
			failure();
	}
	else if(CMP(token,"}") || isID()){
		return;
	}
	else
		failure();
}

void looping_stat(){
	if(CMP(token,"while")){
		getNextToken();
		if(CMP(token,"(")){
			getNextToken();
			expn();
			if(CMP(token,")")){
				getNextToken();
				if(CMP(token,"{")){
					getNextToken();
					statement_list();
					if(CMP(token,"}")){
						getNextToken();
					}
					else
						failure();
				}
				else
					failure();
			}
			else
				failure();
		}
		else
			failure();
	}
	else if(CMP(token,"for")){
		getNextToken();
		if(CMP(token,"(")){
			getNextToken();
			assign_stat();
			if(CMP(token,";")){
				getNextToken();
				expn();
				if(CMP(token,";")){
					getNextToken();
					assign_stat();
					if(CMP(token,")")){
						getNextToken();
						if(CMP(token,"{")){
							getNextToken();
							statement_list();
							if(CMP(token,"}")){
								getNextToken();
							}
							else
								failure();
						}
						else
							failure();
					}
					else
						failure();
				}
				else
					failure();
			}
			else
				failure();
		}
		else
			failure();
	}
	else
		failure();
}

void relop(){
	if(CMP(token,"==") || CMP(token,"!=") || CMP(token,"<=") || CMP(token,">=") || CMP(token,">") || CMP(token,"<")){
		getNextToken();
	}
	else
		failure();
}

void addop(){
	if(CMP(token,"+") || CMP(token,"-")){
		getNextToken();
	}
	else
		failure();
}

void mulop(){
	if(CMP(token,"*") || CMP(token,"/") || CMP(token,"%")){
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