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

YYSTYPE yyval;
extern int yydebug;
extern int yylineno;
extern char* yytext;

extern int yyparse(void);
extern int yywrap(void);
extern int yylex(void);

void yyerror(const char* str) {
        collectionerOfExceptions.add(Exception(std::string(str),yylineno));
}

int yywrap() {
        return 1;
}

void Precompiler(const char* token_input) {
    yyin = fopen(token_input,"r");

    if(yyin == NULL)
        collectionerOfExceptions.add(Exception(std::string("Can't open file called ") +
                        std::string(token_input)));
    yylineno = 1;

    collectionerOfExceptions.clear();
    yydebug = 1;
    do {
        yyparse();
    } while(!feof(yyin));

    if(logstream != NULL) {
        logstream->close();
        delete logstream; logstream = NULL;
    }

    fclose(yyin);

    if(!collectionerOfExceptions.isEmpty()) {
        throw collectionerOfExceptions;
    }
}

%}
%debug
%start procedure
%token OBRACE EBRACE OFBRACE EFBRACE OSBRACE ESBRACE SEMICOLON // DELIMETRS
BOOLI NUMBI MARKI PROCI // IDENTIFIER
BOOLC NUMBC // CONSTANT
ASSIGN COMPARE INC DEC LINK RLINK NOR  // OPERATORS
IF
PRINT PLEASE
EOf

%right COMPARE
%right ASSIGN

%right OFBRACE
%right OBRACE
%right OSBRACE

%%
procedure:
    procedure command
    |
    procedure EOf {
        return 0;
    }
    |
    ;

command:
    returnable SEMICOLON {
        if($1!=NULL)
            $1->setLinePosition(yylineno);
        RLInterpreter::addCommand(new RLCommand(np,$1));
    }
    |
    nonreturnable  {
        if($1!=NULL)
            $1->setLinePosition(yylineno);
        RLInterpreter::addCommand(new RLCommand(np,$1));
    }
    ;

nonreturnable:
    rcondition procedure_declaration {
        getPrecompilerOutput() << "Complite cycle and assign the procedure.\n";
        $$ = new RLCycle(new RLCommand(perform,$2),$1);
    }
    |
    IF rcondition procedure_declaration {
        getPrecompilerOutput() << "Complite condition and assign the procedure.\n";
        $$ = new RLConditional(new RLCommand(perform,$3),$2);
    }
    |
    OSBRACE returnable ESBRACE OSBRACE PLEASE ESBRACE OSBRACE MARKI ESBRACE SEMICOLON {
        collectionerOfExceptions.add(Exception(std::string("Now this doesn't work."),yylineno));
        $$ = new RLConditional(new RLCommand(maketransition,$8),$2);
    }
    |
    PRINT returnable SEMICOLON {
        $$ = new RLCommand(show,$2);
    }
    ;

returnable:
    procedure_declaration {
        $$ = $1;
    }
    |
    ident ASSIGN returnable {
        $$ = new RLCommand(assign,$1,$3);
    }
    |
    returnable COMPARE returnable {
        $$ = new RLCommand(compare,$1,$3);
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
        $$ = new RLCommand(increment,$1);
    }
    |
    ident DEC {
        $$ = new RLCommand(decrement,$1);
    }
    |
    ident LINK ident {
        $$ = new RLCommand(bind,$1,$3);
    }
    |
    ident RLINK ident {
        $$ = new RLCommand(unbind,$1,$3);
    }
    ;

ident:
    BOOLI {
        if(RLIdentRegister::get(atoi(&yytext[9]))==NULL)
            $$ = new RLDereference(new RLBool(false,atoi(&yytext[9])));
        else {
            RLTypePrototype* res = RLIdentRegister::get(atoi(&yytext[9]));
            if(res->getTypeQualifier() == RLTypePrototype::Bool)
                $$ = new RLDereference(res);
            else if(res->getTypeQualifier() == RLTypePrototype::Array) {
                $$ = new RLDereference(res);
            }
            else
                //TODO Reform this exception message!!!
                collectionerOfExceptions.add(Exception(std::string("Expected ") +
                                                       RLTypePrototype::typeName(res->getTypeQualifier()),yylineno));
        }
    }
    |
    NUMBI {
        if(RLIdentRegister::get(atoi(&yytext[9]))==NULL)
            $$ = new RLDereference(new RLNumber(0,atoi(&yytext[9])));
        else {
            RLTypePrototype* res = RLIdentRegister::get(atoi(&yytext[9]));
            if(res->getTypeQualifier() != RLTypePrototype::Number)
                collectionerOfExceptions.add(Exception(std::string("Expected ") +
                                                       RLTypePrototype::typeName(res->getTypeQualifier()),yylineno));
            else
                $$ = new RLDereference(res);
        }
    }
    |
    PROCI {
        if(RLIdentRegister::get(atoi(&yytext[9]))==NULL)
            $$ = new RLDereference(new RLProcedure(atoi(&yytext[9])));
        else {
            RLTypePrototype* res = RLIdentRegister::get(atoi(&yytext[9]));
            if(res->getTypeQualifier() != RLTypePrototype::Procedure)
                collectionerOfExceptions.add(Exception(std::string("Expected ") +
                                                       RLTypePrototype::typeName(res->getTypeQualifier()),yylineno));
            else
                $$ = new RLDereference(res);
        }
    }
    |
    MARKI {
        if(RLIdentRegister::get(atoi(&yytext[9]))!=NULL)
            collectionerOfExceptions.add(Exception(std::string("This mark declared before."),yylineno));
        else
            new RLMark(RLInterpreter::getCurrentFunction(),RLInterpreter::getCurrentFunction()->getSize(),atoi(&yytext[9]));
            $$ = NULL;
    }
    ;

const:
    BOOLC {
        bool res;
        if(yytext[10] == 'T')
            res = true;
        else
            res = false;

        $$ = new RLDereference((new RLBool(res))->markAsConst());
    }
    |
    NUMBC {
        $$ = new RLDereference((new RLNumber(atoi(&yytext[10])))->markAsConst());
    }
    ;

rcondition:
    OBRACE returnable EBRACE {
        getPrecompilerOutput() << "Only return the condition, RLCycle will be created higher.\n";
        $$ = $2;
    }
    ;


procedure_declaration:
    start_procedure procedure end_procedure {
        getPrecompilerOutput() << "Down the stack and return RLProcedure.\n";
        $$ = new RLDereference(
        RLInterpreter::getCurrentFunction());
        RLInterpreter::downStack();
    }
    ;

start_procedure:
    OFBRACE {
        getPrecompilerOutput() << "Up the stack.\n";
        RLInterpreter::upStack();
    }
    ;
end_procedure:
    EFBRACE {
    }
