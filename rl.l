%{
#include <stdio.h>
#include "rl.hh"
%}

%%
test			return TEST;

	/* Constants */
T|F			if(yytext[0]=='T') yylval=1; else yylval=0; return BOOLCONST;
[1-9][0-9]+		yylval=atoi(yytext); return NUMBCONST;
\-[1-9][0-9]+		yylval=atoi(&yytext[1]); return NUMBCONST;

	/* Variable identifier */
,[0-9]+			printf("BOOLIDNT"); yylval=atoi(&yytext[1]); return BOOLIDNT;
.[0-9]+			printf("NUMBIDNT"); yylval=atoi(&yytext[1]); return NUMBIDNT;

	/* Operators */
\<\-			printf("ASSIGOPER"); return ASSIGOPER;
eq			printf("ASSIGOPER"); return EQOPER;

\-\-			printf("INCOPER"); return INCOPER;
\+\+			printf("DECOPER"); return DECOPER;



	/* DELIMETRS */
\{                      printf("OFBRACE"); return OFBRACE;
\}                      printf("EFBRACE"); return EFBRACE;
\(                      printf("OBRACE"); return OBRACE;
\)                      printf("EBRACE"); return EBRACE;
;                       printf("SEMICOLON"); return SEMICOLON;

\n                      printf("");/* пропускаем концы строк */;
[ \t]+                  printf("");/* пропускаем пробелы */;
%%
