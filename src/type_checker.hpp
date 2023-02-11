#pragma once

#include <ostream>

#include "ast.hpp"
#include "token.hpp"

class TypeChecker: public INTVisitor{
public:
  TypeChecker() {}
  int visit(IntegerLiteral &);
  int visit(DoubleLiteral &);
  int visit(StringLiteral &);
  int visit(BinaryOperator &);
  int visit(UnaryOperator &);
  int visit(Sequence &);
  int visit(Return &);
  int visit(Identifier &);
  int visit(IfThenElse &);
  int visit(VarDecl &);
  int visit(FunDecl &);
  int visit(FunCall &);
  int visit(WhileLoop &);
  int visit(ForLoop &);
  int visit(Break &);
  int visit(Assign &);
};