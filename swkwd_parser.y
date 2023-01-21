%{
#include <stdio.h>
#include <stdlib.h>
#include <ast.h>
%}

%union {
Ast a;
Stmt st;
char* s;
T d;
}

%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%token <d> NUMBER
%token EOLN FINISH
%token <s> IDENTIFIER

%type <st> stmts stmt
%type <a> expr

%%
program : stmts FINISH {
  evalStmt($1);
  freeStmt($1);
}

stmts : stmt { $$ = $1); }
  | stmts stmt { $$ = mergeStmt($1, $2); }
;
stmt : IDENTIFIER '=' expr EOLN { $$ = newStmt(newAst('=', $1, $3)); }
;

expr : expr '+' expr { $$ = newAst('+', $1, $3); }
  | expr '-' expr { $$ = newAst('-', $1, $3); }
  | '(' expr ')' { $$ = $2; }
  | '-' expr %prec UMINUS {$$ = newast(NEG, $2, NULL); }
  | IDENTIFIER { $$ = newId($1); }
  | NUMBER { $$ = newNum($1); }
;
%%

void yyerror(char*s, ...) {
  va_list ap;
  va_start(ap,s);
  fprintf(stderr, "%d: error: ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: swkwd_compiler [inputfile]");
  } else {
    yyin = argv[1];
    re
  }
  return 0;
}
