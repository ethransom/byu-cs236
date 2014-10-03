#pragma once

#include <vector>

#include "token.h"

class Predicate {
public:
	std::string identifier;
	std::vector<Token*> param_list;
};

// class Scheme : public Predicate {
// };

// class Fact : public Predicate {};

class Rule {
public:
	Predicate predicate;
	std::vector<Predicate*> predicate_list;
};

// class Query : public Predicate {};

class AST {
public:
	std::vector<Predicate*> scheme_list;
	std::vector<Predicate*> fact_list;
	std::vector<Rule*> rule_list;
	std::vector<Predicate*> query_list;
	
	friend std::ostream& operator<<(std::ostream& os, const AST& ast);
};
