#include "ast.hpp"

std::vector<Token> negatif(std::vector<Token> tok)
{
	int i = 0;

	while(tok[i].get_type() != EOF_)
	{
		if(tok[i].get_type() == MINUS)
		{
			if(tok[i-1].get_type() != RPAREN || tok[i-1].get_type() != ID || tok[i-1].get_type() != STR 
				|| tok[i-1].get_type() != CHAR || tok[i-1].get_type() != INT || tok[i-1].get_type() != DOUBLE)
			{
				if(tok[i+1].get_type() == LPAREN)
				{
					int cpt = 1;
					int j = i+2;
					while(cpt != 0)
					{
						if(tok[j].get_type() == LPAREN)
							cpt++;

						if(tok[j].get_type() == RPAREN)
							cpt--;

						j++;
					}
					tok.insert(tok.begin()+j,Token(RPAREN,(location){0,0}));
				}
				else
				{
					tok.insert(tok.begin()+i+2,Token(RPAREN,(location){0,0}));
				}
				
				tok.insert(tok.begin()+i,Token(MINUS,(location){0,0}));
				tok.insert(tok.begin()+i,Token(INT,(location){0,0},"0"));
				tok.insert(tok.begin()+i,Token(LPAREN,(location){0,0}));
			}
		}
		i++;
	}

	return tok;
}

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
			tok.erase(tok.begin()+i);
		}

		if(tok[i].get_type() == LPAREN)
		{
			stack.push_back(tok[i]);
			tok.erase(tok.begin()+i);
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
			tok.erase(tok.begin()+i);
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

Expr *math_expr(std::vector<Token> tok)
{	
	size_t j = tok.size();
	location l;
	Operation op;
	auto k = tok[j].get_type();
		
	switch(k)
	{
		
		case PLUS: 
		{
			l = tok[j].get_loc();
			op = o_plus;
			tok.erase(tok.begin()+j);
			BinaryOperator *np = new BinaryOperator(l,math_expr(tok),math_expr(tok),op);
			return np;
		}
				
		case MINUS: 
		{
			l = tok[j].get_loc();
			op = o_minus;
			tok.erase(tok.begin()+j);
			BinaryOperator *nm = new BinaryOperator(l,math_expr(tok),math_expr(tok),op);
			return nm;
		}
				
		case TIMES: 
		{
			l = tok[j].get_loc();
			op = o_mult;
			tok.erase(tok.begin()+j);
			BinaryOperator *nt = new BinaryOperator(l,math_expr(tok),math_expr(tok),op);
			return nt;
		}
				
		case DIVIDE: 
		{
			l = tok[j].get_loc();
			op = o_div;
			tok.erase(tok.begin()+j);
			BinaryOperator *nd = new BinaryOperator(l,math_expr(tok),math_expr(tok),op);
			return nd;
		}
				
		case MODULE: 
		{
			l = tok[j].get_loc();
			op = o_module;
			tok.erase(tok.begin()+j);
			BinaryOperator *nmo = new BinaryOperator(l,math_expr(tok),math_expr(tok),op);
			return nmo;
		}
				
		case LSHIFT: 
		{
			l = tok[j].get_loc();
			op = o_lshift;
			tok.erase(tok.begin()+j);
			BinaryOperator *nls = new BinaryOperator(l,math_expr(tok),math_expr(tok),op);
			return nls;
		}
				
		case RSHIFT: 
		{
			l = tok[j].get_loc();
			op = o_rshift;
			tok.erase(tok.begin()+j);
			BinaryOperator *nrs = new BinaryOperator(l,math_expr(tok),math_expr(tok),op);
			return nrs;
		}
				
		case AND: 
		{
			l = tok[j].get_loc();
			op = o_and;
			tok.erase(tok.begin()+j);
			BinaryOperator *nand = new BinaryOperator(l,math_expr(tok),math_expr(tok),op);
			return nand;
		}
				
		case OR: 
		{
			l = tok[j].get_loc();
			op = o_or;
			tok.erase(tok.begin()+j);
			BinaryOperator *nor = new BinaryOperator(l,math_expr(tok),math_expr(tok),op);
			return nor;
		}
				
		case XOR:
		{ 
			l = tok[j].get_loc();
			op = o_xor;
			tok.erase(tok.begin()+j);
			BinaryOperator *nxor = new BinaryOperator(l,math_expr(tok),math_expr(tok),op);
			return nxor;
		}
				
		case COMP: 
		{
			l = tok[j].get_loc();
			op = o_comp;
			tok.erase(tok.begin()+j);
			BinaryOperator *nco = new BinaryOperator(l,math_expr(tok),math_expr(tok),op);
			return nco;
		}
				
		case EQ: 
		{
			l = tok[j].get_loc();
			op = c_eq;
			tok.erase(tok.begin()+j);
			BinaryOperator *neq = new BinaryOperator(l,math_expr(tok),math_expr(tok),op);
			return neq;
		}
				
		case NEQ: 
		{
			l = tok[j].get_loc();
			op = c_neq;
			tok.erase(tok.begin()+j);
			BinaryOperator *nneq = new BinaryOperator(l,math_expr(tok),math_expr(tok),op);
			return nneq;
		}
				
		case LT: 
		{
			l = tok[j].get_loc();
			op = c_lt;
			tok.erase(tok.begin()+j);
			BinaryOperator *nlt = new BinaryOperator(l,math_expr(tok),math_expr(tok),op);
			return nlt;
		}
				
		case GT: 
		{
			l = tok[j].get_loc();
			op = c_gt;
			tok.erase(tok.begin()+j);
			BinaryOperator *ngt = new BinaryOperator(l,math_expr(tok),math_expr(tok),op);
			return ngt;
		}
				
		case LE: 
		{
			l = tok[j].get_loc();
			op = c_le;
			tok.erase(tok.begin()+j);
			BinaryOperator *nle = new BinaryOperator(l,math_expr(tok),math_expr(tok),op);
			return nle;
		}
				
		case GE: 
		{
			l = tok[j].get_loc();
			op = c_ge;
			tok.erase(tok.begin()+j);
			BinaryOperator *nge = new BinaryOperator(l,math_expr(tok),math_expr(tok),op);
			return nge;
		}
				
		case CAND: 
		{
			l = tok[j].get_loc();
			op = c_and;
			tok.erase(tok.begin()+j);
			BinaryOperator *ncand = new BinaryOperator(l,math_expr(tok),math_expr(tok),op);
			return ncand;
		}
				
		case COR: 
		{
			l = tok[j].get_loc();
			op = c_or;
			tok.erase(tok.begin()+j);
			BinaryOperator *ncor = new BinaryOperator(l,math_expr(tok),math_expr(tok),op);
			return ncor;
		}
				
		case INT:
		{
			Expr *nint = make_integer_literal(tok[j]);
			tok.erase(tok.begin()+j);
			return nint;
		}
				
		case DOUBLE:
		{
			Expr *ndou = make_double_literal(tok[j]);
			tok.erase(tok.begin()+j);
			return ndou;
		}
		
		case ID:
		{
			Expr *nid = make_identifier(tok[j]);
			tok.erase(tok.begin()+j);
			return nid;
		}

		default :
		{
			return NULL;
		}
	}
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

	Expr* decl = NULL;
	std::string line = gen_tok_string(tok);
	line.insert(0,"#/#");
	std::smatch match;
	if(tok[0].get_type() == SEMICOLON)
	{
		decl = NULL;
		tok.erase(tok.begin());
	}
	else if(std::regex_search(line,match,pASSIGN_regex))
	{
		decl = make_assign(tok);
	}
	else if(std::regex_search(line,match,pVARDECL_regex))
	{
		decl = make_var_decl(tok);
	}

	Expr *cond = math_expr(tok);
	tok.erase(tok.begin);

}

Expr * parse_assign(std::vector<Token> tok)
{	
	std::string line = gen_tok_string(tok);
	line.insert(0,"#/#");
	std::smatch match;
	Expr* expr = NULL;
	if(std::regex_search(line,match,pOP_regex))
	{
		expr = math_expr(tok);
		tok.erase(tok.begin());
	}
	/*else if(std::regex_search(line,match,pIDENTIFER_regex))
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
	}*/
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

