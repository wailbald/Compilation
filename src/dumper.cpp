#include "dumper.hpp"

void ASTDumper::visit(IntegerLiteral &literal) {
  *ostream << literal.value;
}

void ASTDumper::visit(DoubleLiteral &literal) {
  *ostream << literal.value;
}

void ASTDumper::visit(StringLiteral &literal) {
  *ostream << '"';
  for (auto &c : static_cast<std::string>(literal.value)) {
    switch (c) {
    case '"':
      *ostream << "\\\"";
      break;
    case '\\':
      *ostream << "\\\\";
      break;
    case '\a':
      *ostream << "\\a";
      break;
    case '\b':
      *ostream << "\\b";
      break;
    case '\t':
      *ostream << "\\t";
      break;
    case '\n':
      *ostream << "\\n";
      break;
    case '\v':
      *ostream << "\\v";
      break;
    case '\f':
      *ostream << "\\f";
      break;
    case '\r':
      *ostream << "\\r";
      break;
    default:
      *ostream << c;
    }
  }
  *ostream << '"';
}

void ASTDumper::visit(BinaryOperator &binop) {
  *ostream << '(';
  binop.get_left()->accept(*this);
  *ostream << operation_name[binop.op];
  binop.get_right()->accept(*this);
  *ostream << ')';
}

void ASTDumper::visit(UnaryOperator &binop) {
  *ostream << '(';
  *ostream << operation_name[binop.op];
  binop.get_expr()->accept(*this);
  *ostream << ')';
}

void ASTDumper::visit(Sequence &seqExpr) {
  *ostream << "{";
  inc();
  auto exprs = seqExpr.get_exprs();
  for (auto expr = exprs.cbegin(); expr != exprs.cend(); expr++) {
    nl();
    (*expr)->accept(*this);
  }
  dnl();
  *ostream << "}";
}

void ASTDumper::visit(Identifier &id) {
  *ostream << id.name;
  if (verbose)
    if (auto decl = id.get_decl()) {
      *ostream << "/*" << "decl:" << decl->loc;

      if (int depth_diff = id.get_depth() - decl->get_depth())
        *ostream << " depth_diff:" << depth_diff;
      *ostream << "*/";
    }
}

void ASTDumper::visit(IfThenElse &ite) {
  *ostream << "if ";
  ite.get_condition()->accept(*this);
  inl();
  ite.get_then()->accept(*this);
  dnl();
  *ostream << " else ";
  inl();
  ite.get_else()->accept(*this);
  dec();
}

void ASTDumper::visit(VarDecl &decl) {
  *ostream <<type_name[decl.type] <<" ";
  *ostream << decl.name;
  if (verbose && decl.get_escapes())
    *ostream << "/*e*/";
  if (auto expr = decl.get_expr()) {
    *ostream << " = ";
    expr->accept(*this);
  }
}

void ASTDumper::visit(FunDecl &decl) {
  *ostream <<type_name[decl.type] <<" " << decl.name;
  *ostream << '(';
  auto params = decl.get_params();
  for (auto param = params.cbegin(); param != params.cend(); param++) {
    if (param != params.cbegin())
      *ostream << ", ";
    (*param)->accept(*this);
  }
  *ostream << ")";
  inl();
  decl.get_body()->accept(*this);
  dec();
}

void ASTDumper::visit(FunCall &call) {
  *ostream << call.func_name;
  if (verbose)
    if (auto decl = call.get_decl())
      *ostream << "/*" << "decl:" << decl->loc << "*/";

  *ostream << "(";

  auto args = call.get_args();
  for (auto arg = args.cbegin(); arg != args.cend(); arg++) {
    if (arg != args.cbegin())
      *ostream << ", ";
    (*arg)->accept(*this);
  }
  *ostream << ')';
}

void ASTDumper::visit(WhileLoop &loop) {
  *ostream << "while ";
  *ostream << "(";
  loop.get_condition()->accept(*this);
  *ostream << ')';
  inl();
  loop.get_body()->accept(*this);
  dec();
  nl();
}

void ASTDumper::visit(ForLoop &loop) {
  *ostream << "for( " ;
  loop.get_variable()->accept(*this);
  *ostream << "; ";
  loop.get_cond()->accept(*this);
  *ostream << "; ";
  loop.get_high()->accept(*this);
  *ostream << ")";
  inl();
  loop.get_body()->accept(*this);
  dec();
  nl();
}

void ASTDumper::visit(Break &brk) {
  *ostream << "break";
  if (verbose && brk.get_loop())
    *ostream << "/*loop:" << brk.get_loop()->loc << "*/";
}

void ASTDumper::visit(Return &brk) {
  *ostream << "return ";
  if (verbose && brk.get_func())
    *ostream << "/*loop:" << brk.get_func()->loc << "*/";
  brk.get_expr()->accept(*this);
}

void ASTDumper::visit(Assign &assign) {
  assign.get_lhs()->accept(*this);
  *ostream << " = ";
  assign.get_rhs()->accept(*this);
}

