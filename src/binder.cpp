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

void verif(Decl *decl)
{
	scope s = actuel();
	auto precedent = s.find(decl->name);

	if(precedent != s.end())
	{
		std::cout << decl->name << "already defined" << std::endl;
		exit(5);
	}
	s[decl->name] = decl;
}

Decl *cherche(std::string name)
{
	for(auto s = scopes.crbegin(); s != scopes.crend(); s++)
	{
		auto decl = s->find(name);
		if(decl != s->cend())
		{
			return decl->second;
		}
	}
	std::cout << "function " << name << std::endl;
	exit(6);
}