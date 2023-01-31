#pragma once
#include "token.hpp"
#include "ast_regex.hpp"
#include "lexer.hpp"
#include <vector>
#include <cstdint>
#include <cstdlib>

class Node
{
	Token tok;
	Node *lson;
	Node *rson;

	public:
		Node(Token tok_, Node *lson_, Node *rson_) : tok(tok_), lson(lson_), rson(rson_){};
		Node(Token tok_) : tok(tok_), lson(NULL), rson(NULL){};
		Node *get_right(){return this->rson;};
		Node *get_left(){return this->lson;};
		Token get_tok(){return this->tok;};
		void add_tok(Token t){this->tok = t;};
		void add_left(Node *l){this->lson = l;};
		void add_right(Node *r){this->rson = r;};
};

class Tree
{
	Node *root;

	public:
		Tree(){};
		Tree(Node *root_) : root(root_) {};
		Node *get_root(){return this->root;};
};

int assign_check_type(Token t);
int verif_assign(std::vector<Token> tok, int i);
Tree create_assign(std::vector<Token> tok, int i);


typedef enum { t_undef = 0, t_int, t_double, t_string, t_void, t_tab } Type;
static std::string type_name[] = {"t_undef","int","string","void","tab"};

typedef enum { o_plus, o_minus, o_div, o_mult, o_module, o_lshift, o_rshift, o_and, o_or, o_xor, o_comp, c_eq, c_neq, c_lt, c_gt, c_le, c_ge, c_and, c_or} Operation;
static std::string operation_name[] = { "+", "-", "/", "*", "%", "<<", ">>", "&", "|", "^", "~", "==", "!=", "<", ">", "<=", ">=", "&&", "||"};

class visitor{};

class NewNode{	
public:
	location loc;
	NewNode() {};
	NewNode(location loc_) : loc(loc_){};

	void accept(visitor v);
};

class Expr: public NewNode {
public:
	Expr(): NewNode() {};
	Expr(location &loc_) : NewNode(loc_) {};
};

class Decl: public Expr {
public:
	std::string name;
	Type type;
	int depth = -1;

	Decl(location loc_,std::string _name,Type _type, int depth_ = -1)
      : Expr(loc_), name(_name), type(_type), depth(depth_) {};

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

};

class DoubleLiteral : public Expr {
public:
	double value;

	DoubleLiteral(location loc_,double _value)
      : Expr(loc_), value(_value) {};

};

class StringLiteral : public Expr{
public:
	std::string value;

	StringLiteral(location loc_ ,std::string value_)
      : Expr(loc_), value(value_) {};

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
};

class FunDecl : public Decl {
	std::vector<VarDecl *> params;
	Expr *body;
	std::string name;

public:
	FunDecl(location _loc, std::string _name, Type _type_name, std::vector<VarDecl *> _params, Expr *_expr)
	: Decl(_loc, _name, _type_name), params(_params), body(_expr) {};

	virtual ~FunDecl() {
	    delete body;
	    for (auto param : params)
	      delete param;
  	}

  	std::vector<VarDecl *> &get_params(){return params;};
  	Expr get_body(){return *body;};

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
};

class ForLoop : public Loop {
	Expr *variable;
	Expr *high;
	Expr *body;

public:
	ForLoop(location _loc, VarDecl *_variable, Expr *_high, Expr *_body)
      : Loop(_loc), variable(_variable), high(_high), body(_body) {};

    virtual ~ForLoop() {
	    delete body;
	    delete high;
	    delete variable;
  	}

  	Expr *get_variable() {return variable;};
  	Expr *get_high() {return high;};
  	Expr *get_body(){return body;};
};

class Break : public Expr {
	Loop * loop = nullptr;
public:
	Break(location _loc) : Expr(_loc){};
	void set_loop(Loop *_loop){
		if(!loop && _loop)
			loop = _loop;
	}

	Loop &get_loop(){return *loop;}
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
};

std::vector<Token> gen_cond_vect(std::vector<Token> basetok);
std::vector<Token> gen_body_vect(std::vector<Token> basetok);

Expr * make_identifier(Token tok);
Expr * make_integer_literal(Token tok);
Expr * make_double_literal(Token tok);
Expr * make_string_literal(Token tok);
Decl * make_var_decl(std::vector<Token> tok);

Expr* make_if(std::vector<Token> tok);

Expr *make_while_loop(std::vector<Token> tok);
Expr *make_for_loop(std::vector<Token> tok);

Expr * parse_assign(std::vector<Token> tok);
Expr * make_assign(std::vector<Token> tok);

Expr * parse_token(std::vector<Token> tok);
Expr * parse_seq(std::vector<Token> tok);
Tree parser(std::vector<Token> tok);
