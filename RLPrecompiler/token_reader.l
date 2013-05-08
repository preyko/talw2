%option noyywrap
%option yylineno

%top{
#include <stdio.h>

#include "precompiler.hh"

// This just make namespaces invisible for Flex
#define RLNAMESPACE_BEGIN namespace RLPrecompiler {
#define RLNAMESPACE_END }

RLNAMESPACE_BEGIN
}
%%

BOOLC,val=[T|F]                 {
    return BOOLC;
}
NUMBC,val=[0-9]+           {
    return NUMBC;
}
NUMBC,val=-[0-9]+           {
    return NUMBC;
}


MARKI,id=[0-9]+            {
    return MARKI;
}
PROCI,id=[0-9]+            {

    return PROCI;
}
BOOLI,id=[0-9]+            {

    return BOOLI;
}
NUMBI,id=[0-9]+            {

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

[\t ]+                          {

}
\n                              {

 }

ERRTOKEN\ '%s'\ on\ line\ #[0-9]+. {

}

<<EOF>>                         {
    return EOf;
}
%%
RLNAMESPACE_END
