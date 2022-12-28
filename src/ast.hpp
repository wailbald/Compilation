#include "token.hpp"

class Node
{
	Token *tok;
	Token *lson;
	Token *rson

	public:
		Node(Token *tok_, Token *lson_, Token *rson_) : tok(tok_), lson(lson_), rson(rson_){};
		Node(Token *tok_) : tok(tok_), lson(NULL), rson(NULL){};
		void add_left(Token *l);
		void add_right(Token *r);
};

class Tree
{
	Node *root;

	public:
		Tree(Node *root_) : root(root_) {};
};