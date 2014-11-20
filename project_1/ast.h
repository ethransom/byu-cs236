#pragma once

#include <vector>

#include "token.h"

class Predicate {
public:
	std::string identifier;
	std::vector<Token*> param_list;

	~Predicate() {
		std::cout << "CLearing predicate" << std::endl;
		for (int i = 0; i < param_list.size(); i++)
			delete param_list[i];
	}
};

// class Scheme : public Predicate {
// };

// class Fact : public Predicate {};

class Rule {
public:
	Predicate predicate;
	std::vector<Predicate*> predicate_list;
	~Rule() {
		std::cout << "CLearing rule" << std::endl;
		for (int i = 0; i < predicate_list.size(); i++)
			delete predicate_list[i];
	}
};

// class Query : public Predicate {};

class AST {
public:
	std::vector<Predicate*> scheme_list;
	std::vector<Predicate*> fact_list;
	std::vector<Rule*> rule_list;
	std::vector<Predicate*> query_list;

	~AST();
	
	friend std::ostream& operator<<(std::ostream& os, const AST& ast);
};
