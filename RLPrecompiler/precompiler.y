%{
#include <iostream>

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

int main(int argn,const char** arg) {
    if(argn == 2) {
        yyin = fopen(arg[1],"r");
        token_output = fopen("tokeno.txt","r");
    }

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
        }
        |
        NUMBCONST {
        }
        ;

ident:
        BOOLIDNT {
        }
        |
        NUMBIDNT {
        }
        ;

operators:
        returnable_operators
        |
        nonreturnable_operators
        ;

returnable_operators:
        ident ASSIGOPER const {
        }
        |
        ident EQOPER const {
        }
        |
        ident DECOPER {
        }
        |
        ident INCOPER {
        }
        |
        ident {
        }
        ;

nonreturnable_operators:
        PRINTOPER returnable_operators {
        }
        ;
