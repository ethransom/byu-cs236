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
	};
  Parameter() {};

  bool operator ==(const Parameter &b) const {
    return (type == b.type && str == b.str);
  }

  bool operator !=(const Parameter &b) const {
    return (type != b.type || str != b.str);
  }

	void toString(std::ostringstream& out) const;

	void determineDomain(std::set<std::string>* domain) const;

	Token_type type;
	std::string str;
};

class Predicate {
public:
	std::string identifier;
	std::vector<Parameter> param_list;

  bool operator !=(const Predicate &p) const {
    return (identifier != p.identifier || param_list != p.param_list);
  }

	void toString(std::ostringstream& out) const;

	void determineDomain(std::set<std::string>* domain) const;
};

class Rule {
public:
	Predicate predicate;
	std::vector<Predicate> predicate_list;

	void toString(std::ostringstream& out) const;

	void determineDomain(std::set<std::string>* domain) const;
};

class DatalogProgram {
public:
	std::vector<Predicate> scheme_list;
	std::vector<Predicate> fact_list;
	std::vector<Rule> rule_list;
	std::vector<Predicate> query_list;

	std::set<std::string> domain;

	void determineDomain();

	std::string toString() const;
};
