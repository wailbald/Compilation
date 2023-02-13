#include "type_checker.hpp"

int TypeChecker::visit(IntegerLiteral &literal) {
  return t_int;
}

int TypeChecker::visit(DoubleLiteral &literal) {
  return t_double;
}

int TypeChecker::visit(StringLiteral &literal) {
  return t_string;
}

int TypeChecker::visit(BinaryOperator &binop) {
  
  int left_type = binop.get_left()->accept(*this);
  int right_type = binop.get_right()->accept(*this);
  if(right_type != t_int && right_type != t_double)
  {
    printf("Error: Binary operation can't process type %s\n",type_name[right_type].c_str());
    exit(5);
  }
  if(left_type != t_int && left_type != t_double)
  {
    printf("Error: Binary operation can't process type %s\n",type_name[left_type].c_str());
    exit(5);
  }  

  if(right_type == left_type)
  {
    if(binop.op == c_eq || binop.op == c_neq || binop.op == c_lt || binop.op == c_gt || binop.op == c_le || binop.op == c_ge || binop.op == c_and || binop.op == c_or)
    {
      return t_int;
    }
    return left_type;
  }
  else
  {
    printf("Error: Binary operator can't compute %s with %s\n",type_name[left_type].c_str(),type_name[right_type].c_str());
    exit(5);
  }
  return left_type;
}

int TypeChecker::visit(UnaryOperator &binop) {
  int type = binop.get_expr()->accept(*this);
  if(type != t_int)
  {
    printf("Error: Unary operation can't process type %s\n",type_name[type].c_str());
    exit(5);
  }
  return type;
}

int TypeChecker::visit(Sequence &seqExpr) {
  auto exprs = seqExpr.get_exprs();
  for (auto expr = exprs.cbegin(); expr != exprs.cend(); expr++) {
    (*expr)->accept(*this);
  }
  return t_void;
}

int TypeChecker::visit(Identifier &id) {
  auto decl = id.get_decl();
  return decl->type;
}

int TypeChecker::visit(IfThenElse &ite) {
  int cond_type = ite.get_condition()->accept(*this);
  if(cond_type != t_int)
  {
    printf("Error: if condition must be an integer\n");
    exit(5);
  }
  ite.get_then()->accept(*this);
  ite.get_else()->accept(*this);
  return t_void;
}

int TypeChecker::visit(VarDecl &decl) {
  if (auto expr = decl.get_expr()) {
    expr->accept(*this);
  }
  return decl.type;
}

int TypeChecker::visit(FunDecl &decl) {
  auto params = decl.get_params();
  for (auto param = params.cbegin(); param != params.cend(); param++) {
    (*param)->accept(*this);
  }
  decl.get_body()->accept(*this);
  return t_void;
}

int TypeChecker::visit(FunCall &call) {
  auto decl = call.get_decl();
  auto args = call.get_args();

  if(decl->get_params().size() != args.size())
  {
    printf("Error: Wrong argument number in fucall: passing %lu, expected %lu\n",args.size(),decl->get_params().size());
    exit(5);
  }
  auto params = decl->get_params();
  for(size_t i = 0; i<params.size();i++)
  {
    int arg_type = args[i]->accept(*this);
    int decl_type = params[i]->type;
    if(arg_type != decl_type)
    {
      printf("Error: wrong type fot funcall parameters: expected %s got %s",type_name[decl_type].c_str(),type_name[arg_type].c_str());
      exit(5);
    }
  }
  return decl->type;
}

int TypeChecker::visit(WhileLoop &loop) {
  int cond_type = loop.get_condition()->accept(*this);
  if(cond_type != t_int)
  {
    printf("Error: while condition must be an integer\n");
  }
  loop.get_body()->accept(*this);
  
  return t_void;
}

int TypeChecker::visit(ForLoop &loop) {
  loop.get_variable()->accept(*this);
  
  int cond_type = loop.get_cond()->accept(*this);
  if(cond_type != t_int)
  {
    printf("Error: for condition must be an integer\n");
  }

  loop.get_high()->accept(*this);
  loop.get_body()->accept(*this);

  return t_void;
}

int TypeChecker::visit(Break &brk) {
  return t_void;
}

int TypeChecker::visit(Return &brk) {
  int func_type = brk.get_func()->type;
  int expr_type;
  if(brk.get_expr() == NULL) expr_type = t_void;
  else expr_type = brk.get_expr()->accept(*this);
  
  if(func_type != expr_type)
  {
    printf("Error: %s function cannot return a %s",type_name[func_type].c_str(),type_name[expr_type].c_str());
    exit(5);
  }
  return expr_type;
}

int TypeChecker::visit(Assign &assign) {
  int id_type = assign.get_lhs()->accept(*this);
  int rhs_type = assign.get_rhs()->accept(*this);
  if(id_type != rhs_type)
  {
    printf("Error: can't assign %s to an %s variable\n",type_name[rhs_type].c_str(),type_name[id_type].c_str());
    exit(5);
  }
  return id_type;
}

