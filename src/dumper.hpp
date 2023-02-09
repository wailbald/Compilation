#pragma once

#include <ostream>

#include "ast.hpp"
#include "token.hpp"

class ASTDumper: public Visitor{
  std::ostream *ostream;
  bool verbose;
  unsigned indent_level = 0;
  void inc() { indent_level++; }
  void inl() {
    inc();
    nl();
  };
  void dec() { indent_level--; }
  void dnl() {
    dec();
    nl();
  };

public:
  ASTDumper(std::ostream *_ostream, bool _verbose=true)
      : ostream(_ostream), verbose(_verbose) {}
  void nl() {
    *ostream << std::endl;
    for (unsigned i = 0; i < indent_level; i++)
      *ostream << "  ";
  };
  void visit(IntegerLiteral &);
  void visit(DoubleLiteral &);
  void visit(StringLiteral &);
  void visit(BinaryOperator &);
  void visit(UnaryOperator &);
  void visit(Sequence &);
  void visit(Return &);
  void visit(Identifier &);
  void visit(IfThenElse &);
  void visit(VarDecl &);
  void visit(FunDecl &);
  void visit(FunCall &);
  void visit(WhileLoop &);
  void visit(ForLoop &);
  void visit(Break &);
  void visit(Assign &);
};