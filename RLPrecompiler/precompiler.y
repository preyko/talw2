%{

#define YYSTYPE_IS_DECLARED
#define YYSTYPE TokenDescriptor

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

%}

%debug
%start command_c
%token OBRACE EBRACE OFBRACE EFBRACE OSBRACE ESBRACE SEMICOLON // DELIMETRS
BOOLI NUMBI MARKI PROCI // IDENTIFIER
BOOLARRAY NUMBARRAY PROCARRAY // ARRAYS
BOOLC NUMBC // CONSTANT
ASSIGN COMPARE INC DEC LINK RLINK NOR  // OPERATORS
IF
PRINT PRINTALL PLEASE
EOC

// Command for robot
MOVEUP MOVEDOWN MOVELEFT MOVERIGHT TP

// Params of maze and robot
MAZESCHEME      // Represented by a two dimensional array, every element of this array is boolean type. True - occupied place, false - not occupied place.
VISITEDPLACE    // Same as MAZESCHEME, but true - place was visited, vice versa - not.
ROBOPOS         // Represented by 2-elements array, first - X cooardinate, second - Y cooardinate.
SHOWMAZE        // Show maze/

%right COMPARE
%right ASSIGN

%right OFBRACE
%right OBRACE
%right OSBRACE

%%
command_c:
    command_c command
    |
    command_c EOC {
        return 0;
    }
    |
    ;

command:
    returnable SEMICOLON {
        if($1.command!=NULL)
            $1.command->setLinePosition(yylineno);
        RLInterpreter::addCommand($1.command);
    }
    |
    nonreturnable  {
        if($1.command!=NULL)
            $1.command->setLinePosition(yylineno);
        RLInterpreter::addCommand($1.command);
    }
    ;

nonreturnable:
    rcondition command_c_declaration {
        $$.command = new RLCycle($2.command,$1.command);
    }
    |
    IF rcondition command_c_declaration {
        $$.command = new RLConditional($3.command,$2.command);
    }
    |
    rcondition command {
        $$.command = new RLCycle($2.command,$1.command);
    }
    |
    IF rcondition command {
        $$.command = new RLConditional($3.command,$2.command);
    }
    |
    OSBRACE returnable ESBRACE OSBRACE PLEASE ESBRACE OSBRACE MARKI ESBRACE SEMICOLON {
        //collectionerOfExceptions.add(Exception(std::string("Now this doesn't work."),yylineno));
        $$.command = new RLConditional(new RLCommand(maketransition,$8.command),$2.command);
    }
    |
    PRINT returnable SEMICOLON {
        $$.command = new RLCommand(show,$2.command);
    }

returnable:
    ident ASSIGN returnable {
        $$.command = new RLCommand(assign,$1.command,$3.command);
    }
    |
    returnable COMPARE returnable {
        $$.command = new RLCommand(compare,$1.command,$3.command);
    }
    |
    const {
        $$.command = $1.command;
    }
    |
    ident {
        $$.command = $1.command;
    }
    |
    command_c_declaration {
        $$.command = $1.command;
    }
    |
    ident INC {
        $$.command = new RLCommand(increment,$1.command);
    }
    |
    ident DEC {
        $$.command = new RLCommand(decrement,$1.command);
    }
    |
    ident LINK ident {
        $$.command = new RLCommand(bind,$1.command,$3.command);
    }
    |
    ident RLINK ident {
        $$.command = new RLCommand(unbind,$1.command,$3.command);
    }
    |
    robo_commands {
        $$.command = $1.command;
    }
    ;

robo_commands:
    MOVEUP  {
        $$.command = new RLRoboCommands(RLRoboMaze::mup);
    }
    |
    MOVEDOWN {
        $$.command = new RLRoboCommands(RLRoboMaze::mdown);
    }
    |
    MOVELEFT {
        $$.command = new RLRoboCommands(RLRoboMaze::mleft);
    }
    |
    MOVERIGHT {
        $$.command = new RLRoboCommands(RLRoboMaze::mright);
    }
    |
    TP {
        $$.command = new RLRoboCommands(RLRoboMaze::tp);
    }
    ;

ident:
    BOOLI {
        if(RLIdentRegister::get($1.number)==NULL)
            $$.command = new RLDereference(new RLBool(false,$1.number));
        else {
            RLTypePrototype* res = RLIdentRegister::get($1.number);
            if(res->getTypeQualifier() == RLTypePrototype::Bool)
                $$.command = new RLDereference(res);
            else if(res->getTypeQualifier() == RLTypePrototype::Array) {
		RLArray* arr = (RLArray*)res;
		if(arr->getRootQualifier() == RLTypePrototype::Bool) {
                    $$.command = new RLDereference(res);
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
        if(RLIdentRegister::get($1.number)==NULL)
            $$.command = new RLDereference(new RLNumber(0,$1.number));
        else {
            RLTypePrototype* res = RLIdentRegister::get($1.number);
	    if(res->getTypeQualifier() == RLTypePrototype::Number)
                $$.command = new RLDereference(res);
	    else if(res->getTypeQualifier() == RLTypePrototype::Array) {
		RLArray* arr = (RLArray*)res;
		if(arr->getRootQualifier() == RLTypePrototype::Number) {
                    $$.command = new RLDereference(res);
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
        if(RLIdentRegister::get($1.number)==NULL)
            $$.command = new RLDereference(new RLProcedure($1.number));
        else {
            RLTypePrototype* res = RLIdentRegister::get($1.number);
	    if(res->getTypeQualifier() == RLTypePrototype::Procedure)
                $$.command = new RLDereference(res);
	    else if(res->getTypeQualifier() == RLTypePrototype::Array) {
		RLArray* arr = (RLArray*)res;
		if(arr->getRootQualifier() == RLTypePrototype::Procedure) {
                    $$.command = new RLDereference(res);
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
        if(RLIdentRegister::get($1.number)!=NULL)
            collectionerOfExceptions.add(Exception(std::string("This mark declared before."),yylineno));
        else
            //new RLMark(RLInterpreter::getCurrentFunction(),RLInterpreter::getCurrentFunction()->getSize(),$1.number);
            $$.command = NULL;
    }
    |
    array {
        $$.command = $1.command;
    }
    ;

array:
    array_rep {
        if($1.arrayDescriptor->isNew) {
            $1.arrayDescriptor->nArray->setRootDepth($1.arrayDescriptor->rootDepth);
        }
        std::vector<RLCommandPrototype*>& commands = $1.arrayDescriptor->commandForWrapping;

        $$.command = new RLDereference($1.arrayDescriptor->nArray);
        for(int i = 0; i < commands.size(); i++) {
            $$.command = new RLCommand(arrayat,$$.command,commands[i]);
        }

        delete $1.arrayDescriptor;
    }
    ;
    
array_rep:
    array_rep array_end {
        if($1.arrayDescriptor->isNew) {
            $1.arrayDescriptor->rootDepth++;
        }
        $1.arrayDescriptor->commandForWrapping.push_back($2.command);
    }
    |
    array_rep array_index {
        if($1.arrayDescriptor->isNew) {
            $1.arrayDescriptor->rootDepth++;
	}
        $1.arrayDescriptor->commandForWrapping.push_back($2.command);
    }
    |
    array_start {
        if(RLIdentRegister::get($1.arrayDescriptor->id)==NULL) {
            $1.arrayDescriptor->nArray = new RLArray(0,$1.arrayDescriptor->rootType,$1.arrayDescriptor->id);
            $1.arrayDescriptor->isNew = true;
            //collectionerOfExceptions.add(Exception(std::string("Declaration of arrays inside another declaration not allowed."),yylineno));
	} else {
            RLTypePrototype* res = RLIdentRegister::get($1.arrayDescriptor->id);
            //$$.command = new RLDereference(res);
            $1.arrayDescriptor->nArray = res;
            $1.arrayDescriptor->isNew = false;
	}
        $$ = $1;
    }
    ;

array_start:
    BOOLARRAY {
        $$.arrayDescriptor = new ArrayDescriptor;

        $$.arrayDescriptor->id = $1.number;
        $$.arrayDescriptor->rootDepth = 1;
        $$.arrayDescriptor->rootType = RLTypePrototype::Bool;
    }
    |
    NUMBARRAY {
        $$.arrayDescriptor = new ArrayDescriptor;

        $$.arrayDescriptor->id = $1.number;
        $$.arrayDescriptor->rootDepth = 1;
        $$.arrayDescriptor->rootType = RLTypePrototype::Number;
    }
    |
    PROCARRAY {
        $$.arrayDescriptor = new ArrayDescriptor;

        $$.arrayDescriptor->id = $1.number;
        $$.arrayDescriptor->rootDepth = 1;
        $$.arrayDescriptor->rootType = RLTypePrototype::Procedure;
    }
    ;
array_end:
    returnable ESBRACE {
        $$.command = $1.command;
    }
    ;
array_index:
    returnable ESBRACE OSBRACE {
        $$.command = $1.command;
    }
    ;

const:
    BOOLC {
        $$.command = new RLDereference((new RLBool($1.number==1))->markAsConst());
    }
    |
    NUMBC {
        $$.command = new RLDereference((new RLNumber($1.number))->markAsConst());
    }
    ;

rcondition:
    OBRACE returnable EBRACE {
        $$.command = $2.command;
    }
    ;


command_c_declaration:
    start_command_c command_c end_command_c {
        $$.command = RLInterpreter::getCurrentFunction();
        RLInterpreter::downStack();
    }
    ;

start_command_c:
    OFBRACE {
        RLInterpreter::upStack();
    }
    ;
end_command_c:
    EFBRACE {
    }
