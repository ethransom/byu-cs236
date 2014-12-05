#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <exception>
#include <memory>

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

	void program(DatalogProgram* prog);
	std::vector<std::unique_ptr<Predicate>> scheme_list();
	std::vector<std::unique_ptr<Predicate>> fact_list();
	std::unique_ptr<Predicate> fact();
	std::vector<Rule*> rule_list();
	Rule* rule();
	std::vector<std::unique_ptr<Predicate>> query_list();
	std::unique_ptr<Predicate> query();
	std::vector<std::unique_ptr<Predicate>> predicate_list();
	std::unique_ptr<Predicate> predicate();
	std::vector<Parameter*> parameter_list();
	Parameter* parameter();

public:
	Parser(std::queue<Token>*);

	void parse_tokens(DatalogProgram* prog);
};
