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

int verif_assign(std::vector<Token> tok, int i, int nb, int *taille)
{
	int max = 0;
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
		
	if(max < paren)
		max = paren;
		
	if(tok[i].get_type() == RPAREN)
		paren --;
		
	if(paren < 0)
	{
		std::cout<< "Problème de parenthèse, une parenthèse fermante n'est pas précédé d'une parenthèse ouvrante" << std::endl;
	}
		
	if(tok[i+1].get_type() == LPAREN)
		paren ++;
		
	if(max < paren)
		max = paren;
		
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
		
		if(max < paren)
			max = paren;
		
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
		taille++;
	}
	
	return max;
}

Tree create_assign(std::vector<Token> tok, int i)
{
	//vérification du nombre de caractère avant le symbole d'assignation
	int max = 0;
	int *taille = 0;
	
	int nb1 = i;
	while(tok[nb1].get_type() != ASSIGN)
	{
		nb1++;
	}
	
	max = verif_assign(tok,i,i-nb1,taille);

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
	}
	
	//création de la partie droite de l'arbre

	int tmp = i+(*taille);
	int paren = 0;

	for(int j = 0; j < max; j++)
	{
		while(tmp != i)
		{
			if(tok[tmp].get_type() == LPAREN)
				paren ++;
				
			if(tok[tmp].get_type() == RPAREN)
				paren --;
			
			if((tok[tmp].get_type() == PLUS || tok[tmp].get_type() == MINUS) && paren == j)
			{
				
			}
			
			tmp --;
		}
		
		tmp = i+(*taille);
		
		while(tmp != i)
		{
			if(tok[tmp].get_type() == LPAREN)
				paren ++;
				
			if(tok[tmp].get_type() == RPAREN)
				paren --;
			
			if((tok[tmp].get_type() == TIMES || tok[tmp].get_type() == DIVIDE) && paren == j)
			{
				
			}
			
			tmp --;
		}
	}

	std::cout << "fini" << std::endl;

	return tree;
}

std::vector<Token> gen_cond_vect(std::vector<Token> basetok)
{
	int paren_depth = 1;
	std::vector<Token> cond_tok;
	for(auto elem : basetok)
	{
		if(elem.get_type() == LPAREN)
			paren_depth++;
		if(elem.get_type() == RPAREN)
			paren_depth--;
		if(!paren_depth)
			return cond_tok;
		cond_tok.push_back(elem);
	}
	printf("Error: Unexpected End Of File\n");
	exit(6);
	return cond_tok;
}

std::vector<Token> gen_body_vect(std::vector<Token> basetok)
{
	int brckt_depth = 0;
	bool in_bracket = false;
	std::vector<Token> body_tok;
	for(auto elem : basetok)
	{
		if(elem.get_type() == SEMICOLON)
		{
			if(in_bracket)
			{
				printf("Error: Unexpected SEMICOLON token\n");
				exit(6);
			}
			else
			{
				body_tok.push_back(elem);
				return body_tok;
			}
		}
		if(elem.get_type() == LBRCKT)
		{
			brckt_depth++;
			in_bracket = true;
		}	
			
		if(elem.get_type() == RBRCKT)
			brckt_depth--;
		if(!brckt_depth)
			return body_tok;
		body_tok.push_back(elem);
		
	}
	printf("Error: Unexpected End Of File\n");
	exit(6);
	return body_tok;
}

Expr * make_identifier(Token tok)
{
	return new Identifier(tok.get_loc(),tok.get_text());
}

Expr * make_integer_literal(Token tok)
{
	return new IntegerLiteral(tok.get_loc(),atol(tok.get_text().c_str()));
}

Expr * make_string_literal(Token tok)
{
	return new StringLiteral(tok.get_loc() ,tok.get_text());
}

Expr* make_if(std::vector<Token> tok)
{
	location if_location=tok[0].get_loc();
	tok.erase(tok.begin(),tok.begin()+1);
	
	std::smatch match;
	std::vector<Token> cond_tok = gen_cond_vect(tok);
	Expr *cond_part = parse_token(cond_tok);
	tok.erase(tok.begin(),tok.begin()+cond_tok.size()+1);
	
	std::vector<Token> body_tok = gen_body_vect(tok);
	Expr *body_part = parse_token(body_tok);
	tok.erase(tok.begin(),tok.begin()+body_tok.size()+1);

	std::string line = gen_tok_string(tok);
	line.insert(0,"#/#");
	if(std::regex_search(line,match,pELSEPART_regex))
	{
		std::vector<Token> else_tok = gen_body_vect(tok);
		Expr *else_part = parse_token(else_tok);
		tok.erase(tok.begin(),tok.begin()+else_tok.size());

		IfThenElse* ite = new IfThenElse(if_location,cond_part,body_part,else_part);
		return ite;
	}
	else
	{
		IfThenElse* ite = new IfThenElse(if_location,cond_part,body_part);
		return ite;
	}
}

Expr *make_while_loop(std::vector<Token> tok)
{
	location while_location=tok[0].get_loc();
	tok.erase(tok.begin(),tok.begin()+1);

	std::vector<Token> cond_tok = gen_cond_vect(tok);
	Expr * cond_part = parse_token(cond_tok);
	tok.erase(tok.begin(),tok.begin()+cond_tok.size()+1);
	
	std::vector<Token> body_tok = gen_body_vect(tok);
	Expr *body_part = parse_token(body_tok);
	tok.erase(tok.begin(),tok.begin()+body_tok.size()+1);

	WhileLoop* wl = new WhileLoop(while_location,cond_part,body_part);
	return wl;
}

Expr * make_assign(std::vector<Token> tok)
{

	Identifier * id = (Identifier*)make_identifier(tok[0]);


}

Expr * parse_token(std::vector<Token> tok)
{
	Expr* abc = new Expr();
	return abc;
}

Tree parser(std::vector<Token> tok)
{
	Expr* root = parse_token(tok);
}