#include <unordered_map>
#include "ast.hpp"

typedef std::unordered_map<std::string, Decl*> scope;
std::vector<scope> scopes;
std::vector<FunDecl> function;
std::vector<Loop> loop;
void scope_push();
void scope_pop();
scope actuel();
void verif(Decl decl);
Decl cherche(std::string);