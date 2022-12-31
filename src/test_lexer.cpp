#include "lexer.hpp"

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		printf("Error: Wrong number of argument\nUsage: %s \"file to parse\"\n",argv[0]);
		return 1;
	}
	std::vector<Token> tok = lexer(argv[1]);
	for(size_t i = 0; i<tok.size();++i)
	{
		std::cout<<tok[i];
	}
	std::cout<<std::endl;
	return 0;
}