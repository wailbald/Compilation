#include "ast.hpp"

std::unordered_map<std::string, Expr> func_map;



std::vector<Token> negatif(std::vector<Token> &tok)
{ 
	for(unsigned long i = 0; tok[i].get_type() != SEMICOLON && i < tok.size() && tok.size(); i++)
	{
		std::cout << "i = " << i << " taille = " << tok.size() << std::endl;
		if(tok[i].get_type() == MINUS && i > 1)
		{
			std::cout << "on y est" << std::endl;
			if(tok[i-1].get_type() != RPAREN && tok[i-1].get_type() != ID && tok[i-1].get_type() != STR 
				&& tok[i-1].get_type() != CHAR && tok[i-1].get_type() != INT && tok[i-1].get_type() != DOUBLE)
			{
				if(tok[i+1].get_type() == LPAREN)
				{
					std::cout << "on est ici" << std::endl;
					int paren = 1;
					int j = i+2;
					while(paren <= 0)
					{
						std::cout << "j = " << j << std::endl;
						std::cout << "paren = " << paren << std::endl;
						std::cout << token_name[tok[j].get_type()] << std::endl;
						if(tok[j].get_type() == LPAREN)
							paren ++;
						if(tok[j].get_type() == RPAREN)
							paren --;

						j++;
					}
					std::cout << "la" << std::endl;
					tok.insert(tok.begin()+j,Token(RPAREN,(location){0,0}));
				}
			else
				{
					std::cout << "sinon on est la" << std::endl;
					tok.insert(tok.begin()+i+1,Token(RPAREN,(location){0,0}));
					std::cout << "la" << std::endl;
				}
			std::cout << "la" << std::endl;
			tok.insert(tok.begin()+i,Token(INT,(location){0,0},"0"));
			tok.insert(tok.begin()+i,Token(LPAREN,(location){0,0}));
			}
		}
	}
	return tok;
}

int priorite(Token tok)
{
	
	if(tok.get_type() == LPAREN || tok.get_type() == RPAREN)
		return 11;
	
	if(tok.get_type() == NOT || tok.get_type() == COMP)
		return 10;
	
	if(tok.get_type() == TIMES || tok.get_type() == DIVIDE || tok.get_type() == MODULE)
		return 9;

	if(tok.get_type() == PLUS || tok.get_type() == MINUS)
		return 8;

	if(tok.get_type() == GT || tok.get_type() == GE || tok.get_type() == LE || tok.get_type() == LT)
		return 7;

	if(tok.get_type() == EQ || tok.get_type() == NEQ)
		return 6;
	
	if(tok.get_type() == AND)
		return 5;

	if(tok.get_type() == OR)
		return 4;
	
	if(tok.get_type() == XOR)
		return 3;
	
	if(tok.get_type() == CAND)
		return 2;
	
	if(tok.get_type() == COR)
		return 1;
	std::cout<<token_name[tok.get_type()];
	std::cout << " symbole non defini" << std::endl;
	exit(4);
}

void func_npi(int a, std::vector<Token> t)
{
	auto call = make_funcall(t);
	func_map[std::to_string(a)] = *call;
}

std::vector<Token> turntoNPI(std::vector<Token> &tok)
{
	int id = 1;
	std::vector<Token> out;
	std::vector<Token> stack;
	int i = 0;
	int paren = 0;

	while(tok[i].get_type() != SEMICOLON && !tok.empty())
	{
		if(tok[i].get_type() == ID || tok[i].get_type() == STR || tok[i].get_type() == CHAR
			|| tok[i].get_type() == INT|| tok[i].get_type() == DOUBLE)
		{
			if(tok.size()>1 && tok[i+1].get_type() == LPAREN)
			{
				std::vector<Token> fun;

				while(tok[i].get_type() != RPAREN)
				{
					fun.push_back(tok[i]);
					tok.erase(tok.begin());
				}
				fun.push_back(tok[i]);
				tok.erase(tok.begin());

				func_npi(id,fun);
				Token(FUNCALL,(location){0,0},std::to_string(id));
				id ++;
			}

			else
			{
				out.push_back(tok[i]);
				tok.erase(tok.begin());
			}
		}
		else if(tok[i].get_type() == LPAREN)
		{
			paren ++;
			stack.push_back(tok[i]);
			tok.erase(tok.begin());
		}
		else if(tok[i].get_type() == RPAREN)
		{
			paren --;
			if(paren < 0)
			{
				while(!stack.empty())
				{
					out.push_back(stack.back());
					stack.pop_back();
				}
				return out;
			}
			else
			{
				while(stack.back().get_type() != LPAREN)
				{
					out.push_back(stack.back());
					stack.pop_back();
				}
				//on enleve la parenthese restante
				tok.erase(tok.begin());	
				stack.pop_back();
			}
		}
		else
		{
			for(int j = stack.size()-1; j >= 0; j--)
			{
				if(stack[j].get_type() == LPAREN)
				{
					break; 
				}
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
			tok.erase(tok.begin());
		}
	}
	while(!stack.empty())
	{
		out.push_back(stack.back());
		stack.pop_back();
	}
	return out;
}

Expr *math_expr(std::vector<Token> &tok)
{	
	size_t j = tok.size()-1;
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
			UnaryOperator *nco = new UnaryOperator(l,math_expr(tok),op);
			return nco;
		}
		
		case NOT: 
		{
			l = tok[j].get_loc();
			op = o_not;
			tok.erase(tok.begin()+j);
			UnaryOperator *nnot = new UnaryOperator(l,math_expr(tok),op);
			return nnot;
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

		case FUNCALL:
		{
			auto decl = func_map.find(tok[j].get_text());
			if(decl != func_map.cend())
			{
				tok.erase(tok.begin()+j);
				return &decl->second;
			}
			else
			{
				std::cout << "Probleme lors de la gestion de la fonction dans l'expression mathematique" << std::endl;
				exit(7);
			}
			
		}

		default :
		{
			std::cout<<"Error in math expression"<<std::endl;
			exit(5);
			return NULL;
		}
	}
}

std::vector<Token> gen_cond_vect(std::vector<Token> &basetok)
{
	int paren_depth = 0;
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
		cond_tok.push_back(elem);
		if(!paren_depth)
			return cond_tok;
	}
	printf("Error: Unexpected End Of File\n");
	exit(6);
	return cond_tok;
}

std::vector<Token> gen_body_vect(std::vector<Token> &basetok)
{
	int brckt_depth = 0;
	bool in_bracket = false;
	std::vector<Token> body_tok;
	for(auto elem : basetok)
	{
		if(elem.get_type() == SEMICOLON)
		{
			if(!in_bracket)
			{
				body_tok.push_back(elem);
				return body_tok;
			}
		}
		if(elem.get_type() == LBRACE)
		{
			brckt_depth++;
			in_bracket = true;
		}	
			
		if(elem.get_type() == RBRACE)
			brckt_depth--;
		if(!brckt_depth)
		{
			if(body_tok[0].get_type() == LBRACE)
				body_tok.erase(body_tok.begin());
			return body_tok;
		}
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

Expr * make_mathematical_expression(std::vector<Token> &tok)
{
	std::vector<Token> npi = negatif(tok);
	npi = turntoNPI(npi);
	Expr * expr = math_expr(npi);
	while(tok[0].get_type()!=SEMICOLON && tok.size())
	{
		tok.erase(tok.begin());
	}
	return expr;
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

Decl * make_var_decl(std::vector<Token> &tok)
{
	location decl_loc = tok[0].get_loc();

	Expr * expr = NULL;
	Token token_decl = tok[0];
	Token token_id = tok[1];

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

	VarDecl * decl = new VarDecl(decl_loc,token_id.get_text(),get_decl_type(token_decl),expr);

	return decl;
}

std::vector<VarDecl *> parse_func_params(std::vector<Token> &tok)
{
	std::vector<VarDecl *> params;	
	while(tok[0].get_type() != RPAREN && tok.size())
	{
		location decl_loc = tok[0].get_loc();
		Token token_decl = tok[0];
		Token token_id = tok[1];
		tok.erase(tok.begin(),tok.begin()+1);

		params.push_back(new VarDecl(decl_loc,token_id.get_text(),get_decl_type(token_decl),NULL));

		if(tok[0].get_type() == COLON)
			tok.erase(tok.begin());
	}
	if(tok.size())
		tok.erase(tok.begin());
	else
	{
		printf("Error while parsing function argument\n");
		exit(5);
	}

	return params;
}

Decl* make_fundecl(std::vector<Token> &tok)
{
	location fundecl_location=tok[0].get_loc();

	Token token_decl = tok[0];
	Token token_id = tok[1];

	tok.erase(tok.begin(),tok.begin()+1);

	std::vector<VarDecl *> params = parse_func_params(tok);

	std::vector<Token> body_tok = gen_body_vect(tok);
	Expr *body_part = make_seq(body_tok);
	tok.erase(tok.begin(),tok.begin()+body_tok.size()+1);

	return new FunDecl(fundecl_location, token_id.get_text(), get_decl_type(token_decl),params, body_part);	
}

std::vector<Expr *> parse_func_args(std::vector<Token> &tok)
{
	std::vector<Expr *> args;
	while(tok[0].get_type() != RPAREN)
	{
		std::cout<<"parse_arg: "<<gen_tok_string(tok)<<std::endl;
		std::string line = gen_tok_string(tok);
		std::smatch match;
		line.insert(0,"#/#");
		if(std::regex_search(line,match,pFUNCALL_regex))
		{
			args.push_back(make_funcall(tok));
		}
		if(std::regex_search(line,match,pOP_regex))
		{
			args.push_back(make_mathematical_expression(tok));
		}
		else if(std::regex_search(line,match,pSTRINGLITERAL_regex))
		{
			args.push_back(make_string_literal(tok[0]));
			tok.erase(tok.begin());
		}
		else
		{
			std::cout<<tok[0]<<std::endl;
			printf("Error: Wrong token in argument part\n");
			exit(5);
		}
		if(tok[0].get_type() == COMMA)
		{
			tok.erase(tok.begin());	
		}
	}
	tok.erase(tok.begin());
	return args;
}

Expr* make_funcall(std::vector<Token> &tok)
{
	location funcall_location=tok[0].get_loc();
	Token token_id = tok[0];
	tok.erase(tok.begin());
	tok.erase(tok.begin());
	std::cout<<std::endl<<gen_tok_string(tok)<<std::endl;
	std::vector<Expr *> args = parse_func_args(tok);

	return new FunCall(funcall_location,args,token_id.get_text());
}

Expr* make_if(std::vector<Token> &tok)
{
	location if_location=tok[0].get_loc();
	tok.erase(tok.begin(),tok.begin()+1);
	
	std::smatch match;
	std::vector<Token> cond_tok = gen_cond_vect(tok);
	Expr *cond_part = make_mathematical_expression(cond_tok);
	tok.erase(tok.begin(),tok.begin()+cond_tok.size()+1);
	
	std::vector<Token> body_tok = gen_body_vect(tok);
	Expr *body_part = make_seq(body_tok);
	tok.erase(tok.begin(),tok.begin()+body_tok.size()+1);

	std::string line = gen_tok_string(tok);
	line.insert(0,"#/#");
	if(std::regex_search(line,match,pELSEPART_regex))
	{
		std::vector<Token> else_tok = gen_body_vect(tok);
		Expr *else_part = make_seq(else_tok);
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

Expr *make_while_loop(std::vector<Token> &tok)
{
	location while_location=tok[0].get_loc();
	tok.erase(tok.begin(),tok.begin()+1);

	std::vector<Token> cond_tok = gen_cond_vect(tok);
	Expr * cond_part = make_mathematical_expression(cond_tok);
	while(tok[0].get_type() != LBRACE)
	{
		tok.erase(tok.begin());
	}


	std::vector<Token> body_tok = gen_body_vect(tok);
	Expr *body_part = make_seq(body_tok);
	while(tok[0].get_type() != RBRACE)
	{
		tok.erase(tok.begin());
	}
	tok.erase(tok.begin());

	WhileLoop* wl = new WhileLoop(while_location,cond_part,body_part);
	return wl;
}

std::vector<Token> gen_for_high(std::vector<Token> &tok)
{
	size_t i = 0;
	std::vector<Token> high_tok;
	while(tok[i].get_type() != RPAREN)
	{
		high_tok.push_back(tok[i]);
		i++;
	}
	return high_tok;
}

Expr *make_for_loop(std::vector<Token> &tok)
{
	location for_location=tok[0].get_loc();
	tok.erase(tok.begin(),tok.begin()+2);

	Expr* decl = NULL;
	std::string line = gen_tok_string(tok);
	line.insert(0,"#/#");
	std::smatch match;
	std::cout<<line<<std::endl;
	if(tok[0].get_type() == SEMICOLON)
	{
		decl = NULL;
		tok.erase(tok.begin());
	}
	else if(std::regex_search(line,match,pVARDECL_regex))
	{
		decl = make_var_decl(tok);
	}
	else if(std::regex_search(line,match,pASSIGN_regex))
	{
		decl = make_assign(tok);
	}
	
	std::cout<<gen_tok_string(tok)<<std::endl;
	Expr *cond = make_mathematical_expression(tok);
	tok.erase(tok.begin());

	std::vector<Token> high_tok = gen_for_high(tok);
	Expr *high = make_assign(high_tok);
	while(tok[0].get_type()!= RPAREN)
	{
		if(tok.empty())
		{
			printf("Error, expected RPAREN token\n");
			exit(5);
		}
		tok.erase(tok.begin());
	}
	tok.erase(tok.begin());
	std::cout<<std::endl<<gen_tok_string(tok)<<std::endl;
	std::cout<<"GEN DU BODY\n";
	std::vector<Token> body_tok = gen_body_vect(tok);
	Expr *body = make_seq(body_tok);
	while(tok[0].get_type() != RBRACE)
	{
		tok.erase(tok.begin());
	}
	tok.erase(tok.begin());

	return new ForLoop(for_location, decl, cond, high, body);
}

Expr * parse_assign(std::vector<Token> &tok)
{	
	std::string line = gen_tok_string(tok);
	line.insert(0,"#/#");
	std::smatch match;
	Expr* expr = NULL;
	if(std::regex_search(line,match,pFUNCALL_regex))
	{
		expr=make_funcall(tok);
	}
	else if(std::regex_search(line,match,pOP_regex))
	{
		expr = make_mathematical_expression(tok);
	}
	else if(std::regex_search(line,match,pSTRINGLITERAL_regex))
	{
		expr = make_string_literal(tok[0]);
	}
	else
	{
		std::cout<<line<<std::endl;
		printf("Error: Wrong right part of assign expression\n");
		exit(5);
	}
	return expr;
}

Expr * make_assign(std::vector<Token> &tok)
{
	Identifier * id = (Identifier*)make_identifier(tok[0]);
	tok.erase(tok.begin());

	location loc = tok[0].get_loc();
	tok.erase(tok.begin());

	Expr* rhs = parse_assign(tok);
	if(tok[0].get_type() == SEMICOLON)
		tok.erase(tok.begin());
	else if(!tok.empty())
	{
		printf("Unexpected token after assign\n");
		exit(5);
	}
	return new Assign(loc,id,rhs);
}

Expr *make_return(std::vector<Token> &tok)
{
	location loc = tok[0].get_loc();
	tok.erase(tok.begin());

	std::smatch match;
	std::string line = gen_tok_string(tok);
	line.insert(0,"#/#");
	
	Expr * expr = NULL;
	if(std::regex_search(line,match,pASSIGN_regex))
	{
		expr = make_assign(tok);
	}
	else if(std::regex_search(line,match,pOP_regex))
	{
		expr = make_mathematical_expression(tok);
	}

	return new Return(loc,expr);
}

Expr * make_break(std::vector<Token> &tok)
{
	location loc = tok[0].get_loc();
	tok.erase(tok.begin());

	return new Break(loc);
}

Expr * parse_token(std::vector<Token> &tok)
{
	std::smatch match;
	std::string line = gen_tok_string(tok);
	line.insert(0,"#/#");
	std::cout<<"----------------------------------------\n"<<line<<std::endl;
	if(std::regex_search(line,match,pRETURN_regex))
	{
		return make_return(tok);
	}
	if(std::regex_search(line,match,pBREAK_regex))
	{
		return make_break(tok);
	}
	if(std::regex_search(line,match,pASSIGN_regex))
	{	
		return make_assign(tok);
	}
	if(std::regex_search(line,match,pFUNDECL_regex))
	{
		return make_fundecl(tok);
	}
	if(std::regex_search(line,match,pVARDECL_regex))
	{
		return make_var_decl(tok);
	}
	if(std::regex_search(line,match,pIF_regex))
	{
		return make_if(tok);
	}
	if(std::regex_search(line,match,pWHILE_regex))
	{
		return make_while_loop(tok);
	}
	if(std::regex_search(line,match,pFOR_regex))
	{
		return make_for_loop(tok);
	}
	if(std::regex_search(line,match,pFUNCALL_regex))
	{
		return make_funcall(tok);
	}
	if(std::regex_search(line,match,pOP_regex))
	{
		return make_mathematical_expression(tok);
	}
	if(std::regex_search(line,match,pSTRINGLITERAL_regex))
	{
		Token tok__ = tok[0];
		tok.erase(tok.begin());
		return make_string_literal(tok__);
	}
	return new Expr();
}

Expr * make_seq(std::vector<Token> &tok)
{
	location seq_location = tok[0].get_loc();
	std::vector<Expr *> exprs;
	while(tok[0].get_type() != EOF_ && !tok.empty())
	{
		exprs.push_back(parse_token(tok));
		if(tok[0].get_type() == SEMICOLON && !tok.empty())
		{
			tok.erase(tok.begin());
		}
	}
	return new Sequence(seq_location, exprs);
}

Tree parser(std::vector<Token> &tok)
{
	Expr* root = make_seq(tok);
	return Tree(root);
}

