#include <regex>

// Regex for IF ELSE:
static std::basic_regex<char> pIF_regex("#/#IF LPAREN",std::regex_constants::grep);
static std::basic_regex<char> pELSEPART_regex("#/#ELSE",std::regex_constants::grep);

// Regex for LOOPS:
static std::basic_regex<char> pWHILE_regex("#/#WHILE LPAREN",std::regex_constants::grep);

static std::basic_regex<char> pASSIGN_regex("#/#ID\\{[a-zA-Z][_0-9a-zA-Z]*\\} ASSIGN",std::regex_constants::grep);

static std::basic_regex<char> pLPAREN_regex("#/#LPAREN",std::regex_constants::grep);
static std::basic_regex<char> pRPAREN_regex("#/#RPAREN",std::regex_constants::grep);

static std::basic_regex<char> pVARDECL_regex("#/#DECL{(int|double|string|tab)} ID\\{[a-zA-Z][_0-9a-zA-Z]*\\}",std::regex_constants::egrep);
static std::basic_regex<char> pOP_regex("#/#(LPAREN )*(INT\\{[0-9]*\\}|DOUBLE\\{([1-9][0-9]*\\.[0-9]*|0\\.0)\\}|ID\\{[a-zA-Z][_0-9a-zA-Z]*\\}) (RPAREN )*((PLUS|MINUS|TIMES|DIVIDE|MODULE|LSHIFT|RSHIFT|AND|XOR|COMP|EQ|NEQ|LT|GT|LE|GE|CAND|COR) (LPAREN )*(INT\\{[0-9]*\\}|DOUBLE\\{([1-9][0-9]*\\.[0-9]*|0\\.0)\\}|ID\\{[a-zA-Z][_0-9a-zA-Z]*\\}) (RPAREN )*)*",std::regex_constants::egrep);

static std::basic_regex<char> pIDENTIFER_regex("ID\\{[a-zA-Z][_0-9a-zA-Z]*\\}",std::regex_constants::egrep);
static std::basic_regex<char> pINTEGERLITERAL_regex("INT\\{[0-9]*\\}",std::regex_constants::egrep);
static std::basic_regex<char> pSTRINGLITERAL_regex("STR\\{\".*\"\\}",std::regex_constants::egrep|std::regex_constants::nosubs);