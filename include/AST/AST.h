#pragma once

#include <vector>

class Decl;
class TranslationUnitDecl;
class RuleDecl;
class FunctionDecl;
class VarDecl;

class Stmt;
class BreakStmt;
class ContinueStmt;
class NextStmt;
class NextfileStmt;
class ForStmt;
class IfStmt;
class WhileStmt;
class DoStmt;
class DeclStmt;
class ExprStmt;
class CompoundStmt;
class ValueStmt;
class Expr;
class BinaryExpr;
class CallExpr;
class DeclRefExpr;
class FloatingLiteral;
class StringLiteral;
class UnaryExpr;

class Decl {
public:
  enum DeclKind { DK_TranslationUnit, DK_Rule, DK_Function, DK_Var };

private:
  const DeclKind Kind;

protected:
  Decl(DeclKind K) : Kind(K) {}

public:
  DeclKind GetKind() const { return Kind; }
};

class TranslationUnitDecl : public Decl {
  std::vector<Decl *> Decls;

protected:
  TranslationUnitDecl() : Decl(DK_TranslationUnit) {}
  TranslationUnitDecl(std::vector<Decl *> D)
      : Decl(DK_TranslationUnit), Decls(D) {}

public:
  static TranslationUnitDecl *Create(std::vector<Decl *> Decls) {
    return new TranslationUnitDecl(Decls);
  }
  static TranslationUnitDecl *CreateEmpty() { return new TranslationUnitDecl; }

  const std::vector<Decl *> &GetDecls() const { return Decls; }
};

class FunctionDecl : public Decl {
  std::string Name;
  std::vector<std::string> Params;
  CompoundStmt *Body;

protected:
  FunctionDecl() : Decl(DK_Function) {}

  FunctionDecl(std::string N, std::vector<std::string> P, CompoundStmt *B)
      : Decl(DK_Function), Name(N), Params(P), Body(B) {}

public:
  static FunctionDecl *Create(std::string N, std::vector<std::string> P,
                              CompoundStmt *B) {
    return new FunctionDecl(N, P, B);
  }

  static FunctionDecl *CreateEmpty() { return new FunctionDecl; }
};

class RuleDecl : public Decl {
  Expr *Pattern;
  CompoundStmt *Action;

protected:
  RuleDecl() : Decl(DK_Rule) {}

  RuleDecl(Expr *P, CompoundStmt *A) : Decl(DK_Rule), Pattern(P), Action(A) {}

public:
  static RuleDecl *Create(Expr *Pattern, CompoundStmt *Action) {
    return new RuleDecl(Pattern, Action);
  }
};

class Stmt {
public:
  enum StmtKind {
    SK_Break,
    SK_Continue,
    SK_Next,
    SK_Nextfile,
    SK_For,
    SK_If,
    SK_While,
    SK_Do,
    SK_Decl,
    SK_Expr,
    SK_Compound
  };

private:
  const StmtKind Kind;

protected:
  Stmt(StmtKind K) : Kind(K) {}

public:
  StmtKind GetKind() const { return Kind; }
};

class CompoundStmt : public Stmt {
  std::vector<Stmt *> Body{};

protected:
  CompoundStmt() : Stmt(SK_Compound) {}

  CompoundStmt(std::vector<Stmt *> B) : Stmt(SK_Compound), Body(B) {}

public:
  static CompoundStmt *Create(std::vector<Stmt *> B) {
    return new CompoundStmt(B);
  }

  static CompoundStmt *CreateEmpty() { return new CompoundStmt; }

  std::vector<Stmt *> GetBody() const { return Body; }
};

class IfStmt : public Stmt {
  Expr *Cond;
  Stmt *Then;
  Stmt *Else;

protected:
  IfStmt() : Stmt(SK_If) {}

  IfStmt(Expr *C, Stmt *T, Stmt *E) : Stmt(SK_If), Cond(C), Then(T), Else(E) {}

public:
  static IfStmt *Create(Expr *Cond, Stmt *Then, Stmt *Else) {
    return new IfStmt(Cond, Then, Else);
  }
};

class ForStmt : public Stmt {
  Stmt *Init;
  Expr *Cond;
  Expr *Inc;
  Stmt *Body;

protected:
  ForStmt() : Stmt(SK_For) {}

  ForStmt(Stmt *It, Expr *C, Expr *I, Stmt *B)
      : Stmt(SK_For), Init(It), Cond(C), Inc(I), Body(B) {}

public:
  static ForStmt *Create(Stmt *Init, Expr *Cond, Expr *Inc, Stmt *Body) {
    return new ForStmt(Init, Cond, Inc, Body);
  }
};

class BreakStmt : public Stmt {
protected:
  BreakStmt() : Stmt(SK_Break) {}

public:
  static BreakStmt *Create() { return new BreakStmt; }
};

class ContinueStmt : public Stmt {
protected:
  ContinueStmt() : Stmt(SK_Continue) {}

public:
  static ContinueStmt *Create() { return new ContinueStmt; }
};

class NextStmt : public Stmt {
protected:
  NextStmt() : Stmt(SK_Next) {}

public:
  static NextStmt *Create() { return new NextStmt; }
};

class NextfileStmt : public Stmt {
protected:
  NextfileStmt() : Stmt(SK_Nextfile) {}

public:
  static NextfileStmt *Create() { return new NextfileStmt; }
};
