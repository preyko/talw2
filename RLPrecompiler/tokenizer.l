%option noyywrap

%top{
#include <stdio.h>

#include "RLTokenizer.h"

// This just make namespaces invisible for Flex
#define RLNAMESPACE_BEGIN namespace RLTokenizer {
#define RLNAMESPACE_END }

RLNAMESPACE_BEGIN
FILE* token_output;
}

%x LINE_COMMENT MULTILINE_COMMENT
%%
<INITIAL>
{
	/* Constants */
T|F                     fprintf(token_output,"BOOLC,val=%s ",yytext);
[0-9]+                  fprintf(token_output,"NUMBC,val=%s ",yytext);
\-[0-9]+                fprintf(token_output,"NUMBC,val=%s ",yytext);

	/* Variable identifier */
\~[0-9]+                fprintf(token_output,"MARKI,id=%s ",&yytext[1]);
\$[0-9]+                fprintf(token_output,"PROCI,id=%s ",&yytext[1]);
\.[0-9]+                fprintf(token_output,"BOOLI,id=%s ",&yytext[1]);
\,[0-9]+                fprintf(token_output,"NUMBI,id=%s ",&yytext[1]);

	/* Operators */
\<\-                    fprintf(token_output,"ASSIGN ");
eq                      fprintf(token_output,"COMPARE ");

\,\#[0-9]+              fprintf(token_output,"NUMBI,id=%s INC ",&yytext[2]);
\,\*[0-9]+              fprintf(token_output,"NUMBI,id=%s DEC ",&yytext[2]);

\.\#[0-9]+              fprintf(token_output,"BOOLI,id=%s NOR ",&yytext[2]);

\|                      fprintf(token_output,"IF ");

please                  fprintf(token_output,"PLEASE ");

\@                      fprintf(token_output,"LINK ");
\%                      fprintf(token_output,"RLINK ");

print                   fprintf(token_output,"PRINT ");



	/* DELIMETRS */
\{                      fprintf(token_output,"OFBRACE ");
\}                      fprintf(token_output,"EFBRACE ");
\[                      fprintf(token_output,"OSBRACE ");
\]                      fprintf(token_output,"ESBRACE ");
\(                      fprintf(token_output,"OBRACE ");
\)                      fprintf(token_output,"EBRACE ");
;                       fprintf(token_output,"SEMICOLON ");

\n                      yylineno++; fprintf(token_output,"\n");
[ \t]+                  fprintf(token_output," ");

    /* ERRTOKEN */
 .                      fprintf(token_output,"ERRTOKEN '%s' on line #%d. ",yytext,yylineno);

    /* Commentaries */
\/\/                    BEGIN(LINE_COMMENT);
\/\*                    BEGIN(MULTILINE_COMMENT);
}

<LINE_COMMENT>
{
.                       // Skip all, except end of line
\n                      BEGIN(INITIAL);
}

<MULTILINE_COMMENT>
{
.                       // Skip all, except '/*'
\*\/                    BEGIN(INITIAL);
}

%%
RLNAMESPACE_END

namespace RLTokenizer {
    void Tokenize(FILE* input, FILE* output) {
        yyin = input;
        token_output = output;

        yylex();
    }

    void Tokenize(const char* input, const char* output) {
        FILE* in;
        FILE* out;

        in = fopen(input,"r");
        out = fopen(output,"w");

        if(in == NULL)
            throw Exception(std::string("Can't open file called ") +
                            std::string(input));
        if(out == NULL)
            throw Exception(std::string("Can't create file with name ") +
                            std::string(output));

        Tokenize(in,out);

        fclose(in);
        fclose(out);
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
