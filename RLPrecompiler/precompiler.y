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
//#define YYSTYPE std::string
#define YYSTYPE RLCommandPrototype*

struct Token {
    std::string d;
    RLCommandPrototype* t;
};

//#define YYSTYPE Token*



int yydebug = 1;
YYSTYPE yyval;
extern int yylineno;
extern char* yytext;

extern int yyparse(void);
extern int yywrap(void);
extern int yylex(void);

void yyerror(const char* str) {
        (*logstream) << "Error on line " << yylineno << ": " << str << std::endl;
}

int yywrap() {
        return 0;
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
EOf


%%
procedure:
    procedure line
    |
    procedure EOf {
        return 0;
    }
    |
    ;

line:
    command SEMICOLON {
        RLInterpreter::addCommand($1);
        //Add command in RLInterpreter: " << $1->d << std::endl;
    }
    |
    nonreturnable  {
        RLInterpreter::addCommand($1);
        //Add command in RLInterpreter: " << $1->d << std::endl;
    }
    ;

command:
    returnable
    ;

nonreturnable:
    cycle_condition limited_procedure {
        (*logstream) << "Complite cycle and assign the procedure.\n";
        //$$->d = $1->d;
        //$$->d += " after exec ";
        //$$->d += $2->d;
    }
    |
    OSBRACE returnable ESBRACE OSBRACE PLEASE ESBRACE OSBRACE MARKI ESBRACE SEMICOLON {
        //$$->d = "make transition, if ";
        //$$->d += $2->d;
        //$$->d += " true";
    }
    |
    PRINT returnable SEMICOLON {
        $$ = new RLCommand(show,$2);
    }
    ;

returnable:
    limited_procedure {
        //$$->d = $1->d;
    }
    |
    ident ASSIGN returnable {
        $$ = new RLCommand(assign,$1,$3);
        //$$->d = $1->d;
        //$$->d += " = ";
        //$$->d += $3->d;
    }
    |
    returnable COMPARE returnable {
        $$ = new RLCommand(compare,$1,$3);
        //$$->d = $1->d;
        //$$->d += " == ";
        //$$->d += $3->d;
    }
    |
    const {
        //$$->d = $1->d;
    }
    |
    ident {
        //$$->d = $1->d;
    }
    |
    ident INC {
        //$$->d = "inc ";
        //$$->d += $1->d;
        $$ = new RLCommand(increment,$1);
    }
    |
    ident DEC {
        //$$->d = "dec ";
        //$$->d += $1->d;
        $$ = new RLCommand(decrement,$1);
    }
    ;

ident:
    BOOLI {
        if(RLIdentRegister::get(atoi(&yytext[9]))==NULL)
          $$ = new RLDereference(new RLBool(false,atoi(&yytext[9])));
        else {
          RLTypePrototype* res = RLIdentRegister::get(atoi(&yytext[9]));
          if(res->getTypeQualifier() != RLTypePrototype::Bool)
              throw "ERROR: Expected Bool!";
          else
              $$ = new RLDereference(res);
        }
        //$$->d = "id ";
        //$$->d += &yytext[9];
    }
    |
    NUMBI {
        if(RLIdentRegister::get(atoi(&yytext[9]))==NULL)
            $$ = new RLDereference(new RLNumber(0,atoi(&yytext[9])));
        else {
            RLTypePrototype* res = RLIdentRegister::get(atoi(&yytext[9]));
            if(res->getTypeQualifier() != RLTypePrototype::Number)
                throw "ERROR: Expected Number!";
            else
                $$ = new RLDereference(res);
        }
        //$$->d = "id ";
        //$$->d += &yytext[9];
    }
    |
    PROCI {
        /*
        if(RLIdentRegister::get(atoi(&yytext[9]))==NULL)
          $$ = new RLDereference(new RLProcedure(false,atoi(&yytext[9])));
        else
          RLTypePrototype* res = RLIdentRegister::get(atoi(&yytext[9]));
          if(res->getTypeQualifier() != Procedure)
              //drop errror;
          else
              $$ = new RLDereference(res);
        */
        //$$->d = "id ";
        //$$->d += &yytext[9];
    }
    |
    MARKI {
        /*
        if(RLIdentRegister::get(atoi(&yytext[9]))==NULL)
          $$ = new RLDereference(new RLMark(false,atoi(&yytext[9])));
        else
          RLTypePrototype* res = RLIdentRegister::get(atoi(&yytext[9]));
          if(res->getTypeQualifier() != RLTypePrototype::Bool)
              //drop errror;
          else
              $$ = new RLDereference(res);
        */
        //$$->d = "set mark on line ";
        //$$->d += yylineno;
        //$$->d += " with id ";
        //$$->d += &yytext[9];
    }
    ;

const:
    BOOLC {
        bool res;
        if(yytext[10] == 'T')
            res = true;
        else
            res = false;

        $$ = new RLDereference(new RLBool(res));
        //$$->d = "bva ";
        //$$->d += &yytext[10];
    }
    |
    NUMBC {
        $$ = new RLDereference(new RLNumber(atoi(&yytext[10])));
        //$$->d = "nval ";
        //$$->d += &yytext[10];
    }
    ;

cycle_condition:
    OBRACE returnable EBRACE {
        (*logstream) << "Only return the condition, RLCycle will be created higher.\n";
        //$$->d = "cycle condition: ";
        //$$->d += $2->d;
    }
    ;


limited_procedure:
    start_procedure procedure end_procedure {
        (*logstream) << "Down the stack and return RLProcedure.\n";
        //$$->d = "procedure";
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
