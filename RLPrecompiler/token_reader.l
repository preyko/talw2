%option noyywrap

%{
#include <stdio.h>

#include "precompiler.hh"
%}
%%

BOOLC,val=[T|F]                 printf("%s",yytext);
NUMBC,val=[1-9][0-9]+           printf("%s",yytext);
NUMBC,val=-[1-9][0-9]+          printf("%s",yytext);


(MARKI,id=[1-9][0-9]+)          printf("%s",yytext);
(PROCI,id=[1-9][0-9]+)          printf("%s",yytext);
(BOOLI,id=[1-9][0-9]+)          printf("%s",yytext);
(NUMBI,id=[1-9][0-9]+)          printf("%s",yytext);

ASSIGN                          printf("%s",yytext);
COMPARE                         printf("%s",yytext);

INC                             printf("%s",yytext);
DEC                             printf("%s",yytext);

NOR                             printf("%s",yytext);

PLEASE                          printf("%s",yytext);

LINK                            printf("%s",yytext);
RLINK                           printf("%s",yytext);

PRINT                           printf("%s",yytext);

OFBRACE                         printf("%s",yytext);
EFBRACE                         printf("%s",yytext);
OSBRACE                         printf("%s",yytext);
ESBRACE                         printf("%s",yytext);
OBRACE                          printf("%s",yytext);
EBRACE                          printf("%s",yytext);
SEMICOLON                       printf("%s",yytext);

[\t ]+                          printf("%s",yytext);
\n                              printf("%s",yytext);

ERRTOKEN\ '%s'\ on\ line\ #[0-9]+.  printf("%s",yytext);
%%
