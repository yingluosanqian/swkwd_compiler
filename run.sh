bison -d swkwd.y
flex -oswkwd.lex.c swkwd.l
gcc -o swkwd swksw.tab.c swkwd.lex.c ast.c