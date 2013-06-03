%option noyywrap
%option yylineno

%top{
#include <stdio.h>

#include "RLCommand.h"
#include "RLPrecompiler.h"

#include "precompiler.hh"
    
// This just make namespaces invisible for Flex
#define RLNAMESPACE_BEGIN namespace RLPrecompiler {
#define RLNAMESPACE_END }
    
RLNAMESPACE_BEGIN
}
%%

BOOLC,val=[T|F]                 {
    if(yytext[10] == 'T')
        yylval.number = 1;
    else
        yylval.number = 0;
        
    return BOOLC;
}
NUMBC,val=[0-9]+           {
    yylval.number = atoi(&yytext[10]);
    return NUMBC;
}
NUMBC,val=-[0-9]+           {
    yylval.number = atoi(&yytext[10]);
    return NUMBC;
}

PROCI,id=[0-9]+[\t\n ]+OSBRACE             {
    int id;
    sscanf(&yytext[9],"%d",&id);
    yylval.number = id;
    return PROCARRAY;
}
BOOLI,id=[0-9]+[\t\n ]+OSBRACE             {
    int id;
    sscanf(&yytext[9],"%d",&id);
    yylval.number = id;
    return BOOLARRAY;
}
NUMBI,id=[0-9]+[\t\n ]+OSBRACE             {
    int id;
    sscanf(&yytext[9],"%d",&id);
    yylval.number = id;
    return NUMBARRAY;
}

MARKI,id=[0-9]+            {
    yylval.number = atoi(&yytext[9]);
    return MARKI;
}
PROCI,id=[0-9]+            {
    yylval.number = atoi(&yytext[9]);
    return PROCI;
}
BOOLI,id=[0-9]+            {
    yylval.number = atoi(&yytext[9]);
    return BOOLI;
}
NUMBI,id=[0-9]+            {
    yylval.number = atoi(&yytext[9]);
    return NUMBI;
}

ASSIGN                          {
    return ASSIGN;
}
COMPARE                         {
    return COMPARE;
}

INC                             {
    return INC;
}
DEC                             {
    return DEC;
}

NOR                             {
    return NOR;

}

IF                              {
    return IF;
}

PLEASE                          {
    return PLEASE;
}

LINK                            {
    return LINK;
}
RLINK                           {
    return RLINK;
}

PRINT                           {
    return PRINT;
}
PRINTALL                        {
    return PRINTALL;
}

OFBRACE                         {

    return OFBRACE;
}
EFBRACE                         {

    return EFBRACE;
}
OSBRACE                         {

    return OSBRACE;
}
ESBRACE                         {

    return ESBRACE;
}
OBRACE                          {

    return OBRACE;
}
EBRACE                          {

    return EBRACE;
}
SEMICOLON                       {

    return SEMICOLON;
}

MOVEUP                          return MOVEUP;
MOVEDOWN                        return MOVEDOWN;
MOVELEFT                        return MOVELEFT;
MOVERIGHT                       return MOVERIGHT;
TP                              return TP;
MAZESCHEME                      return MAZESCHEME;
VISITEDPLACE                    return VISITEDPLACE;
ROBOPOS                         return ROBOPOS;

SHOWMAZE                       	return SHOWMAZE;

[\t ]+                          {

}
\n                              {

 }

ERRTOKEN\ '%s'\ on\ line\ #[0-9]+\. {
    
}

<<EOF>>                         {
    return EOC;
}
%%
RLNAMESPACE_END
