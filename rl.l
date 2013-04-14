%{
#include <stdio.h>
#include "rl.tab.h"
%}
%%
[0-9]+                  return NUMBER;
heat                    return TOKHEAT;
on|off                  return STATE;
target                  return TOKTARGET;
temperature             return TOKTEMPERATURE;
\n                      /* игнорируем символ конца строки */;
[ \t]+                  /* игнорируем пробелы и символы табуляции */;
%%

int main() {
    yyin = fopen("tests.txt","r");
    yylex();

    return 0;
}
