swkwd: swkwd_lexer.l swkwd_parser.y ast.h
       bison -d swkwd_parser.y
       flex -o swkwd_lexer.lex.c swkwd_lexer.l
       gcc -o $@ swkwd_parser.tab.c swkwd_lexer.lex.c ast.c