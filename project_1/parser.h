#pragma once

#include <iostream>
#include <deque>
#include <vector>
#include <string>

#include "token.h"
#include "datalogprogram.h"

class Parser {
	std::deque<Token>* tokens;

	// does the current token have type 'type'?
	bool peek(Token_type token);
	// match the token and copy the string to dest
	bool match(Token_type token, std::string& dest);
	// match the token and discard the string
	bool match(Token_type token);

	// BEGIN PARSE RULES

	bool program(DatalogProgram& prog);
	bool scheme(Scheme&);
	bool scheme_list(std::vector<Scheme>&);
	bool id_list(std::vector<Identifier>&);
	bool fact(Fact&);
	bool fact_list(std::vector<Fact>&);
	bool rule(Rule&);
	bool rule_list(std::vector<Rule>&);
	bool head_predicate(Predicate&);
	bool predicate(Predicate&);
	bool predicate_list(std::vector<Predicate>&);
	bool parameter(Parameter*&);
	bool parameter_list(std::vector<Parameter*>&);
	bool expression(Expression&);
	bool binary_operator(char&);
	bool query(Query&);
	bool query_list(std::vector<Query>&);
	bool string_list(std::vector<Literal>& strings);

public:
	Parser(std::deque<Token>*);

	// use the tokens to populate the datalog program
	// note that this consumes tokens from the queue
	// returns NULL upon success
	// returns the first invalid token for syntax errors
	Token* parse_tokens(DatalogProgram& program);
};
