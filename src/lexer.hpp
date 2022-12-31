#pragma once

#include <iostream>
#include <regex>
#include <string>
#include <climits>
#include <unistd.h>

#include "token.hpp"

const std::string line_terminator(";{");

static std::basic_regex EOL_regex("#/#[\r\n]",std::regex_constants::grep);
static std::basic_regex BLANK_regex("#/#[ \t\f]",std::regex_constants::grep);
// Reserved Word regex
static std::basic_regex IF_regex("#/#if",std::regex_constants::grep);
static std::basic_regex ELSE_regex("#/#else",std::regex_constants::grep);
static std::basic_regex WHILE_regex("#/#while",std::regex_constants::grep);
static std::basic_regex FOR_regex("#/#for",std::regex_constants::grep);
static std::basic_regex RETURN_regex("#/#return",std::regex_constants::grep);
static std::basic_regex DO_regex("#/#do",std::regex_constants::grep);
static std::basic_regex BREAK_regex("#/#break",std::regex_constants::grep);
static std::basic_regex CONTINUE_regex("#/#continue",std::regex_constants::grep);

// Reserved Word Decl
static std::basic_regex INTDECL_regex("#/#int",std::regex_constants::grep);
static std::basic_regex STRDECL_regex("#/#str",std::regex_constants::grep);
static std::basic_regex CHARDECL_regex("#/#char",std::regex_constants::grep);
static std::basic_regex DOUBLEDECL_regex("#/#double",std::regex_constants::grep);
static std::basic_regex TABDECL_regex("#/#tab",std::regex_constants::grep);

// End of File regex
static std::basic_regex EOF_regex("#/##EOF#",std::regex_constants::grep);

// Symbol regex
static std::basic_regex LPAREN_regex("#/#(",std::regex_constants::grep);
static std::basic_regex RPAREN_regex("#/#)",std::regex_constants::grep);
static std::basic_regex LBRACE_regex("#/#{",std::regex_constants::grep);
static std::basic_regex RBRACE_regex("#/#}",std::regex_constants::grep);
static std::basic_regex LBRCKT_regex("#/#\\[",std::regex_constants::grep);
static std::basic_regex RBRCKT_regex("#/#\\]",std::regex_constants::grep);
static std::basic_regex COMMA_regex("#/#\\,",std::regex_constants::grep);
static std::basic_regex SEMICOLON_regex("#/#\\;",std::regex_constants::grep);
static std::basic_regex COLON_regex("#/#\\:",std::regex_constants::grep);
static std::basic_regex DOT_regex("#/#\\.",std::regex_constants::grep);

// Comparaison Operator (boolean)
static std::basic_regex EQ_regex("#/#==",std::regex_constants::grep);
static std::basic_regex NEQ_regex("#/#!=",std::regex_constants::grep);
static std::basic_regex LT_regex("#/#<",std::regex_constants::grep);
static std::basic_regex GT_regex("#/#>",std::regex_constants::grep);
static std::basic_regex LE_regex("#/#<=",std::regex_constants::grep);
static std::basic_regex GE_regex("#/#>=",std::regex_constants::grep);
static std::basic_regex CAND_regex("#/#&&",std::regex_constants::grep);
static std::basic_regex COR_regex("#/#||",std::regex_constants::grep);

// Binary Operator regex
static std::basic_regex PLUS_regex("#/#\\+",std::regex_constants::grep);
static std::basic_regex MINUS_regex("#/#-",std::regex_constants::grep);
static std::basic_regex TIMES_regex("#/#\\*",std::regex_constants::grep);
static std::basic_regex DIVIDE_regex("#/#/",std::regex_constants::grep);
static std::basic_regex MODULE_regex("#/#%",std::regex_constants::grep);
static std::basic_regex ASSIGN_regex("#/#=",std::regex_constants::grep);
static std::basic_regex LSHIFT_regex("#/#<<",std::regex_constants::grep);
static std::basic_regex RSHIFT_regex("#/#>>",std::regex_constants::grep);
static std::basic_regex AND_regex("#/#&",std::regex_constants::grep);
static std::basic_regex OR_regex("#/#|",std::regex_constants::grep);
static std::basic_regex XOR_regex("#/#^",std::regex_constants::grep);
static std::basic_regex COMP_regex("#/#~",std::regex_constants::grep);

// Identifiers
static std::basic_regex ID_regex("#/#[a-zA-Z][_0-9a-zA-Z]*",std::regex_constants::egrep);

// Literals
static std::basic_regex STR_regex("#/#\".*\"",std::regex_constants::nosubs);
static std::basic_regex CHAR_regex("#/#'.'",std::regex_constants::nosubs);
static std::basic_regex INT_regex("#/#([1-9][0-9]*|0)",std::regex_constants::egrep);
static std::basic_regex DOUBLE_regex("#/#([1-9][0-9]*\\.[0-9]*|0\\.0)",std::regex_constants::egrep);


std::string read_line(FILE* f);
std::vector<Token> gen_tok_line(std::string line,size_t line_num, char* filename);
std::vector<Token> lexer(char* filename);