#include "ast.hpp"

int assign_check_type(Token t)
{
	if(t.get_type() == ID || t.get_type() == STR || t.get_type() == CHAR
	|| t.get_type() == INT|| t.get_type() == DOUBLE)
		return 1;

	if(t.get_type() == PLUS || t.get_type() ==  MINUS||t.get_type() ==  TIMES||t.get_type() == DIVIDE 
	|| t.get_type() == MODULE|| t.get_type() == LSHIFT|| t.get_type() == RSHIFT ||t.get_type() == XOR)
		return 2;
		
	if(t.get_type() == LPAREN || t.get_type() == RPAREN)
		return 3;

	if(t.get_type() == EQ || t.get_type() == NEQ || t.get_type() == LT || t.get_type() == GT ||
		t.get_type() == LE || t.get_type() == GE || t.get_type() == CAND || t.get_type() == COR)
		return 4;
		
	std::cout << "Certains symbole de l'assignation ne sont pas pris en charge" << std::endl;
	exit(3);
}

void verif_assign(std::vector<Token> tok, int i, int nb)
{
	int paren = 0;
	
	i += nb;
	
	if(nb == 2)
	{
		if(tok[i-nb].get_type() != DECL || tok[i-1].get_type() != ID)
		{
			std::cout << "Erreur lors de la déclaration de variable" << std::endl;
			std::cout << "La déclaration de variable doit être de la forme 'type' 'nom' '=' 'valeur'" << std::endl;
			exit(2);
		}
		
		if(tok[i-1].get_type() != ID)
		{
			std::cout << "Erreur lors de l'assignation de variable" << std::endl;
			std::cout << "L'assignation de variable doit être de la forme 'nom' '=' 'valeur'" << std::endl;
			exit(2);
		}
	}
	
	if(tok[i].get_type() == LPAREN)
		paren ++;
		
	if(tok[i].get_type() == RPAREN)
		paren --;
		
	if(paren < 0)
	{
		std::cout<< "Problème de parenthèse, une parenthèse fermante n'est pas précédé d'une parenthèse ouvrante" << std::endl;
	}
		
	if(tok[i+1].get_type() == LPAREN)
		paren ++;
		
	if(tok[i+1].get_type() == RPAREN)
		paren --;
		
	if(paren < 0)
	{
		std::cout<<"Problème de parenthèse, une parenthèse fermante n'est pas précédé d'une parenthèse ouvrante" << std::endl;
	}
	
	i += 2;
	
	while(tok[i+1].get_type() != SEMICOLON)
	{
		if(tok[i].get_type() == LPAREN)
			paren ++;
		
		if(tok[i].get_type() == RPAREN)
			paren --;
			
		if(paren < 0)
		{
			std::cout<<"Problème de parenthèse, une parenthèse fermante n'est pas précédé d'une parenthèse ouvrante" << std::endl;
			exit(4);
		}
		
		if(assign_check_type(tok[i]) == assign_check_type(tok[i-1]) 
			|| assign_check_type(tok[i]) == assign_check_type(tok[i+1])
			|| (assign_check_type(tok[i]) == 4 && assign_check_type(tok[i-1]) == 2)
			|| (assign_check_type(tok[i]) == 4 && assign_check_type(tok[i+1]) == 2))
		{
			std::cout << "problème dans l'assignation, vous ne pouvez pas utiliser ces symboles à la suites" << std::endl;
		}
		i++;
	}
}

Tree create_assign(std::vector<Token> tok, int i)
{
	//vérification du nombre de caractère avant le symbole d'assignation
	int nb1 = i;
	while(tok[nb1].get_type() != ASSIGN)
	{
		nb1++;
	}
	
	verif_assign(tok,i,i-nb1);

	int nb = i - nb1;
	Node n(tok[i+nb]);

	Tree tree(&n);
	
	//création de la partie gauche de l'arbre
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
	
	//création de la partie droite de l'arbre



	std::cout << "fini" << std::endl;

	return tree;
}
