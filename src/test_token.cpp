#include "ast.hpp"
#include <vector>

int main()
{
	std::vector<Token> tok;
	tok.push_back(Token(VARDECL,(location){1,1,1},"int"));
	tok.push_back(Token(ID,(location){1,2,2},"a"));
	tok.push_back(Token(ASSIGN,(location){1,3,3}));
	tok.push_back(Token(INT,(location){1,4,4},"1"));
	tok.push_back(Token(SEMICOLON,(location){1,5,5}));

	std::cout<<"et ici"<<tok[1]<<std::endl;

	for(size_t i = 0; i<tok.size(); i++)
	{
		std::cout<<tok[i];
	}
	std::cout<<std::endl;

	create_assign(tok,0);

	return 0;
}