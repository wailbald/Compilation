#include "lexer.hpp"
#include "dumper.hpp"
#include "ast.hpp"


int main(int argc, char** argv)
{
	if(argc != 2)
	{
		printf("Error: Wrong number of argument\nUsage: %s \"file to parse\"\n",argv[0]);
		return 1;
	}
	std::vector<Token> tok = lexer(argv[1]);
	Tree prog = parser(tok);
	ASTDumper dumper(&std::cout);
	prog.root->accept(dumper);
	std::cout<<std::endl;

	return 0;
}