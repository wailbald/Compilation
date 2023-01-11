#pragma once

#include <iostream>
#include <regex>
#include <string>
#include <climits>
#include <unistd.h>

#include "token.hpp"

const std::string line_terminator(";{");

static std::basic_regex<char> EOL_regex("#/#[\r\n]",std::regex_constants::grep);
static std::basic_regex<char> BLANK_regex("#/#[ \t\f]",std::regex_constants::grep);
// Reserved Word regex
static std::basic_regex<char> IF_regex("#/#if",std::regex_constants::grep);
static std::basic_regex<char> ELSE_regex("#/#else",std::regex_constants::grep);
static std::basic_regex<char> WHILE_regex("#/#while",std::regex_constants::grep);
static std::basic_regex<char> FOR_regex("#/#for",std::regex_constants::grep);
static std::basic_regex<char> RETURN_regex("#/#return",std::regex_constants::grep);
static std::basic_regex<char> DO_regex("#/#do",std::regex_constants::grep);
static std::basic_regex<char> BREAK_regex("#/#break",std::regex_constants::grep);
static std::basic_regex<char> CONTINUE_regex("#/#continue",std::regex_constants::grep);

// Reserved Word Decl
static std::basic_regex<char> INTDECL_regex("#/#int",std::regex_constants::grep);
static std::basic_regex<char> STRDECL_regex("#/#str",std::regex_constants::grep);
static std::basic_regex<char> CHARDECL_regex("#/#char",std::regex_constants::grep);
static std::basic_regex<char> DOUBLEDECL_regex("#/#double",std::regex_constants::grep);
static std::basic_regex<char> TABDECL_regex("#/#tab",std::regex_constants::grep);

// End of File regex
static std::basic_regex<char> EOF_regex("#/##EOF#",std::regex_constants::grep);

// Symbol regex
static std::basic_regex<char> LPAREN_regex("#/#(",std::regex_constants::grep);
static std::basic_regex<char> RPAREN_regex("#/#)",std::regex_constants::grep);
static std::basic_regex<char> LBRACE_regex("#/#{",std::regex_constants::grep);
static std::basic_regex<char> RBRACE_regex("#/#}",std::regex_constants::grep);
static std::basic_regex<char> LBRCKT_regex("#/#\\[",std::regex_constants::grep);
static std::basic_regex<char> RBRCKT_regex("#/#\\]",std::regex_constants::grep);
static std::basic_regex<char> COMMA_regex("#/#\\,",std::regex_constants::grep);
static std::basic_regex<char> SEMICOLON_regex("#/#\\;",std::regex_constants::grep);
static std::basic_regex<char> COLON_regex("#/#\\:",std::regex_constants::grep);
static std::basic_regex<char> DOT_regex("#/#\\.",std::regex_constants::grep);

// Comparaison Operator (boolean)
static std::basic_regex<char> EQ_regex("#/#==",std::regex_constants::grep);
static std::basic_regex<char> NEQ_regex("#/#!=",std::regex_constants::grep);
static std::basic_regex<char> LT_regex("#/#<",std::regex_constants::grep);
static std::basic_regex<char> GT_regex("#/#>",std::regex_constants::grep);
static std::basic_regex<char> LE_regex("#/#<=",std::regex_constants::grep);
static std::basic_regex<char> GE_regex("#/#>=",std::regex_constants::grep);
static std::basic_regex<char> CAND_regex("#/#&&",std::regex_constants::grep);
static std::basic_regex<char> COR_regex("#/#||",std::regex_constants::grep);

// Binary Operator regex
static std::basic_regex<char> PLUS_regex("#/#\\+",std::regex_constants::grep);
static std::basic_regex<char> MINUS_regex("#/#-",std::regex_constants::grep);
static std::basic_regex<char> TIMES_regex("#/#\\*",std::regex_constants::grep);
static std::basic_regex<char> DIVIDE_regex("#/#/",std::regex_constants::grep);
static std::basic_regex<char> MODULE_regex("#/#%",std::regex_constants::grep);
static std::basic_regex<char> ASSIGN_regex("#/#=",std::regex_constants::grep);
static std::basic_regex<char> LSHIFT_regex("#/#<<",std::regex_constants::grep);
static std::basic_regex<char> RSHIFT_regex("#/#>>",std::regex_constants::grep);
static std::basic_regex<char> AND_regex("#/#&",std::regex_constants::grep);
static std::basic_regex<char> OR_regex("#/#|",std::regex_constants::grep);
static std::basic_regex<char> XOR_regex("#/#^",std::regex_constants::grep);
static std::basic_regex<char> COMP_regex("#/#~",std::regex_constants::grep);

// Identifiers
static std::basic_regex<char> ID_regex("#/#[a-zA-Z][_0-9a-zA-Z]*",std::regex_constants::egrep);

// Literals
static std::basic_regex<char> STR_regex("#/#\".*\"",std::regex_constants::nosubs);
static std::basic_regex<char> CHAR_regex("#/#'.'",std::regex_constants::nosubs);
static std::basic_regex<char> INT_regex("#/#([1-9][0-9]*|0)",std::regex_constants::egrep);
static std::basic_regex<char> DOUBLE_regex("#/#([1-9][0-9]*\\.[0-9]*|0\\.0)",std::regex_constants::egrep);

std::string read_line(FILE* f);
std::vector<Token> gen_tok_line(std::string line,size_t line_num, char* filename);
std::vector<Token> lexer(char* filename);
std::string gen_tok_string(std::vector<Token> tok);