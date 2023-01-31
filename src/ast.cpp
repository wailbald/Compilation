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

/*
%nonassoc FUNCTION VAR TYPE DO OF ASSIGN;
%left OR;
%left AND;
%nonassoc EQ NEG GT GE LE LT;
%left PLUS MINUS;
%left TIMES DIVIDE;
%left UMINUS;
*/

int priorite(Token tok)
{
	if(tok.get_type() == TIMES || tok.get_type() == DIVIDE)
		return 5;

	if(tok.get_type() == PLUS || tok.get_type() == MINUS)
		return 4;

	if(tok.get_type() == EQ || tok.get_type() == NEQ || tok.get_type() == GT 
		|| tok.get_type() == GE || tok.get_type() == LE || tok.get_type() == LT)
		return 3;

	if(tok.get_type() == AND)
		return 2;

	if(tok.get_type() == OR)
		return 1;
		
	std::cout << "symbole non defini" << std::endl;
	exit(4);
}

std::vector<Token> turntoNPI(std::vector<Token> tok, int i)
{
	std::vector<Token> out;
	std::vector<Token> stack;

	while(tok[i].get_type() != SEMICOLON)
	{
		if(tok[i].get_type() == ID || tok[i].get_type() == STR || tok[i].get_type() == CHAR
			|| tok[i].get_type() == INT|| tok[i].get_type() == DOUBLE)
		{
			out.push_back(tok[i]);
		}

		if(tok[i].get_type() == LPAREN)
		{
			stack.push_back(tok[i]);
		}

		if(tok[i].get_type() == RPAREN)
		{
			while(stack.back().get_type() != LPAREN)
			{
				out.push_back(stack.back());
				stack.pop_back();
			}
			//on enleve la parenthese restante
			stack.pop_back();
		}

		else
		{
			for(int j = stack.size()-1; j > 0; j--)
			{
				if(stack[i].get_type() == LPAREN)
					break; 
				else
				{
					if(priorite(tok[i]) < priorite(stack[j]))
					{
						out.push_back(stack[j]);
						stack.erase(stack.begin()+j);
					}
				}
			}
			stack.push_back(tok[i]);
		}
		i++;
	}
	while(!stack.empty())
	{
		out.push_back(stack.back());
		stack.pop_back();
	}
	return out;
}

std::vector<Token> gen_cond_vect(std::vector<Token> basetok)
{
	int paren_depth = 1;
	std::vector<Token> cond_tok;
	for(auto elem : basetok)
	{
		switch (elem.get_type())
		{
			case(LBRACE):
				printf("Error: Unexpected Token in condition\n");
				exit(6);
			case(RBRACE):
				printf("Error: Unexpected Token in condition\n");
				exit(6);
			case(SEMICOLON):
				printf("Error: Unexpected Token in condition\n");
				exit(6);
			case(COLON):
				printf("Error: Unexpected Token in condition\n");
				exit(6);
			case(IF):
				printf("Error: Unexpected Token in condition\n");
				exit(6);
			case(ELSE):
				printf("Error: Unexpected Token in condition\n");
				exit(6);
			case(WHILE):
				printf("Error: Unexpected Token in condition\n");
				exit(6);
			case(FOR):
				printf("Error: Unexpected Token in condition\n");
				exit(6);
			case(RETURN):
				printf("Error: Unexpected Token in condition\n");
				exit(6);
			case(DO):
				printf("Error: Unexpected Token in condition\n");
				exit(6);
			case(BREAK):
				printf("Error: Unexpected Token in condition\n");
				exit(6);
			case(CONTINUE):
				printf("Error: Unexpected Token in condition\n");
				exit(6);
			case(EOF_):
				printf("Error: Unexpected End Of File in a condition\n");
				exit(6);
			default: break;
		}
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

Expr * make_double_literal(Token tok)
{
	return new DoubleLiteral(tok.get_loc(),atol(tok.get_text().c_str()));
}

Expr * make_string_literal(Token tok)
{
	return new StringLiteral(tok.get_loc() ,tok.get_text());
}

Type get_decl_type(Token tok)
{
	if(tok.get_text() == "void")
	{
		return t_void;
	}
	if(tok.get_text() == "int")
	{
		return t_int;
	}
	if(tok.get_text() == "double")
	{
		return t_double;
	}
	if(tok.get_text() == "t_string")
	{
		return t_string;
	}
	return t_undef;
}

Decl * make_var_decl(std::vector<Token> tok)
{
	location decl_loc = tok[0].get_loc();

	Expr * expr = NULL;

	if(tok[2].get_type() == SEMICOLON)
	{
		tok.erase(tok.begin(),tok.begin()+2);
	}
	else if(tok[2].get_type() == ASSIGN)
	{
		tok.erase(tok.begin());
		expr = make_assign(tok);
	}
	else
	{
		printf("Unexpected token in variable delcaration");
		exit(5);
	}

	VarDecl * decl = new VarDecl(decl_loc,tok[1].get_text(),get_decl_type(tok[0]),expr);

	return decl;
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

Expr *make_for_loop(std::vector<Token> tok)
{
	location for_location=tok[0].get_loc();
	tok.erase(tok.begin(),tok.begin()+1);

	Decl* decl;
	std::string line = gen_tok_string(tok);
	line.insert(0,"#/#");
	std::smatch match;
	if(tok[0].get_type() == SEMICOLON)
	{
		decl = NULL;
		tok.erase(tok.begin());
	}
	else if(std::regex_search(line,match,pOP_regex))
	{

	}
}

Expr * parse_assign(std::vector<Token> tok)
{	
	std::string line = gen_tok_string(tok);
	line.insert(0,"#/#");
	std::smatch match;
	Expr* expr = NULL;
	if(std::regex_search(line,match,pOP_regex))
	{
		//expr = make_math_expression()
		tok.erase(tok.begin());
	}
	else if(std::regex_search(line,match,pIDENTIFER_regex))
	{
		expr = make_identifier(tok[0]);
		tok.erase(tok.begin());
	}
	else if(std::regex_search(line,match,pDOUBLELITERAL_regex))
	{
		expr = make_double_literal(tok[0]);
		tok.erase(tok.begin());
	}
	else if(std::regex_search(line,match,pINTEGERLITERAL_regex))
	{
		expr = make_integer_literal(tok[0]);
		tok.erase(tok.begin());
	}
	else
	{
		printf("Error: Wrong right part of assign expression\n");
		exit(5);
	}
	return expr;
}

Expr * make_assign(std::vector<Token> tok)
{
	Identifier * id = (Identifier*)make_identifier(tok[0]);
	tok.erase(tok.begin());

	location loc = tok[0].get_loc();
	tok.erase(tok.begin());

	Expr* rhs = parse_assign(tok);
	if(tok[0].get_type() == SEMICOLON)
		tok.erase(tok.begin());
	else
	{
		printf("Unexpected token after assign\n");
		exit(5);
	}
	return new Assign(loc,id,rhs);
}

Expr * parse_token(std::vector<Token> tok)
{
	Expr* abc = new Expr();
	return abc;
}

Expr * parse_seq(std::vector<Token> tok)
{
	Expr* abc = new Expr();
	return abc;
}

Tree parser(std::vector<Token> tok)
{
	Expr* root = parse_seq(tok);
}

