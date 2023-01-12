#include "lexer.hpp"

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		printf("Error: Wrong number of argument\nUsage: %s \"file to parse\"\n",argv[0]);
		return 1;
	}
	std::vector<Token> tok = lexer(argv[1]);
	std::string str = gen_tok_string(tok); 
	std::cout<<str<<std::endl;

	return 0;
}