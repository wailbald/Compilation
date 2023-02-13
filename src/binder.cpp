#include "binder.hpp"

void Binder::scope_push()
{
	scopes.push_back(scope());
}

void Binder::scope_pop()
{
	scopes.pop_back();
}

scope &Binder::actuel()
{
	return scopes.back();
}

void Binder::verif(Decl *decl)
{
	scope &s = actuel();
	auto precedent = s.find(decl->name);

	if(precedent != s.cend())
	{
		std::cout << decl->name << "already defined" << std::endl;
		exit(5);
	}
	s.insert({decl->name, decl});
}

void Binder::verif_func(Decl *decl)
{
	auto precedent = scopes.begin()->find(decl->name);

	if(precedent != scopes.begin()->cend())
	{
		std::cout << decl->name << "already defined" << std::endl;
		exit(5);
	}

	scopes.begin()->insert({decl->name, decl});
}

Binder::Binder()
{
	scope_push();

	primitif("print_err", t_void, {t_string});
	primitif("print", t_void, {t_string});
	primitif("print_int", t_void, {t_int});
	primitif("print_float", t_void, {t_double});
	primitif("flush", t_void, {});
	primitif("getchar", t_string, {});
	primitif("ord", t_int, {t_string});
	primitif("chr", t_string, {t_int});
	primitif("size", t_int, {t_string});
	primitif("substring", t_string, {t_string, t_int, t_int});
	primitif("concat", t_string, {t_string, t_string});
	primitif("strcmp", t_int, {t_string, t_string});
	primitif("streq", t_int, {t_string, t_string});
	primitif("exit", t_void, {t_int});

}

void Binder::primitif(std::string name, Type type,
			  std::vector<Type> arg)
{
	std::vector<VarDecl *> args;
  	int counter = 0;
  	for (Type tn : arg) 
  	{
    	std::ostringstream argname;
    	argname << "a_" << counter++;
    	args.push_back(
    	new VarDecl((location){0,0}, argname.str(), tn, nullptr));
  	}

  FunDecl *fd = new FunDecl((location){0,0}, name, type, std::move(args), nullptr);

  verif(fd);
}

Decl *Binder::cherche(const location loc, std::string name)
{
	for(auto s = scopes.crbegin(); s != scopes.crend(); s++)
	{
		auto decl = s->find(name);
		if(decl != s->cend())
		{
			return decl->second;
		}
	}
	std::cout << "not found in the scope " << name << std::endl;
	exit(6);
}

void Binder::visit(IntegerLiteral &i)
{

}

void Binder::visit(StringLiteral &str)
{

}

void Binder::visit(DoubleLiteral &dou)
{

}

void Binder::visit(UnaryOperator &un)
{
	un.get_expr()->accept(*this);
}

void Binder::visit(BinaryOperator &bop)
{
	bop.get_left()->accept(*this);
	bop.get_right()->accept(*this);
}

void Binder::visit(Sequence &seq)
{
	std::vector<Expr *> veq = seq.get_exprs();
	for(unsigned long i = 0; i < veq.size(); i++)
	{
    	veq[i]->accept(*this);
	}
}

void Binder::visit(Identifier &id)
{
	VarDecl *var = dynamic_cast<VarDecl*>(cherche(id.loc, id.name));
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

	if(id.get_depth() != var->get_depth())
	{
		var->set_escapes();
	}
}

void Binder::visit(IfThenElse &ite)
{
	scope_push();
	depth++;

	ite.get_condition()->accept(*this);
	ite.get_then()->accept(*this);
	ite.get_else()->accept(*this);

	scope_pop();
	depth--;
}

void Binder::visit(VarDecl &var)
{
	verif(&var);
	var.set_depth(depth);

	if(var.get_expr())
	{
		var.get_expr()->accept(*this);
	}
}

void Binder::visit(FunDecl &f_decl)
{
	function.push_back(f_decl);
	scope_push();
	depth++;

	f_decl.set_depth(depth);
	verif_func(&f_decl);

	for(auto p : f_decl.get_params())
	{
		p->accept(*this);
	}
	f_decl.get_body()->accept(*this);

	scope_pop();
	depth--;
}

void Binder::visit(FunCall &f_call)
{
	FunDecl &f_decl = dynamic_cast<FunDecl&>(*cherche(f_call.loc, f_call.func_name));
	f_call.set_decl(&f_decl);
	auto args = f_call.get_args();
	for(auto arg : args)
	{
		arg->accept(*this);
	}
}

void Binder::visit(WhileLoop &wloop)
{
	scope_push();
	depth++;

	wloop.get_condition()->accept(*this);
	loop.push_back(&wloop);
	wloop.get_body()->accept(*this);
	loop.pop_back();

	scope_pop();
	depth--;
}

void Binder::visit(ForLoop &floop)
{
	scope_push();
	depth++;

	floop.get_variable()->accept(*this);
	floop.get_cond()->accept(*this);
	floop.get_high()->accept(*this);
	loop.push_back(&floop);
	floop.get_body()->accept(*this);
	loop.pop_back();

	scope_pop();
	depth--;
}

void Binder::visit(Break &br)
{
	if(loop.empty())
	{
		std::cout << "Erreur le break est hors d'un if ou d'une boucle" << std::endl;
		exit(10);
	}

	br.set_loop(loop.back());
}

void Binder::visit(Assign &ass)
{
	ass.get_lhs()->accept(*this);
	ass.get_rhs()->accept(*this);
}

void Binder::visit(Return &ret)
{
	if(function.empty())
	{
		std::cout << "Erreur Le return se trouve hors d'une fonction" << std::endl;
		exit(12);
	}
	ret.set_func(&function.back());
	ret.get_expr()->accept(*this);
}