%{
#include <stdio.h>

#include "rl.hh"

FILE* token_output;

%}


%%
	/* Constants */
T|F			fprintf(token_output,"BOOLCONST"); if(yytext[0]=='T') yylval=1; else yylval=0; return BOOLCONST;
[1-9][0-9]+		fprintf(token_output,"NUMBCONST"); yylval=atoi(yytext); return NUMBCONST;
\-[1-9][0-9]+		fprintf(token_output,"NUMBCONST"); yylval=atoi(&yytext[1]); return NUMBCONST;

	/* Variable identifier */
,[0-9]+			fprintf(token_output,"BOOLIDNT"); yylval=atoi(&yytext[1]); return BOOLIDNT;
.[0-9]+			fprintf(token_output,"NUMBIDNT"); yylval=atoi(&yytext[1]); return NUMBIDNT;

	/* Operators */
\<\-			fprintf(token_output,"ASSIGOPER"); return ASSIGOPER;
eq			fprintf(token_output,"EQOPER"); return EQOPER;

\-\-			fprintf(token_output,"INCOPER"); return INCOPER;
\+\+			fprintf(token_output,"DECOPER"); return DECOPER;

print_			fprintf(token_output,"PRINTOPER"); return PRINTOPER;



	/* DELIMETRS */
\{                      fprintf(token_output,"OFBRACE"); return OFBRACE;
\}                      fprintf(token_output,"EFBRACE"); return EFBRACE;
\(                      fprintf(token_output,"OBRACE"); return OBRACE;
\)                      fprintf(token_output,"EBRACE"); return EBRACE;
;                       fprintf(token_output,"SEMICOLON"); return SEMICOLON;

\n                      fprintf(token_output,"");/* пропускаем концы строк */;
[ \t]+                  fprintf(token_output,"");/* пропускаем пробелы */;
%%
