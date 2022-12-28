#include "token.hpp"

std::ostream& operator<<(std::ostream& os, Token& tok)
{
	return os<<"\\@"<<token_name[tok.type]<<","<<tok.text<<"\\";
}
