#pragma once

#include <vector>
#include <ostream>
#include <set>
#include <exception>

#include "token.h"

enum ParamType {
	EXPRESSION,
	IDENTIFIER,
	LITERAL
};

class Parameter;

class Expression {
public:
	Parameter* left;
	char type;
	Parameter* right;

	void collectDomain(std::set<std::string>&) const;

	// void flatten(std::ostream& os) const { left->flatten(os); os << type; right->flatten(os); };
	void flatten(std::ostream& os) const;

	void eval();
};

class Identifier {
public:
	void collectDomain(std::set<std::string>&) const {};
	std::string str;

	void flatten(std::ostream& os) const { os << str; };

	friend std::ostream& operator<<(std::ostream& os, const Identifier&);

	void eval();
};

class Literal {
public:
	void collectDomain(std::set<std::string>&) const;
	std::string str;

	void flatten(std::ostream& os) const { os << str; };

	friend std::ostream& operator<<(std::ostream& os, const Literal&);

	void eval();
};

class Parameter {
public:
	Parameter(ParamType t) {
		type = t;
		switch (t) {
			case EXPRESSION:
				new(&expression) Expression();
				break;
			case IDENTIFIER:
				new(&identifier) Identifier();
				break;
			case LITERAL:
				new(&literal) Literal();
				break;
		}
	};
	ParamType type;
	union {
		Expression expression;
		Identifier identifier;
		Literal literal;
	};

	void collectDomain(std::set<std::string>&) const {};

	void flatten(std::ostream& os) const;

	void eval();
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
	friend std::ostream& operator<<(std::ostream& os, const Query& obj);
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
