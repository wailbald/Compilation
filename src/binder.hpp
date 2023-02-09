#pragma once
#include <unordered_map>
#include "ast.hpp"

typedef std::unordered_map<std::string, Decl*> scope;

class Binder{

	std::vector<scope> scopes;
	std::vector<FunDecl> function;
	std::vector<Loop> loop;
	void scope_push();
	void scope_pop();
	scope actuel();
	void verif(Decl *decl);
	Decl *cherche(std::string);

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
