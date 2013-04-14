YACC=bison
LEX=flex
BUILDDIR=build

all: rl.l rl.y
	$(YACC) -d rl.y
	$(LEX) rl.l
	mkdir -p $(BUILDDIR)
	gcc lex.yy.c rl.tab.c -o $(BUILDDIR)/test

clear:
	rm -r build
