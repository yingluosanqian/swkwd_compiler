%option noyywrap nodefault yylineno case-insensitive

%{
#include "ast.h"
#include "swkwd.tab.h"
%}

IDENTIFIER [a-z][a-z0-9]+
OP [+-*/=]
NUMBER [0-9]+
WHITESPACE [ \t]
NOTES #[^\n]*
EMPTYLINE ^[ \t]*\n

%%
OP { return yytext[0]; }

NUMBER { yylval.d = atoi(yytext); return NUMBER;}

IDF { yylval.s = yytext; return IDENTIFIER; }

\n { return EOLN; }

NOTES {}

EMPTYLINE {}

<<EOF>> { return FINISH; }

WHITESPACE {}

. { yyerror("Unknown character [%c].\n", yytext[0]); }

%%
