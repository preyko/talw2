%{
#include <stdio.h>
#include <string.h>


#define YYSTYPE void*

int yydebug = 1;

extern FILE* yyin;
extern FILE* token_output;

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


/****** PRECOMPILLER HEAD ******/
#include "rltype/RLType.h"

int main(int argn,const char** arg) {
	yyin = fopen("tests.txt","r");
	token_output = fopen("tokeno.txt","r");
	
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
PRINTOPER


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
		$$=(void*)new RLBool((bool)$1);
	}
	|
	NUMBCONST {
		$$=(void*)new RLNumber((int)$1);
	}
	;

ident:
	BOOLIDNT {
		RLTypePrototype* tmp = RLIdentRegister::get((int)$1);
		if(tmp == NULL)
			tmp = new RLBool(false,(int)$1);
		else if(tmp->getTypeQualifier() != RLType::Bool) {
			printf("Error! Identifier with id %d is not Bool type.\n",(int)$1);
			exit(1);
		}

		$$=(void*)tmp;
	}
	|
	NUMBIDNT {
		RLTypePrototype* tmp = RLIdentRegister::get((int)$1);
		if(tmp == NULL)
			tmp = new RLNumber(0,(int)$1);
		else if(tmp->getTypeQualifier() != RLType::Number) {
			printf("Error! Identifier with id %d is not Number type.\n",(int)$1);
			exit(1);
		}

		$$=(void*)tmp;
	}
	;

operators:
	returnable_operators
	|
	nonreturnable_operators
	;
	
returnable_operators:
	ident ASSIGOPER const {
                RLTypePrototype* ident = (RLTypePrototype*) $1;
		$$=(void*)ident->applyBinary(assign,(RLTypePrototype*)$3);
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
	|
	ident {
	}
	;

nonreturnable_operators:
	PRINTOPER returnable_operators {
		RLType* tmp = (RLType*) $2;
		if(tmp!=NULL)
			tmp->print();
	}
	;
