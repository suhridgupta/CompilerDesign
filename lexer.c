#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void getLexFile(){
	FILE *pf,*pout;
	pf = fopen("Prog.c","r");
	pout = fopen("LexProg.c","w");
	char lexeme[1000];
	int lexemeLen = 0;
	if(pf == NULL || pout == NULL){
		printf("Error in Opening\n");
		exit(0);
	}
	char c;
	while(1){
		lexemeLen = 0;
		c = fgetc(pf);
		if(c == EOF){
			fputc('<',pout);
			fputc('$',pout);
			fputc('>',pout);
			fclose(pf);
			fclose(pout);
			return;
		}
		if(c == ' ' || c == '\t' || c == '\n'){
			fputc(c,pout);
			continue;
		}
		else if(isalpha(c) || c == '_'){
			lexeme[lexemeLen++] = c;
			c = fgetc(pf);
			while(isalnum(c) || c == '_'){
				lexeme[lexemeLen++] = c;
				c = fgetc(pf);
			}
			ungetc(c,pf);
			//ungetc(c,pf);
		}
		else if(isdigit(c)){
			lexeme[lexemeLen++] = c;
			c = fgetc(pf);
			while(isdigit(c) || c == '.'){
				lexeme[lexemeLen++] = c;
				c = fgetc(pf);
			}
			ungetc(c,pf);
			//ungetc(c,pf);
		}
		else if(c == '+'){
			lexeme[lexemeLen++] = c;
			c = fgetc(pf);
			if(c == '+' || c == '=')
				lexeme[lexemeLen++] = c;
			else
				ungetc(c,pf);
		}
		else if(c == '-'){
			lexeme[lexemeLen++] = c;
			c = fgetc(pf);
			if(c == '-' || c == '=')
				lexeme[lexemeLen++] = c;
			else 
				ungetc(c,pf);
		}
		else if(c == '*'){
			lexeme[lexemeLen++] = c;
			c = fgetc(pf);
			if(c == '=')
				lexeme[lexemeLen++] = c;
			else
				ungetc(c,pf);
		}
		else if(c == '/'){
			lexeme[lexemeLen++] = c;
			c = fgetc(pf);
			if(c == '=')
				lexeme[lexemeLen++] = c;
			else
				ungetc(c,pf);
		}
		else if(c == '='){
			lexeme[lexemeLen++] = c;
			c = fgetc(pf);
			if(c == '=')
				lexeme[lexemeLen++] = c;
			else
				ungetc(c,pf);
		}
		else{
			lexeme[lexemeLen++] = c;
		}
		fputc('<',pout);
		fwrite(lexeme,lexemeLen,1,pout);
		fputc('>',pout);
	}
}