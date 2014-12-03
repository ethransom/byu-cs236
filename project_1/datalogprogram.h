#pragma once

#include <vector>
#include <set>
#include <sstream>

#include "token.h"

class Parameter {
public:
	Parameter(Token* token) {
		type = token->type;
		str = token->str;
	}

	std::string toString();

	void determineDomain(std::set<std::string>* domain);

	Token_type type;
	std::string str;
};

class Predicate {
public:
	std::string identifier;
	std::vector<Parameter*> param_list;

	~Predicate() {
		for (auto param : param_list)
			delete param;
	}

	std::string toString();

	void determineDomain(std::set<std::string>* domain);
};

class Rule {
public:
	Predicate* predicate;
	std::vector<Predicate*> predicate_list;

	~Rule() {
		delete predicate;

		for (auto p : predicate_list)
			delete p;
	}

	std::string toString();

	void determineDomain(std::set<std::string>* domain);
};

// class Query : public Predicate {};

class DatalogProgram {
public:
	std::vector<Predicate*> scheme_list;
	std::vector<Predicate*> fact_list;
	std::vector<Rule*> rule_list;
	std::vector<Predicate*> query_list;

	std::set<std::string> domain;

	~DatalogProgram();

	void determineDomain();

	std::string toString();
};
