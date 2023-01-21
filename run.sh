bison -d swkwd.y
flex -oswkwd.lex.c swkwd.l
gcc -o swkwd swkwd.tab.c swkwd.lex.c ast.c