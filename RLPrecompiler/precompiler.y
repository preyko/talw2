%{
#define YYSTYPE RLCommandPrototype*

YYSTYPE yyval;
extern int yydebug;
extern int yylineno;
extern char* yytext;

extern int yyparse(void);
extern int yywrap(void);
extern int yylex(void);

extern FILE* yyin;

void yyerror(const char* str) {
        collectionerOfExceptions.add(Exception(std::string(str),yylineno));
}

int yywrap() {
        return 1;
}

void Precompile(const char* token_input) {
    collectionerOfExceptions.clear();
    
    yyin = fopen(token_input,"r");

    if(yyin == NULL)
        collectionerOfExceptions.add(Exception(std::string("Can't open file called ") +
                        std::string(token_input)));

    yylineno = 1;
    #ifdef EPDOUT
    yydebug = 1;
    #elseif
    yydebug = 0;
    #endif
    yydebug = 1;
    
    RLInterpreter::Initialization();

    do {
        yyparse();
    } while(!feof(yyin));
    
    fclose(yyin);
    
    if(outputStreamOpenedHere_) {
        std::ofstream* outfstream = dynamic_cast<std::ofstream*>(logstream);
        if(outfstream != NULL) {
            outfstream->close();
            delete outfstream;
            outfstream = NULL;
        }
    }

    if(!collectionerOfExceptions.isEmpty()) {
        throw collectionerOfExceptions;
    } else {
	getPrecompilerOutput() << "\nPrecompilation completed successfully!\n";
    }
}

struct ArrayInfo {
    int rootDepth;
    RLArray* nArray;
    RLTypePrototype::RLTypeQualifier rootType;
};

ArrayInfo arrayInfo_{0,NULL,RLTypePrototype::Number};

%}

%debug
%start procedure
%token OBRACE EBRACE OFBRACE EFBRACE OSBRACE ESBRACE SEMICOLON // DELIMETRS
BOOLI NUMBI MARKI PROCI // IDENTIFIER
BOOLARRAY NUMBARRAY PROCARRAY // ARRAYS
BOOLC NUMBC // CONSTANT
ASSIGN COMPARE INC DEC LINK RLINK NOR  // OPERATORS
IF
PRINT PRINTALL PLEASE
EOC

%right COMPARE
%right ASSIGN

%right OFBRACE
%right OBRACE
%right OSBRACE

%%
procedure:
    procedure command
    |
    procedure EOC {
        return 0;
    }
    |
    ;

command:
    returnable SEMICOLON {
        if($1!=NULL)
            $1->setLinePosition(yylineno);
        RLInterpreter::addCommand($1);
    }
    |
    nonreturnable  {
        if($1!=NULL)
            $1->setLinePosition(yylineno);
        RLInterpreter::addCommand($1);
    }
    ;

nonreturnable:
    rcondition procedure_declaration {
        $$ = new RLCycle(new RLCommand(perform,$2),$1);
    }
    |
    IF rcondition procedure_declaration {
        $$ = new RLConditional(new RLCommand(perform,$3),$2);
    }
    |
    rcondition command {
	$$ = new RLCycle($2,$1);
    }
    |
    IF rcondition command {
	$$ = new RLConditional($3,$2);
    }
    |
    OSBRACE returnable ESBRACE OSBRACE PLEASE ESBRACE OSBRACE MARKI ESBRACE SEMICOLON {
        //collectionerOfExceptions.add(Exception(std::string("Now this doesn't work."),yylineno));
        $$ = new RLConditional(new RLCommand(maketransition,$8),$2);
    }
    |
    PRINT returnable SEMICOLON {
        $$ = new RLCommand(show,$2);
    }
    |
    PRINTALL SEMICOLON {
	$$ = new RLPrintAll();
    }
    ;

returnable:
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
    procedure_declaration {
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
        if(RLIdentRegister::get(int($1))==NULL)
            $$ = new RLDereference(new RLBool(false,int($1)));
        else {
            RLTypePrototype* res = RLIdentRegister::get(int($1));
            if(res->getTypeQualifier() == RLTypePrototype::Bool)
                $$ = new RLDereference(res);
            else if(res->getTypeQualifier() == RLTypePrototype::Array) {
		RLArray* arr = (RLArray*)res;
		if(arr->getRootQualifier() == RLTypePrototype::Bool) {
		    $$ = new RLDereference(res);
		} else
		    collectionerOfExceptions.add(Exception(std::string("Expected array of type RL") +
                                       RLTypePrototype::typeName(res->getTypeQualifier()),yylineno));
	    } else 
                //TODO Reform this exception message!!!
                collectionerOfExceptions.add(Exception(std::string("Expected RL") +
                                                       RLTypePrototype::typeName(res->getTypeQualifier()),yylineno));
        }
    }
    |
    NUMBI {
        if(RLIdentRegister::get(int($1))==NULL)
            $$ = new RLDereference(new RLNumber(0,int($1)));
        else {
	    RLTypePrototype* res = RLIdentRegister::get(int($1));
	    if(res->getTypeQualifier() == RLTypePrototype::Number)
		$$ = new RLDereference(res);
	    else if(res->getTypeQualifier() == RLTypePrototype::Array) {
		RLArray* arr = (RLArray*)res;
		if(arr->getRootQualifier() == RLTypePrototype::Number) {
		    $$ = new RLDereference(res);
		} else
		    collectionerOfExceptions.add(Exception(std::string("Expected array of type RL") +
				       RLTypePrototype::typeName(res->getTypeQualifier()),yylineno));
	    } else 
		//TODO Reform this exception message!!!
		collectionerOfExceptions.add(Exception(std::string("Expected RL") +
						       RLTypePrototype::typeName(res->getTypeQualifier()),yylineno));
        }
    }
    |
    PROCI {
        if(RLIdentRegister::get(int($1))==NULL)
            $$ = new RLDereference(new RLProcedure(int($1)));
        else {
	    RLTypePrototype* res = RLIdentRegister::get(int($1));
	    if(res->getTypeQualifier() == RLTypePrototype::Procedure)
		$$ = new RLDereference(res);
	    else if(res->getTypeQualifier() == RLTypePrototype::Array) {
		RLArray* arr = (RLArray*)res;
		if(arr->getRootQualifier() == RLTypePrototype::Procedure) {
		    $$ = new RLDereference(res);
		} else
		    collectionerOfExceptions.add(Exception(std::string("Expected array of type RL") +
				       RLTypePrototype::typeName(res->getTypeQualifier()),yylineno));
	    } else 
		//TODO Reform this exception message!!!
		collectionerOfExceptions.add(Exception(std::string("Expected RL") +
						       RLTypePrototype::typeName(res->getTypeQualifier()),yylineno));
        }
    }
    |
    MARKI {
        if(RLIdentRegister::get(int($1))!=NULL)
            collectionerOfExceptions.add(Exception(std::string("This mark declared before."),yylineno));
        else
            new RLMark(RLInterpreter::getCurrentFunction(),RLInterpreter::getCurrentFunction()->getSize(),int($1));
            $$ = NULL;
    }
    |
    array {
	$$ = $1;
    }
    ;

array:
    array_rep {
	if(arrayInfo_.nArray != NULL) {
	    arrayInfo_.nArray->setRootDepth(arrayInfo_.rootDepth);
	    arrayInfo_.nArray = NULL;
	} else {
	    $$ = $1;
	}
    }
    ;
    
array_rep:
    array_rep array_end {
	if(arrayInfo_.nArray != NULL) {
	    arrayInfo_.rootDepth++;
	}
	$$ = new RLCommand(arrayat,$1,$2);
    }
    |
    array_rep array_index {
	if(arrayInfo_.nArray != NULL) {
	    arrayInfo_.rootDepth++;
	}
        $$ = new RLCommand(arrayat,$1,$2);
    }
    |
    array_start {
	if(RLIdentRegister::get((int)$1)==NULL) {
	    if(arrayInfo_.nArray == NULL) {
		arrayInfo_.rootDepth = 1;
		arrayInfo_.nArray = new RLArray(0,arrayInfo_.rootType,(int)$1);
		$$ = new RLDereference(arrayInfo_.nArray);
	    } else {
		collectionerOfExceptions.add(Exception(std::string("Declaration of arrays inside another declaration not allowed."),yylineno));
	    }
	} else {
	    RLTypePrototype* res = RLIdentRegister::get((int)$1);
	    $$ = new RLDereference(res);
	}
	//$$ = new RLCommand(arrayat,$$,$2);
    }
    ;

array_start:
    BOOLARRAY {
	arrayInfo_.rootType = RLTypePrototype::Bool;
        $$ = $1;
    }
    |
    NUMBARRAY {
	arrayInfo_.rootType = RLTypePrototype::Number;
        $$ = $1;
    }
    |
    PROCARRAY {
	arrayInfo_.rootType = RLTypePrototype::Procedure;
        $$ = $1;
    }
    ;
array_end:
    returnable ESBRACE {
	$$ = $1;
    }
    ;
array_index:
    returnable ESBRACE OSBRACE {
	$$ = $1;
    }
    ;

const:
    BOOLC {
        $$ = new RLDereference((new RLBool((int)$1==1))->markAsConst());
    }
    |
    NUMBC {
        $$ = new RLDereference((new RLNumber((int)$1))->markAsConst());
    }
    ;

rcondition:
    OBRACE returnable EBRACE {
        $$ = $2;
    }
    ;


procedure_declaration:
    start_procedure procedure end_procedure {
        $$ = new RLDereference(
        RLInterpreter::getCurrentFunction());
        RLInterpreter::downStack();
    }
    ;

start_procedure:
    OFBRACE {
        RLInterpreter::upStack();
    }
    ;
end_procedure:
    EFBRACE {
    }
