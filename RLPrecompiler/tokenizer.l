%option noyywrap

%top{
#include <fstream>

#include "RLTokenizer.h"

// This just make namespaces invisible for Flex
#define RLNAMESPACE_BEGIN namespace RLTokenizer {
#define RLNAMESPACE_END }


namespace RLTokenizer {
std::ostream* tokenOutput_ = NULL;

std::ostream& getTokenizerOutput() {
    if(tokenOutput_ == NULL)
        return std::cout;
    else
        return *tokenOutput_;
}

bool streamOpenedHere_ = false;

void setTokenizerOutput(std::ostream& ostr) {
    streamOpenedHere_ = false;
    tokenOutput_ = &ostr;
}

void setTokenizerOutput(std::string filename) {
    streamOpenedHere_ = true;
    tokenOutput_ = new std::ofstream(filename.c_str());
}

Exceptions exceptions_;
}

RLNAMESPACE_BEGIN
}

%x LINE_COMMENT MULTILINE_COMMENT ERRTOKEN
%%
<INITIAL>
{
	/* Constants */
T|F                     getTokenizerOutput() << "BOOLC,val=" << yytext << " ";
[0-9]+                  getTokenizerOutput() << "NUMBC,val=" << yytext << " ";
\-[0-9]+                getTokenizerOutput() << "NUMBC,val=" << yytext << " ";

	/* Variable identifier */
\~[0-9]+                getTokenizerOutput() << "MARKI,id=" << &yytext[1] << " ";
\$[0-9]+                getTokenizerOutput() << "PROCI,id=" << &yytext[1] << " ";
\.[0-9]+                getTokenizerOutput() << "BOOLI,id=" << &yytext[1] << " ";
\,[0-9]+                getTokenizerOutput() << "NUMBI,id=" << &yytext[1] << " ";

	/* Operators */
\<\-                    getTokenizerOutput() << "ASSIGN ";
eq                      getTokenizerOutput() << "COMPARE ";

\,\#[0-9]+              getTokenizerOutput() << "NUMBI,id=" << &yytext[2] << " INC ";
\,\*[0-9]+              getTokenizerOutput() << "NUMBI,id=" << &yytext[2] << " DEC ";

\.\#[0-9]+              getTokenizerOutput() << "BOOLI,id=" << &yytext[2] << " NOR ";

\|                      getTokenizerOutput() << "IF ";

please                  getTokenizerOutput() << "PLEASE ";

\@                      getTokenizerOutput() << "LINK ";
\%                      getTokenizerOutput() << "RLINK ";

print                   getTokenizerOutput() << "PRINT ";
printall                getTokenizerOutput() << "PRINTALL ";

        /* Robocommands */
rup                     getTokenizerOutput() << "MOVEUP ";
rdown                   getTokenizerOutput() << "MOVEDOWN ";
rleft                   getTokenizerOutput() << "MOVELEFT ";
rright                  getTokenizerOutput() << "MOVERIGHT ";
rtp                     getTokenizerOutput() << "TP ";
rmaze                   getTokenizerOutput() << "MAZESCHEME ";
vp                      getTokenizerOutput() << "VISITEDPLACE ";
rposition               getTokenizerOutput() << "ROBOPOS ";

show_maze               getTokenizerOutput() << "SHOWMAZE ";

	/* DELIMETRS */
\{                      getTokenizerOutput() << "OFBRACE ";
\}                      getTokenizerOutput() << "EFBRACE ";
\[                      getTokenizerOutput() << "OSBRACE ";
\]                      getTokenizerOutput() << "ESBRACE ";
\(                      getTokenizerOutput() << "OBRACE ";
\)                      getTokenizerOutput() << "EBRACE ";
;                       getTokenizerOutput() << "SEMICOLON ";

\n                      yylineno++; getTokenizerOutput() << "\n";
[ \t]+                  getTokenizerOutput() << " ";

    /* ERRTOKEN */
.                       exceptions_.add(Exception(yytext,yylineno)); BEGIN(ERRTOKEN);

    /* Commentaries */
\/\/                    BEGIN(LINE_COMMENT);
\/\*\/\*                BEGIN(MULTILINE_COMMENT);
}

<ERRTOKEN>
{
[ ]                     BEGIN(INITIAL);
.                       {
                            std::cout << exceptions_.getCollectioner().size() << std::endl;
                            exceptions_.getLast().what() += yytext;
                        }
}

<LINE_COMMENT>
{
.                       // Skip all, except end of line
\n                      yylineno++; BEGIN(INITIAL);
}

<MULTILINE_COMMENT>
{
.                       // Skip all, except '/*'
\n                      yylineno++;
\*\/                    BEGIN(INITIAL);
}

%%
RLNAMESPACE_END

namespace RLTokenizer {

void Tokenize(const char* input) {
    yyin = fopen(input,"r");

    if(yyin == NULL)
        throw Exception(std::string("Can't open file called ") +
                        std::string(input));

    exceptions_.clear();
    
    do {
        yylex();
    } while(!feof(yyin));
    
    if(streamOpenedHere_) {
        std::ofstream* tmp = dynamic_cast<std::ofstream*>(tokenOutput_);
        if(tmp != NULL) {
            tmp->close();
            delete tmp;
            tmp = NULL;
        }
    } else {
        std::ofstream* tmp = dynamic_cast<std::ofstream*>(tokenOutput_);
        if(tmp != NULL) {
            tmp->close();
        }
    }

    fclose(yyin);
    
    if(!exceptions_.isEmpty())
        throw exceptions_;
}
}


#ifdef MAIN
int main(const int argn,const char** arg) {
    FILE* in;
    FILE* out;
    if(argn >= 2) {
        in = fopen(arg[1],"r");

        if(in == NULL) {
            fprintf(stderr,"ERROR: Can't open %s as RLCode file.\n",arg[1]);
            return 1;
        }
    }
    if(argn >= 3) {
        out = fopen(arg[2],"w");

        if(out == NULL) {
            fprintf(stderr,"ERROR: Can't open %s for token out file.\n",arg[2]);
            return 1;
        }
    } else {
        out = fopen("token_output.txt","w");
    }

    RLTokenizer::Tokenize(in,out);

    return 0;
}
#endif
