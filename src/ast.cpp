#include "ast.hpp"

void Node::add_left(Token *l)
{
	this->lson = l;
}

void Node::add_right(Token *r)
{
	this->rson = r;
}