#include "ast.hpp"
#include <vector>

int main()
{
	std::vector<Token> tok;
	
	std::string str = gen_tok_string(tok); 
	std::cout<<str<<std::endl;

	return 0;
}