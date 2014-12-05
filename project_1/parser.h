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

	DatalogProgram program();
	void scheme_list(std::vector<Predicate>* list);
	void fact_list(std::vector<Predicate>* list);
	bool fact(Predicate* p);
	void rule_list(std::vector<Rule>* list);
	bool rule(Rule* rule);
	std::vector<Predicate> query_list();
	bool query(Predicate* p);
	std::vector<Predicate> predicate_list();
	bool predicate(Predicate* predicate);
	void parameter_list(std::vector<Parameter>* params);
	bool parameter(Parameter* p);

public:
	Parser(std::queue<Token>*);

	DatalogProgram parse_tokens();
};
