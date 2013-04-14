YACC=BISON
LEX=FLEX

all: rl.l rl.y
    $(YACC) -d rl.y
    $(LEX) rl.l
    mkdir build
    cc lex.yy.c y.tab.c -o build/test

clear:
    rm -r build
