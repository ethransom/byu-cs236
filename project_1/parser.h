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
	std::deque<Token>* tokens;

	void error();
	bool accept(Token_type token);
	bool accept(Token_type token, std::string* dest);

	// BEGIN PARSE RULES

	// all rules passed a ref to the object they're supposed to parse
	// all rules return success status as boolean
	// rules won't modify ref if unsuccessful
	// TODO: ^ good idea? actually true?

	bool program(DatalogProgram& prog);
	bool scheme_list(std::vector<Predicate>&);
	bool fact_list(std::vector<Predicate>&);
	bool fact(Predicate&);
	bool rule_list(std::vector<Rule>&);
	bool rule(Rule&);
	bool query_list(std::vector<Predicate>&);
	bool query(Predicate&);
	bool predicate_list(std::vector<Predicate>&);
	bool predicate(Predicate&);
	bool parameter_list(std::vector<Parameter>&);
	bool parameter(Parameter&);

public:
	Parser(std::deque<Token>*);

	// use the tokens to populate the datalog program
	// note that this consumes tokens from the queue
	// returns NULL upon success
	// returns the first invalid token for syntax errors
	Token* parse_tokens(DatalogProgram& program);
};
