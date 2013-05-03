%{
enum RLTokenValueType {
    BoolC, NumbC,
    BoolI, NumbI, ProcI, MarkI,
};

struct RLTokenValue {
    int val;
    RLTokenValueType type;
};

#define YYSTYPE RLTokenValue

int yydebug = 1;

extern FILE* yyin;
extern int yyparse(void);
extern int yywrap(void);
extern int yylex(void);

extern int main(int,const char**);

void yyerror(const char* str) {
        fprintf(stderr,"Error: %s\n",str);
}

int yywrap() {
        return 1;
}

int main(int argn,const char** arg) {
    if(argn == 2) {
        yyin = fopen(arg[1],"r");
    }

    do {
            yyparse();
    } while(!feof(yyin));

    return 0;
}

%}

%start procedure
%token OBRACE EBRACE OFBRACE EFBRACE SEMICOLON // DELIMETRS
BOOLI NUMBI // IDENTIFIER
BOOLC NUMBC // CONSTANT
ASSIGN COMPARE INC DEC // OPERATORS
PRINT


%%
procedure:
    |
    procedure line
    ;

line:
    command SEMICOLON
    ;

command:
    returnable
    |
    nonreturnable
    ;

nonreturnable:
    PRINT
    ;

returnable:
    ident ASSIGN returnable {
    }
    |
    returnable COMPARE returnable {
    }
    |

ident:
    BOOLI
    ;

