#pragma once

#include <iostream>
#include <string>

enum token_t {
	/* Symbols */

	LPAREN,		// (
	RPAREN,		// )
	LBRACE,		// {
	RBRACE,		// }
	LBRCKT,		// [
	RBRCKT,		// ]
	COMMA,		// ,
	SEMICOLON,	// ;
	COLON,		// :
	DOT,		// .	
	
	/* Binary Operator */

	PLUS,		// +
	MINUS,		// -
	TIMES,		// *
	DIVIDE,		// /
	MODULE,		// %
	ASSIGN,		// =
	LSHIFT, 	// <<
	RSHIFT,		// >>
	AND,		// &
	OR,			// |
	XOR,		// ^
	COMP,		// ~

	/* Comparaison Operator */

	EQ,			// ==
	NEQ,		// !=
	LT,			// <
	GT,			// >
	LE,			// <=
	GE,			// >=
	CAND,		// &&
	COR,		// ||
	

	/* Keywords */

	IF,			// if
	ELSE,		// else
	WHILE,		// while
	FOR,		// for
	RETURN,		// return
	DO,			// do
	BREAK,		// break
	CONTINUE,	// continue

	/* Identifiers */

	ID,			// identifier
	
	/* Type */

	STR,		// string
	CHAR,		// char
	INT,		// integer
	DOUBLE,		// double
	TAB,		// tab

	/* Other */

	DECL,		// déclaration fonction ou variable
	EOF_			// End Of File
};

static std::string token_name[] =
{
	"LPAREN",		// (
	"RPAREN",		// )
	"LBRACE",		// {
	"RBRACE",		// }
	"LBRCKT",		// [
	"RBRCKT",		// ]
	"COMMA",		// ,
	"SEMICOLON",	// ;
	"COLON",		// :
	"DOT",			// .	
	
	/* Binary Operator */

	"PLUS",			// +
	"MINUS",		// -
	"TIMES",		// *
	"DIVIDE",		// /
	"MODULE",		// %
	"ASSIGN",		// =
	"LSHIFT", 		// <<
	"RSHIFT",		// >>
	"AND",			// &
	"OR",			// |
	"XOR",			// ^
	"COMP",			// ~

	/* Comparaison Operator */

	"EQ",			// ==
	"NEQ",			// !=
	"LT",			// <
	"GT",			// >
	"LE",			// <=
	"GE",			// <=
	"CAND",			// &&
	"COR",			// ||
	
	/* Keywords */

	"IF",			// if
	"ELSE",			// else
	"WHILE",		// while
	"FOR",			// for
	"RETURN",		// return
	"DO",			// do
	"BREAK",		// break
	"CONTINUE",		// continue

	/* Identifiers */

	"ID",			// identifier
	
	/* Literal */

	"STR",			// string
	"CHAR",			// char
	"INT",			// integer
	"DOUBLE",		// double
	"TAB",			// tab

	/* Other */

	"DECL",
	"EOF_"			// End Of File
};

typedef struct location{
	size_t line;
	size_t col;
} location;

class Token
{
	token_t type;
	location loc;
	std::string text;

	public:
	Token(token_t type_,location loc_, std::string text_) : type(type_), loc(loc_), text(text_) {};
	Token(token_t type_,location loc_) : type(type_), loc(loc_), text("") {};
	location get_loc(){return this->loc;};
	token_t get_type(){return this->type;};
	std::string get_text(){return this->text;};

	friend std::ostream& operator<<(std::ostream& os, Token& tok);
};