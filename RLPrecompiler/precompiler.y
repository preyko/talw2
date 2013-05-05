%{
enum RLTokenValueType {
    BoolC, NumbC,
    BoolI, NumbI, ProcI, MarkI,
};

struct RLTokenValue {
    int val;
    RLTokenValueType type;
};

//#define YYSTYPE RLTokenValue
#define YYSTYPE std::string

int yydebug = 1;
extern int yylineno;
extern char* yytext;

extern int yyparse(void);
extern int yywrap(void);
extern int yylex(void);

void yyerror(const char* str) {
        fprintf(stderr,"Error on line %d: %s\n",yylineno, str);
}

int yywrap() {
        return 1;
}

void Precompiler(const char* token_input) {
    yyin = fopen(token_input,"r");

    if(yyin == NULL)
        throw Exception(std::string("Can't open file called ") +
                        std::string(token_input));

    if(logstream == NULL)
        logstream = &std::cout;

    yydebug = 1;

    do {
        yyparse();
    } while(!feof(yyin));
}

%}

%start procedure
%token OBRACE EBRACE OFBRACE EFBRACE OSBRACE ESBRACE SEMICOLON // DELIMETRS
BOOLI NUMBI MARKI PROCI // IDENTIFIER
BOOLC NUMBC // CONSTANT
ASSIGN COMPARE INC DEC LINK RLINK NOR  // OPERATORS
PRINT PLEASE


%%
procedure:
    procedure line
    |
    ;

line:
    command SEMICOLON {
        //(*logstream) << "RLInterpreter::addCommand($1);\n";
        (*logstream) << "Add command in RLInterpreter: " << $1 << std::endl;
    }
    |
    nonreturnable  {
        (*logstream) << "Add command in RLInterpreter: " << $1 << std::endl;
    }
    ;

command:
    returnable
    ;

nonreturnable:
    cycle_condition limited_procedure {
        (*logstream) << "Complite cycle and assign the procedure.\n";
        $$ = $1;
        $$ += " after exec ";
        $$ += $2;
    }
    |
    OSBRACE returnable ESBRACE OSBRACE PLEASE ESBRACE OSBRACE MARKI ESBRACE SEMICOLON {
        $$ = "make transition, if ";
        $$ += $2;
        $$ += " true";
    }
    ;

returnable:
    limited_procedure {
        $$ = $1;
    }
    |
    ident ASSIGN returnable {
        //(*logstream) << "$$ = new RLCommand(assign,$1,$3);\n";
        $$ = $1;
        $$ += " = ";
        $$ += $3;
    }
    |
    returnable COMPARE returnable {
        //(*logstream) << "$$ = new RLCommand(compare,$1,$3);\n";
        $$ = $1;
        $$ += " == ";
        $$ += $3;
    }
    |
    const {
        $$ = $1;
    }
    |
    ident {
        $$ = $1;
    }
    |
    ident INC {
        $$ = "inc ";
        $$ += $1;
    }
    |
    ident DEC {
        $$ = "dec ";
        $$ += $1;
    }
    ;

ident:
    BOOLI {
        //(*logstream) << "if(RLIdentifierRegister::get($1)==NULL)\n";
        //(*logstream) << "  $$ = new RLDereference(new RLBool(false,$1));\n";
        //(*logstream) << "else\n";
        //(*logstream) << "  RLTypePrototype* res = RLIdentifierRegister::get($1);\n";
        //(*logstream) << "  if(res->getTypeQualifier() != Bool)\n";
        //(*logstream) << "      drop errror;\n";
        //(*logstream) << "  else\n";
        //(*logstream) << "      $$ = new RLDereference(res);\n";
        $$ = "id ";
        $$ += &yytext[9];
    }
    |
    NUMBI {
        //(*logstream) << "if(RLIdentifierRegister::get($1)==NULL)\n";
        //(*logstream) << "  $$ = new RLDereference(new RLNumber(0,$1));\n";
        //(*logstream) << "else\n";
        //(*logstream) << "  RLTypePrototype* res = RLIdentifierRegister::get($1);\n";
        //(*logstream) << "  if(res->getTypeQualifier() != Number)\n";
        //(*logstream) << "      drop errror;\n";
        //(*logstream) << "  else\n";
        //(*logstream) << "      $$ = new RLDereference(res);\n";
        $$ = "id ";
        $$ += &yytext[9];
    }
    |
    PROCI {
        //(*logstream) << "if(RLIdentifierRegister::get($1)==NULL)\n";
        //(*logstream) << "  $$ = new RLDereference(new RLProcedure(false,$1));\n";
        //(*logstream) << "else\n";
        //(*logstream) << "  RLTypePrototype* res = RLIdentifierRegister::get($1);\n";
        //(*logstream) << "  if(res->getTypeQualifier() != Procedure)\n";
        //(*logstream) << "      drop errror;\n";
        //(*logstream) << "  else\n";
        //(*logstream) << "      $$ = new RLDereference(res);\n";
        $$ = "id ";
        $$ += &yytext[9];
    }
    |
    MARKI {
        //(*logstream) << "if(RLIdentifierRegister::get($1)==NULL)\n";
        //(*logstream) << "  $$ = new RLDereference(new RLMark(false,$1));\n";
        //(*logstream) << "else\n";
        //(*logstream) << "  RLTypePrototype* res = RLIdentifierRegister::get($1);\n";
        //(*logstream) << "  if(res->getTypeQualifier() != Bool)\n";
        //(*logstream) << "      drop errror;\n";
        //(*logstream) << "  else\n";
        //(*logstream) << "      $$ = new RLDereference(res);\n";
        $$ = "set mark on line ";
        $$ += yylineno;
        $$ += " with id ";
        $$ += &yytext[9];
    }
    ;

const:
    BOOLC {
        //(*logstream) << "$$ = new RLDereference($1)\n";
        $$ = "bva ";
        $$ += &yytext[10];
    }
    |
    NUMBC {
        //(*logstream) << "$$ = new RLDereference($1)\n";
        $$ = "nval ";
        $$ += &yytext[10];
    }
    ;

cycle_condition:
    OBRACE returnable EBRACE {
        (*logstream) << "Only return the condition, RLCycle will be created higher.\n";
        $$ = "cycle condition: ";
        $$ += $2;
    }
    ;


limited_procedure:
    start_procedure procedure end_procedure {
        (*logstream) << "Down the stack and return RLProcedure.\n";
        $$ = "procedure";
    }
    ;

start_procedure:
    OFBRACE {
        (*logstream) << "Up the stack.\n";
    }
    ;
end_procedure:
    EFBRACE {
    }
