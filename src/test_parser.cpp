#include "lexer.hpp"
#include "dumper.hpp"
#include "ast.hpp"
#include "type_checker.hpp"
#include "binder.hpp"


int main(int argc, char** argv)
{
	if(argc != 2)
	{
		printf("Error: Wrong number of argument\nUsage: %s \"file to parse\"\n",argv[0]);
		return 1;
	}
	std::vector<Token> tok = lexer(argv[1]);
	Tree prog = parser(tok);
	Binder bind;
	TypeChecker checker;
	ASTDumper dumper(&std::cout);
	prog.root->accept(bind);
	prog.root->accept(checker);
	prog.root->accept(dumper);
	std::cout<<std::endl;

	return 0;
}