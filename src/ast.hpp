#include "token.hpp"
#include <vector>

class Node
{
	Token tok;
	Node *lson;
	Node *rson;

	public:
		Node(Token tok_, Node *lson_, Node *rson_) : tok(tok_), lson(lson_), rson(rson_){};
		Node(Token tok_) : tok(tok_), lson(NULL), rson(NULL){};
		Node *get_right(){return this->rson;};
		Node *get_left(){return this->lson;};
		Token get_tok(){return this->tok;};
		void add_tok(Token t){this->tok = t;};
		void add_left(Node *l){this->lson = l;};
		void add_right(Node *r){this->rson = r;};
};

class Tree
{
	Node *root;

	public:
		Tree(Node *root_) : root(root_) {};
		Node *get_root(){return this->root;};
};

int check_type(Token t);
void verif_assign(std::vector<Token> tok, int i);
Tree create_assign(std::vector<Token> tok, int i);
