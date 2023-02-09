#pragma once
#include <unordered_map>
#include "ast.hpp"

typedef std::unordered_map<std::string, Decl*> scope;

class Binder: public Visitor{

	std::vector<scope> scopes;
	std::vector<FunDecl> function;
	std::vector<Loop *> loop;
	int depth = 0;
	void scope_push();
	void scope_pop();
	scope actuel();
	void verif(Decl *decl);
	Decl *cherche(const location loc, std::string);

public:
	Binder();
	void visit(IntegerLiteral &i);
	void visit(StringLiteral &str);
	void visit(DoubleLiteral &dou);
	void visit(UnaryOperator &un);
	void visit(BinaryOperator &bop);
	void visit(Sequence &seq);
	void visit(Identifier &id);
	void visit(IfThenElse &ite);
	void visit(VarDecl &var);
	void visit(FunDecl &f_decl);
	void visit(FunCall &f_call);
	void visit(WhileLoop &wloop);
	void visit(ForLoop &floop);
	void visit(Break &br);
	void visit(Assign &ass);
	void visit(Return &ret);
};
