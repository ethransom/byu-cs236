#pragma once

#include <iostream>
#include <vector>
#include <exception>

#include "token.h"
#include "ast.h"

class ParseError : public std::exception {
	const char* what() const throw();
};

class Parser {
	std::vector<Token>* tokens;

	void error();
	bool accept(Token_type token);
	bool accept(Token_type token, std::string* dest);

	AST* program();
	std::vector<Predicate*> scheme_list();
	std::vector<Predicate*> fact_list();
	Predicate* fact();
	std::vector<Rule*> rule_list();
	Rule* rule();
	std::vector<Predicate*> query_list();
	Predicate* query();
	std::vector<Predicate*> predicate_list();
	Predicate* predicate();
	std::vector<Token*> parameter_list();
	Token* parameter();

public:
	Parser(std::vector<Token>*);

	AST* parse_tokens();
};
