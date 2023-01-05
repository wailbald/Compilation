#include "lexer.hpp"

std::string read_line(FILE* f)
{
	std::string line;
	int buff=0;
	do{
		buff = fgetc(f);
		line.push_back((char)buff);
	}
	while((buff != EOF ) && (line_terminator.find(buff) == line_terminator.npos));
	if(buff == EOF)
	{
		line.pop_back();
		line.append("#EOF#");
	}

	return line;
}

std::vector<Token> gen_tok_line(std::string line,size_t *line_num,size_t *col_num, char* filename)
{
	std::vector<Token> tok_line;
	std::smatch match;
	std::string full_line(line);
	while(line.size()!=0)
	{
		line.insert(0,"#/#");
		if(std::regex_search(line,match,EOL_regex))
		{
			line.erase(match.position(),match.length());
			*col_num = 1;
			*line_num = *line_num + 1;
			continue;
		}
		if(std::regex_search(line,match,BLANK_regex))
		{
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,INTDECL_regex))
		{
			tok_line.push_back(Token(DECL,(location){*line_num,*col_num},"int"));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,STRDECL_regex))
		{
			tok_line.push_back(Token(DECL,(location){*line_num,*col_num},"str"));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,CHARDECL_regex))
		{
			tok_line.push_back(Token(DECL,(location){*line_num,*col_num},"char"));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,DOUBLEDECL_regex))
		{
			tok_line.push_back(Token(DECL,(location){*line_num,*col_num},"double"));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,TABDECL_regex))
		{	
			tok_line.push_back(Token(TAB,(location){*line_num,*col_num},"tab"));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}	
		if(std::regex_search(line,match,IF_regex))
		{
			tok_line.push_back(Token(IF,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,ELSE_regex))
		{
			tok_line.push_back(Token(ELSE,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,WHILE_regex))
		{
			tok_line.push_back(Token(WHILE,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,FOR_regex))
		{
			tok_line.push_back(Token(FOR,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,RETURN_regex))
		{
			tok_line.push_back(Token(ELSE,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,DO_regex))
		{
			tok_line.push_back(Token(DO,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,BREAK_regex))
		{
			tok_line.push_back(Token(BREAK,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,CONTINUE_regex))
		{
			tok_line.push_back(Token(CONTINUE,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		
		if(std::regex_search(line,match,EOF_regex))
		{
			tok_line.push_back(Token(EOF_,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,LPAREN_regex))
		{
			tok_line.push_back(Token(LPAREN,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,RPAREN_regex))
		{
			tok_line.push_back(Token(RPAREN,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,LBRACE_regex))
		{
			tok_line.push_back(Token(LBRACE,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,RBRACE_regex))
		{
			tok_line.push_back(Token(RBRACE,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,LBRCKT_regex))
		{
			tok_line.push_back(Token(LBRCKT,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,RBRCKT_regex))
		{
			tok_line.push_back(Token(RBRCKT,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,COMMA_regex))
		{
			tok_line.push_back(Token(COMMA,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,SEMICOLON_regex))
		{
			tok_line.push_back(Token(SEMICOLON,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,COLON_regex))
		{
			tok_line.push_back(Token(COLON,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,DOT_regex))
		{
			tok_line.push_back(Token(DOT,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,EQ_regex))
		{
			tok_line.push_back(Token(EQ,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,NEQ_regex))
		{
			tok_line.push_back(Token(NEQ,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,LT_regex))
		{
			tok_line.push_back(Token(LT,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,GT_regex))
		{
			tok_line.push_back(Token(GT,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,LE_regex))
		{
			tok_line.push_back(Token(LE,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,GE_regex))
		{
			tok_line.push_back(Token(GE,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,CAND_regex))
		{
			tok_line.push_back(Token(CAND,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,COR_regex))
		{
			tok_line.push_back(Token(COR,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,PLUS_regex))
		{
			tok_line.push_back(Token(PLUS,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,MINUS_regex))
		{
			tok_line.push_back(Token(MINUS,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,TIMES_regex))
		{
			tok_line.push_back(Token(TIMES,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,DIVIDE_regex))
		{
			tok_line.push_back(Token(DIVIDE,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,MODULE_regex))
		{
			tok_line.push_back(Token(MODULE,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,ASSIGN_regex))
		{
			tok_line.push_back(Token(ASSIGN,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,LSHIFT_regex))
		{
			tok_line.push_back(Token(LSHIFT,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,RSHIFT_regex))
		{
			tok_line.push_back(Token(RSHIFT,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,AND_regex))
		{
			tok_line.push_back(Token(AND,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,OR_regex))
		{
			tok_line.push_back(Token(OR,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,XOR_regex))
		{
			tok_line.push_back(Token(XOR,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,COMP_regex))
		{
			tok_line.push_back(Token(COMP,(location){*line_num,*col_num}));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,ID_regex))
		{
			tok_line.push_back(Token(ID,(location){*line_num,*col_num},line.substr(match.position()+3,match.length()-3)));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,STR_regex))
		{
			tok_line.push_back(Token(STR,(location){*line_num,*col_num},line.substr(match.position()+3,match.length()-3)));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,CHAR_regex))
		{
			tok_line.push_back(Token(CHAR,(location){*line_num,*col_num},line.substr(match.position()+3,match.length()-3)));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,DOUBLE_regex))
		{
			tok_line.push_back(Token(DOUBLE,(location){*line_num,*col_num},line.substr(match.position()+3,match.length()-3)));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		if(std::regex_search(line,match,INT_regex))
		{
			tok_line.push_back(Token(INT,(location){*line_num,*col_num},line.substr(match.position()+3,match.length()-3)));
			line.erase(match.position(),match.length());
			*col_num= *col_num + match.length() - 3;
			continue;
		}
		char path[PATH_MAX ];
		getcwd(path,PATH_MAX);
		printf("\033[1m%s/%s:%lu:%lu: \033[1;31mError\033[0m: Unrecognized token\n",path,filename,*line_num,*col_num);
		exit(2);
	}
	return tok_line;
}

std::vector<Token> lexer(char* filename)
{
	std::string line;
	
	FILE* f = fopen(filename,"r");
	std::vector<Token> tok;
	size_t line_num = 1;
	size_t col_num = 1;
	do
	{
		line = read_line(f);
		std::vector<Token> tok_line = gen_tok_line(line,&line_num,&col_num,filename);
		tok.insert(tok.end(),tok_line.begin(),tok_line.end());
	}
	while(line.find("#EOF#")==line.npos);

	return tok;
}