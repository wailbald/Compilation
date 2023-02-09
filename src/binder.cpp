#include "binder.hpp"

void scope_push()
{
	scopes.push_back(scope());
}

void scope_pop()
{
	scopes.pop_back();
}

scope actuel()
{
	return scopes.back();
}

void verif(Decl decl)
{
	scope s = actuel();
	auto precedent = s.find(decl.name);

	if(precedent != s.end())
	{
		std::cout << decl.name << "already defined" << std::endl;
		exit(5);
	}
	s[decl.name] = &decl;
}

Binder::Binder()
{
	scope_push();
}

Decl cherche(std::string name)
{
	for(auto s = scopes.crbegin(); s != scopes.crend(); s++)
	{
		auto decl = s->find(name);
		if(decl != s->cend())
		{
			return *decl->second;
		}
	}
	std::cout << "function " << name << std::endl;
	exit(6);
}

void visit(IntegerLiteral &i)
{

}

void visit(StringLiteral &str)
{

}

void visit(DoubleLiteral &dou)
{

}

void visit(UnaryOperator &un)
{
	un.get_expr()->accept(*this);
}

void visit(BinaryOperator &bop)
{
	bop.get_left()->accept(*this);
	bop.get_right()->accept(*this);
}

void visit(Sequence &seq)
{
	for(auto iterator = seq.exprs.begin(); iterator != seq.exprs.end(); ++iterator)
	{
    	iterator->accept(*this);
	}
}

void visit(Identifier &id)
{
	VarDecl *var = dynamic_cast<VarDecl*>(&find(id.loc, id.name));
	if(!var)
	{
		std::cout << "erreur la variable n'est pas declaree dans ce scope ou a ete define deux fois" << std::endl;
		exit(9);
	}
	else
	{
		id.set_decl(var);
		id.set_depth(depth);
	}

	if(id->get_depth() != var.get_depth())
	{
		var->set_escape();
	}
}

void visit(IfThenElse &ite)
{
	scope_push();
	depth++;

	ite.get_condition()->accept(*this);
	ite.get_then_part()->accept(*this);
	ite.get_else_part()->accept(*this);

	scope_pop();
	depth--;
}

void visit(VarDecl &var)
{
	if(var.get_expr())
	{
		var.get_expr()->accept(*this);
	}
	verif(var);
	var.det_depth(depth)
}

void visit(FunDecl &f_decl)
{
	function.push_back(&f_decl);
	scope_push();
	depth++;

	for(auto p : f_decl.get_params())
	{
		p->accept(*this);
	}
	f_decl.get_expr()->accept(*this);

	scope_pop();
	depth--;
	function.pop_back();
}

void visit(FunCall &f_call)
{
	FunDecl &f_decl = dynamic_cast<FunDecl&>(find(f_call.loc, f_call.func_name));
	f_call.set_decl(&f_decl);
	f_call.set_depth(depth);

	for(auto arg : f_call.get_args())
	{
		arg->accept(*this);
	}
}

void visit(WhileLoop &wloop)
{
	scope_push();
	depth++;

	wloop.get_condition().accept(*this);
	loop.push_back(wloop);
	wloop.get_body().accept(*this);
	loop.pop_back()

	scope_pop();
	depth--;
}

void visit(ForLoop &floop)
{
	scope_push();
	depth++;

	floop.get_variable().accept(*this);
	floop.get_high().accept(*this);
	loop.push(back(floop));
	floop.get_body().accept(*this);
	loop.pop_back();

	scope_pop();
	depth--;
}

void visit(Break &br)
{
	if(loop.empty())
	{
		std::cout << "Erreur le break est hors d'un if ou d'une boucle" << std::endl;
		exit(10);
	}

	br.set_loop(loop.back());
}

void visit(Assign &ass)
{
	ass.get_lhs().accept(*this);
	ass.get_rhs().accept(*this);

	if(ass.get_lhs().get_decl()->read_only)
	{
		std::cout << "Erreur assignation a une variable de lecture" <<std::endl;
		exit(11);
	}
}

void visit(Return &ret)
{
	if(function.empty())
	{
		std::cout << "Erreur Le return se trouve hors d'une fonction" << std::endl;
		exit(12);
	}
	ret.set_func(function.back());
}