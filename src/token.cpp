#include "token.hpp"

std::ostream& operator<<(std::ostream& os, Token& tok)
{
	os<<token_name[tok.type];
	if(tok.get_text().size() != 0)
	{
		os<<"{"<<tok.text<<"}";
	}
	return os;

}
