#include "ast.hpp"
#include <vector>

std::vector<Token> negatif(std::vector<Token> tok)
{
    size_t i = 0;

    while(tok[i].get_type() != EOF_)
    {
        if(tok[i].get_type() == MINUS)
        {
                if(tok[i+1].get_type() == LPAREN)
                {
                    int cpt = 1;
                    size_t j = i+2;
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

                tok.insert(tok.begin()+i,Token(INT,(location){0,0},"0"));
                tok.insert(tok.begin()+i,Token(LPAREN,(location){0,0}));
        }
        i++;
    }

    return tok;
}

int main()
{
	std::vector<Token> tok;
	
	tok.push_back(Token(MINUS,(location){0,0}));
	tok.push_back(Token(INT,(location){0,0},"46"));


	tok = negatif(tok);
	std::string str = gen_tok_string(tok); 
	std::cout<<str<<std::endl;

	create_assign(tok,0);

	return 0;
}