#include "ast.hpp"

Tree create_assign(std::vector<Token> tok, int i)
{
	int nb = 0;
	while(tok[nb].get_type() != ASSIGN)
	{
		nb++;
	}
	Node n(tok[i+nb]);

	Tree tree(&n);
	if(nb == 2)
	{
		Node n1(tok[i+1]);
		Node n2(tok[i+2]);

		tree.get_root()->add_left(&n1);

		Node *tmp = tree.get_root();
		tmp = tmp->get_left();
		tmp->add_left(&n2);

		std::cout << token_name[tree.get_root()->get_left()->get_tok().get_type()] << std::endl;
		std::cout << tree.get_root()->get_left()->get_tok().get_text() << std::endl;

		std::cout<<std::endl;

		std::cout << token_name[tree.get_root()->get_tok().get_type()] << std::endl;
	}
	if(nb == 1)
	{
		Node n1(tok[i+1]);
		tree.get_root()->add_left(&n1);
		std::cout << "dans celui ci" << std::endl;
	}

	std::cout << "fini" << std::endl;

	return tree;
}