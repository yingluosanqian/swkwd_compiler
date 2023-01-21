extern int yylineno;

// 类型定义
#define T int

// token 编号
#define ID 258
#define NUM 259
#define NEG 260

// 与 lexer 的接口
void yyerror(char *s, ...);

// identifier 符号表
struct IdentifierSymbol {
    char *name;
    T val;
};
#define NHASH 9997
struct IdentifierSymbol symtab[NHASH];
struct IdentifierSymbol *getSym(char *s);
void setSym(char *s, T val);

// 抽象语法树
typedef struct AbstractSyntaxTree {
  int tokenType;
  struct AbstractSyntaxTree *left;
  struct AbstractSyntaxTree *right;
} *Ast;

typedef struct Statement {
  Ast ast;
  struct Statement* nxt;
} *Stmt;

struct Number {
  int tokenType;
  T val;
};

struct Identifier {
  int tokenType;
  char *val;
};

Ast newAst(int nodetype, Ast left, Ast right);
Ast newNum(T num);
Ast newId(char* s);
Stmt newStmt(Ast ast);
Stmt mergeStmt(Stmt stmt1, Stmt stmt2);

void evalStmt(Stmt);
void evalAst(Ast);

void freeAst(Ast);
void freeStmt(Ast);
