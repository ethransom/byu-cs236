#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <exception>

#include "token.h"
#include "datalogprogram.h"

class ParseError : public std::exception {
	const char* what() const throw();
};

class Parser {
	std::queue<Token>* tokens;

	void error();
	bool accept(Token_type token);
	bool accept(Token_type token, std::string* dest);

	DatalogProgram* program();
	std::vector<Predicate*> scheme_list();
	std::vector<Predicate*> fact_list();
	Predicate* fact();
	std::vector<Rule*> rule_list();
	Rule* rule();
	std::vector<Predicate*> query_list();
	Predicate* query();
	std::vector<Predicate*> predicate_list();
	Predicate* predicate();
	std::vector<Parameter*> parameter_list();
	Parameter* parameter();

public:
	Parser(std::queue<Token>*);

	DatalogProgram* parse_tokens();
};
