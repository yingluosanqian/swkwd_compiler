%option noyywrap yylineno case-insensitive

%{
#include <string.h>
#include "ast.h"
#include "swkwd.tab.h"
%}

IDENTIFIER [a-z][a-z0-9]*
OP [+\-*/=()]
NUMBER [0-9]+
WHITESPACE [ \t]
NOTES #[^\n]*
EMPTYLINE ^[ \t]*\n

%%
{OP} { return yytext[0]; }
{NUMBER} { yylval.d = atoi(yytext); return NUMBER;}
{IDENTIFIER} { yylval.s = strdup(yytext); return IDENTIFIER; }
{EMPTYLINE} {}
\n { return EOLN; }
{NOTES} {}
{WHITESPACE} {}
. { yyerror("Unknown character [%c].\n", yytext[0]); }
%%
