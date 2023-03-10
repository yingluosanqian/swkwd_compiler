#ifndef __AST_C
#define __AST_C

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "ast.h"

// 哈希
static unsigned hash(char *s) {
  unsigned int hash = 0;
  unsigned c;
  while (c = *s++) hash = hash * 9 ^ c;
  return hash;
}

static struct IdentifierSymbol symtab[NHASH];

// 查询变量是否存在
struct IdentifierSymbol* getSym(char *s) {
  struct IdentifierSymbol *res = &symtab[hash(s) % NHASH];
  int scount = NHASH;
  while (--scount >= 0) {
    if (res->name && !strcmp(res->name, s)) {
      return res;
    }
    if (!res->name) {
      return NULL;
    }
    if(++res >= symtab + NHASH) res = symtab;
  }
  yyerror("symbol table overflow.");
  abort();
}

// 修改变量的值
void setSym(char *s, T val) {
  struct IdentifierSymbol *res = &symtab[hash(s) % NHASH];
  int scount = NHASH;
  while (--scount >= 0) {
    if (res->name && !strcmp(res->name, s)) {
      res->val = val;
      return;
    }
    if (!res->name) {
      res->name = strdup(s);
      res->val = val;
      return;
    }
    if (++res >= symtab + NHASH) res = symtab;
  }
  yyerror("symbol table overflow.");
  abort();
}

// 输出符号表
void emitSymbol() {
  int i;
  for (i = 0; i < NHASH; i++) {
    if (symtab[i].name != 0) {
      printf("%s %d\n", symtab[i].name, symtab[i].val);
    }
  }
}

// 构造抽象语法树
Ast newAst(int tokenType, Ast left, Ast right) {
  Ast res = malloc(sizeof(struct AbstractSyntaxTree));
  if (!res) {
    yyerror("Insufficient memory space.");
    exit(0);
  }

  res->tokenType = tokenType;
  res->left = left;
  res->right = right;
  return res;
}

Ast newNum(T val) {
  struct Number *res = malloc(sizeof(struct Number));
  if (!res) {
    yyerror("Insufficient memory space.");
    exit(0);
  }

  res->tokenType = NUM;
  res->val = val;
  return (Ast)res;
}

Ast newId(char* val) {
  struct Identifier *res = malloc(sizeof(struct Identifier));
  if (!res) {
    yyerror("Insufficient memory space.");
    exit(0);
  }

  res->tokenType = ID;
  res->val = strdup(val);
  return (Ast)res;
}

Stmt newStmt(Ast ast) {
  Stmt res = malloc(sizeof(struct Statement));
  if (!res) {
    yyerror("Insufficient memory space.");
    exit(0);
  }

  res->ast = ast;
  res->nxt = NULL;
  return res;
}

Stmt mergeStmt(Stmt stmt1, Stmt stmt2) {
  stmt2->nxt = stmt1;
  return stmt2;
}

void freeAst(Ast ast) {
  if (ast == NULL) return;
  // did not finish
  // too lazy to do it
}

void freeStmt(Stmt stmt) {
  if(stmt == NULL) return;
  freeAst(stmt->ast);
  freeStmt(stmt->nxt);
  free(stmt);
}

// 遍历抽象语法树
T evalAst(Ast ast) {
  // printf("token: %d\n", ast->tokenType);
  T v = 0, val_r;
  char *name;
  switch (ast->tokenType) {
    case NUM:
      v = ((struct Number *)ast)->val;
      break;
    case ID:;
      name = ((struct Identifier *)ast)->val;
      struct IdentifierSymbol *id = getSym(name);
      if(id == NULL) {
        yyerror("Undefined identifier %s.", name);
        abort();
      } else {
        v = id->val;
      }
      break;
    case NEG:
      v = -evalAst(ast->left);
      break;
    case '+':
      v = evalAst(ast->left) + evalAst(ast->right);
      break;
    case '-':
      v = evalAst(ast->left) - evalAst(ast->right);
      break;
    case '*':
      v = evalAst(ast->left) * evalAst(ast->right);
      break;
    case '/':
      val_r = evalAst(ast->right);
      if (val_r == 0) {
        yyerror("Divide by 0.");
        abort();
      }
      v = evalAst(ast->left) / evalAst(ast->right);
      break;
    case '=':
      val_r = evalAst(ast->right);
      name = ((struct Identifier *)(ast->left))->val;
      setSym(name, val_r);
      val_r = evalAst(ast->left);
      break;
    default:
      yyerror("Unknown token [%d].", ast->tokenType);
  }
  return v;
}

// 遍历抽象语法树
void evalStmt(Stmt stmt) {
    if(stmt->nxt) {
        evalStmt(stmt->nxt);
    }
    evalAst(stmt->ast);
}

#endif
