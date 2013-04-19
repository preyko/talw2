%{
#include <stdio.h>
#include <string.h>

#include "rltype/RLType.h"

//#define YYSTYPE RLType*


int yydebug = 1;

extern FILE* yyin;

extern "C" {
	int yyparse(void);
	int yywrap(void);
	int yylex(void);
	
	int main(int,const char**);
}

void yyerror(const char* str) {
        fprintf(stderr,"Error: %s\n",str);
}

int yywrap() {
        return 1;
}

int main(int argn,const char** arg) {
	yyin = fopen("tests.txt","r");
	
	do {
        	yyparse();
	} while(!feof(yyin));

	return 0;
}

%}

%start body
%token OBRACE EBRACE OFBRACE EFBRACE SEMICOLON TEST // DELIMETRS
BOOLIDNT NUMBIDNT // IDENTIFIER
BOOLCONST NUMBCONST // CONSTANT
ASSIGOPER EQOPER INCOPER DECOPER // OPERATORS

%%
body:
        |
        body command
        ;

command:
	operators SEMICOLON
	;

const:
	BOOLCONST {
		$$=$1;
	}
	|
	NUMBCONST {
		$$=$1;
	}
	;

ident:
	BOOLIDNT {
		$$=$1;
	}
	|
	NUMBIDNT {
		$$=$1;
	}
	;

operators:
	returnable_operators
	|
	nonreturnable_operators
	;
	
returnable_operators:
	ident ASSIGOPER const {
                //printf("Ident %d = %d\n",$1,$3);
	}
	|
	ident EQOPER const {
                //printf("Ident %d == %d\n",$1,$3);
	}
	|
	ident DECOPER {
                //printf("Ident %d decrement\n",$1);
	}
	|
	ident INCOPER {
                //printf("Ident %d increment\n",$1);
	}
	;

nonreturnable_operators:
	TEST
	;
