#pragma once
#include "token.hpp"
#include "ast_regex.hpp"
#include "lexer.hpp"
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <unordered_map>

class Node;
class IntegerLiteral;
class DoubleLiteral;
class StringLiteral;
class BinaryOperator;
class UnaryOperator;
class Sequence;
class Return;
class Identifier;
class IfThenElse;
class VarDecl;
class FunDecl;
class FunCall;
class WhileLoop;
class ForLoop;
class Break;
class Assign;

class Visitor{
public:
	virtual void visit(IntegerLiteral &) = 0;
  virtual void visit(DoubleLiteral &)= 0;
  virtual void visit(StringLiteral &)= 0;
  virtual void visit(BinaryOperator &)= 0;
  virtual void visit(UnaryOperator &)= 0;
  virtual void visit(Sequence &)= 0;
  virtual void visit(Return &)= 0;
  virtual void visit(Identifier &)= 0;
  virtual void visit(IfThenElse &)= 0;
  virtual void visit(VarDecl &)= 0;
  virtual void visit(FunDecl &)= 0;
  virtual void visit(FunCall &)= 0;
  virtual void visit(WhileLoop &)= 0;
  virtual void visit(ForLoop &)= 0;
  virtual void visit(Break &)= 0;
  virtual void visit(Assign &)= 0;
};

class Tree
{
	public:
		Node *root;
		Tree(){};
		Tree(Node *root_) : root(root_) {};
		Node *get_root(){return this->root;};
};

typedef enum { t_undef = 0, t_int, t_double, t_string, t_void, t_tab } Type;
static std::string type_name[] = {"t_undef","int","double","string","void","tab"};

typedef enum { o_plus, o_minus, o_div, o_mult, o_module, o_lshift, o_rshift, o_and, o_or, o_xor, o_comp, o_not, c_eq, c_neq, c_lt, c_gt, c_le, c_ge, c_and, c_or} Operation;
static std::string operation_name[] = { "+", "-", "/", "*", "%", "<<", ">>", "&", "|", "^", "~", "!", "==", "!=", "<", ">", "<=", ">=", "&&", "||"};

class Node{	
public:
	location loc;
	Node() {};
	Node(location loc_) : loc(loc_){};
	virtual ~Node() {};

	virtual void accept(Visitor &visitor) = 0;
};

class Expr: public Node {
public:
	Expr(): Node() {};
	Expr(location &loc_) : Node(loc_) {};
	virtual ~Expr() {};

	virtual void accept(Visitor &visitor) = 0;
};

class Decl: public Expr {
public:
	std::string name;
	Type type;
	int depth = -1;

	Decl(location loc_,std::string _name,Type _type, int depth_ = -1)
      : Expr(loc_), name(_name), type(_type), depth(depth_) {};
  virtual ~Decl() {};

    void set_depth(int _depth) {
    	if(depth == -1 && _depth != -1)
      		depth = _depth;
  	};

  	int get_depth(){return depth;};
};

class IntegerLiteral : public Expr {
public:
	int32_t value;

	IntegerLiteral(location loc_,int32_t _value)
      : Expr(loc_), value(_value) {};
  void accept(Visitor &visitor) { visitor.visit(*this); }
};

class DoubleLiteral : public Expr {
public:
	double value;

	DoubleLiteral(location loc_,double _value)
      : Expr(loc_), value(_value) {};
	void accept(Visitor &visitor) { visitor.visit(*this); }
};

class StringLiteral : public Expr{
public:
	std::string value;

	StringLiteral(location loc_ ,std::string value_)
      : Expr(loc_), value(value_) {};
  void accept(Visitor &visitor) { visitor.visit(*this); }
};

class BinaryOperator : public Expr {
  Expr *left;
  Expr *right;

public:
  Operation op;

  // Constructor
  BinaryOperator(location _loc, Expr *_left, Expr *_right,Operation _op)
      : Expr(_loc), left(_left), right(_right), op(_op) {};

  // Destructor
  virtual ~BinaryOperator() {
    delete right;
    delete left;
  };

  Expr *get_left(){return left;};
  Expr *get_right(){return right;};

  void accept(Visitor &visitor) { visitor.visit(*this); }
};

class UnaryOperator : public Expr {
  Expr * expr;

public:
  Operation op;

  // Constructor
  UnaryOperator(location _loc, Expr *_expr,Operation _op)
      : Expr(_loc), expr(_expr), op(_op) {};

  // Destructor
  virtual ~UnaryOperator() {
    delete expr;
  };

  Expr *get_expr(){return expr;};

  void accept(Visitor &visitor) { visitor.visit(*this); }
};

  class Sequence : public Expr{
  	std::vector<Expr *> exprs;

  public:
  	Sequence(location _loc, std::vector<Expr *> _exprs)
      : Expr(_loc), exprs(_exprs) {};

  	virtual ~Sequence() {
    for (auto expr : exprs)
      delete expr;
  	}

  	std::vector<Expr *> get_exprs() { return exprs; };

  	void accept(Visitor &visitor) { visitor.visit(*this); }

 };

class Identifier : public Expr {
	Decl *decl = nullptr;
  	int depth = -1;

public:
  	std::string name;

	Identifier(location _loc, std::string _name)
  		: Expr(_loc), name(_name) {};

  	void set_decl(Decl *_decl){
  		if(!decl && _decl)
  			decl = _decl;
  	};

  	Decl* get_decl(){return decl;};

  	void set_depth(int _depth) {
    	if(depth == -1 && _depth != -1)
      	depth = _depth;
  	}
  int get_depth(){return depth;};

  void accept(Visitor &visitor) { visitor.visit(*this); }
};

class IfThenElse : public Expr{
	Expr * condition;
	Expr * then_part;
	Expr * else_part;

public:
	IfThenElse(location _loc, Expr *_condition, Expr *_then_part,Expr *_else_part)
      : Expr(_loc), condition(_condition), then_part(_then_part), else_part(_else_part) {}

    IfThenElse(location _loc, Expr *_condition, Expr *_then_part)
      : Expr(_loc), condition(_condition), then_part(_then_part) {}

    virtual ~IfThenElse() {
	    delete else_part;
	    delete then_part;
	    delete condition;
  	};

  	Expr *get_condition(){return condition;};
  	Expr *get_then(){return then_part;};
  	Expr *get_else(){return else_part;};

  	void accept(Visitor &visitor) { visitor.visit(*this); }

};

class VarDecl : public Decl {
	Expr* expr;
	bool escapes = false;

public:
	VarDecl(location _loc, std::string _name, Type _type_name, Expr* _expr)
	: Decl(_loc, _name, _type_name), expr(_expr){};

	virtual ~VarDecl() { delete expr; };
	Expr *get_expr()
	{
		return expr;
	}

	void set_escapes(){escapes = true;};
	bool get_escapes(){return escapes;};

	void accept(Visitor &visitor) { visitor.visit(*this); }
};

class FunDecl : public Decl {
	std::vector<VarDecl *> params;
	Expr *body;	

public:
	FunDecl(location _loc, std::string _name, Type _type_name, std::vector<VarDecl *> _params, Expr *_expr)
	: Decl(_loc, _name, _type_name), params(_params), body(_expr) {};

	virtual ~FunDecl() {
	    delete body;
	    for (auto param : params)
	      delete param;
  	}

  	std::vector<VarDecl *> &get_params(){return params;};
  	Expr *get_body(){return body;};

  	void accept(Visitor &visitor) { visitor.visit(*this); }


};

class FunCall : public Expr {
	std::vector<Expr *> args;
	FunDecl *decl = nullptr;

public:
	std::string func_name;

	FunCall(location _loc, std::vector<Expr *> _args, std::string _func_name)
	: Expr(_loc), args(_args), func_name(_func_name) {};

	virtual ~FunCall() {
	    for (auto arg : args)
	      delete arg;
  	}

  	std::vector<Expr *> get_args(){return args;};

  	void set_decl(FunDecl *_decl){
  		if(!decl && _decl)
  			decl = _decl;
  	};

  	FunDecl *get_decl(){return decl;};

  	void accept(Visitor &visitor) { visitor.visit(*this); }

};

class Loop : public Expr{
public:
	Loop(location _loc) : Expr(_loc) {};
};

class WhileLoop : public Loop{
	Expr *condition;
	Expr *body;

public:
	WhileLoop(location _loc, Expr *_condition, Expr *_body)
	: Loop(_loc), condition(_condition), body(_body) {};

	virtual ~WhileLoop() {
    	delete body;
    	delete condition;
  	};

  	Expr *get_condition(){return condition;};
  	Expr *get_body(){return body;};

  	void accept(Visitor &visitor) { visitor.visit(*this); }

};

class ForLoop : public Loop {
	Expr *variable;
	Expr *cond;
	Expr *high;
	Expr *body;

public:
	ForLoop(location _loc, Expr *_variable, Expr* _cond,Expr *_high, Expr *_body)
      : Loop(_loc), variable(_variable), cond(_cond),high(_high), body(_body) {};

    virtual ~ForLoop() {
	    delete body;
	    delete cond;
	    delete high;
	    delete variable;
  	}

  	Expr *get_variable() {return variable;};
  	Expr *get_cond() {return cond;};
  	Expr *get_high() {return high;};
  	Expr *get_body(){return body;};

  	void accept(Visitor &visitor) { visitor.visit(*this); }

};

class Break : public Expr {
	Loop * loop = nullptr;
public:
	Break(location _loc) : Expr(_loc){};
	void set_loop(Loop *_loop){
		if(!loop && _loop)
			loop = _loop;
	}

	Loop *get_loop(){return loop;}

	void accept(Visitor &visitor) { visitor.visit(*this); }
};

class Return : public Expr {
	FunDecl * func = nullptr;
	Expr * expr;
public:
	Return(location _loc, Expr* _expr) : Expr(_loc), expr(_expr){};
	void set_func(FunDecl *_func){
		if(!func && _func)
			func = _func;
	}

	FunDecl *get_func(){return func;}
	Expr *get_expr(){return expr;}

	void accept(Visitor &visitor) { visitor.visit(*this); }
};

class Assign : public Expr{
	Identifier *lhs;
	Expr *rhs;
public:
	Assign(location _loc, Identifier *_lhs, Expr *_rhs)
	: Expr(_loc), lhs(_lhs), rhs(_rhs) {};

	virtual ~Assign() {
	    delete rhs;
	    delete lhs;
  	};

  	Identifier *get_lhs(){return lhs;};
  	Expr *get_rhs(){return rhs;};

  	void accept(Visitor &visitor) { visitor.visit(*this); }

};

std::vector<Token> negatif(std::vector<Token> &tok);
int priorite(Token tok);
std::vector<Token> turntoNPI(std::vector<Token> &tok);
Expr * math_expr(std::vector<Token> &tok);
Type get_decl_type(Token tok);

std::vector<VarDecl *> parse_func_params(std::vector<Token> &tok);

std::vector<Token> gen_cond_vect(std::vector<Token> &basetok);
std::vector<Token> gen_body_vect(std::vector<Token> &basetok);
std::vector<Expr *> parse_func_args(std::vector<Token> &tok);
Expr * parse_assign(std::vector<Token> &tok);

Expr *make_return(std::vector<Token> &tok);
Expr *make_break(std::vector<Token> &tok);

Expr * make_identifier(Token tok);
Expr * make_integer_literal(Token tok);
Expr * make_double_literal(Token tok);
Expr * make_string_literal(Token tok);
Expr* make_funcall(std::vector<Token> &tok);
Expr * make_mathematical_expression(std::vector<Token> &tok);

Decl * make_var_decl(std::vector<Token> &tok);
Decl* make_fundecl(std::vector<Token> &tok);

Expr * make_if(std::vector<Token> &tok);
Expr * make_while_loop(std::vector<Token> &tok);
Expr * make_for_loop(std::vector<Token> &tok);

Expr * make_assign(std::vector<Token> &tok);

Expr * parse_token(std::vector<Token> &tok);
Expr * make_seq(std::vector<Token> &tok);
Tree parser(std::vector<Token> &tok);