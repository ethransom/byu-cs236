#pragma once

#include <vector>
#include <ostream>
#include <set>

#include "token.h"

class Parameter {
public:
	void collectDomain(std::set<std::string>&) const {};

	virtual void flatten(std::ostream& os) const = 0;
	// virtual std::ostream& operator<<(std::ostream& os) = 0;
};

class Expression : public Parameter {
public:
	Parameter* left;
	char type;
	Parameter* right;

	void collectDomain(std::set<std::string>&) const;

	virtual void flatten(std::ostream& os) const { left->flatten(os); os << type; right->flatten(os); };
	// virtual std::ostream& operator<<(std::ostream& os);
};

class Identifier : public Parameter {
public:
	void collectDomain(std::set<std::string>&) const {};
	std::string str;

	virtual void flatten(std::ostream& os) const { os << str; };
	// virtual std::ostream& operator<<(std::ostream& os);

	friend std::ostream& operator<<(std::ostream& os, const Identifier&);
};

class Literal : public Parameter {
public:
	void collectDomain(std::set<std::string>&) const;
	std::string str;

	virtual void flatten(std::ostream& os) const { os << str; };
	// virtual std::ostream& operator<<(std::ostream& os);

	friend std::ostream& operator<<(std::ostream& os, const Literal&);
};

class Predicate {
public:
	std::string identifier;
	std::vector<Parameter*> param_list;
	// ^ yes, using a pointer is breaking from convention, but polymorphism

	friend std::ostream& operator<<(std::ostream& os, const Predicate& obj);
};

class Scheme {
public:
	std::string identifier;
	std::vector<Identifier> param_list;

	friend std::ostream& operator<<(std::ostream& os, const Scheme& s);
};

class Fact {
public:
	std::string identifier;
	std::vector<Literal> param_list;

	// friend std::ostream& operator<<(std::ostream& os, const Fact& f);
};

class Rule {
public:
	Scheme scheme;
	std::vector<Predicate> predicate_list;

	// friend std::ostream& operator<<(std::ostream& os, const Rule& obj);
};

class Query {
public:
	std::string identifier;
	std::vector<Parameter*> param_list;
};

class DatalogProgram {
public:
	std::vector<Scheme> scheme_list;
	std::vector<Fact> fact_list;
	std::vector<Rule> rule_list;
	std::vector<Query> query_list;

	// void flatten(std::ostream&);

	friend std::ostream& operator<<(std::ostream& os, const DatalogProgram& obj);
};
