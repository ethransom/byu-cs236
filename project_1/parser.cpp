#include "parser.h"

#include <cstdlib>
#include <string>

const char* ParseError::what() const throw() {
	return "a parsing error occured";
}

extern std::string Token_type_human_readable[];

bool Parser::accept(Token_type type) {
	if (tokens->size() < 1) return false;

	Token token = tokens->front();
	// std::cout << "checking " << Token_type_human_readable[token.type]
		// << " with " << Token_type_human_readable[type] << std::endl;
	if (token.type == type) {
		tokens->pop();
		return true;
	} else {
		return false;
	}
}

bool Parser::accept(Token_type type, std::string* dest) {
	if (tokens->size() < 1) return false;

	Token token = tokens->front();
	// std::cout << "checking " << Token_type_human_readable[token.type]
		// << " with " << Token_type_human_readable[type] << std::endl;
	if (token.type == type) {
		tokens->pop();
		*dest = token.str;
		return true;
	} else {
		return false;
	}
}

// if this isn't there, that's okay
#define expect(val) if (!val) return NULL

// very bad if this isn't there
#define require(val) if (!val) throw ParseError();

void Parser::program(DatalogProgram* prog) {
	require(accept(SCHEMES));
	require(accept(COLON));
	prog->scheme_list = scheme_list();

	require(accept(FACTS));
	require(accept(COLON));
	prog->fact_list = fact_list();

	require(accept(RULES));
	require(accept(COLON));
	prog->rule_list = rule_list();

	require(accept(QUERIES));
	require(accept(COLON));
	prog->query_list = query_list();

	if (tokens->size() != 0) {
		throw ParseError();
	}
}

std::vector<std::unique_ptr<Predicate>> Parser::scheme_list() {
	std::vector<std::unique_ptr<Predicate>> list;

	// accept at least one predicate
	auto p = predicate();
	require(p);
	list.push_back(p);

	while ((p = predicate())) {
		list.push_back(p);
	}

	return list;
}

std::vector<std::unique_ptr<Predicate>> Parser::fact_list() {
	std::vector<std::unique_ptr<Predicate>> list;
	std::unique_ptr<Predicate> p;

	// possibly accept a fact
	while((p = fact())) {
		list.push_back(p);
	}

	return list;
}

std::unique_ptr<Predicate> Parser::fact() {
	auto p = predicate();
	expect(p);
	require(accept(PERIOD));

	return p;
}

std::vector<Rule*> Parser::rule_list() {
	std::vector<Rule*> list;
	Rule* r;

	while((r = rule())) {
		list.push_back(r);
	}

	return list;
}

Rule* Parser::rule() {
	auto p = predicate();
	expect(p);
	require(accept(COLON_DASH));
	auto list = predicate_list();
	// require(list);
	require(accept(PERIOD));

	Rule* rule = new Rule();
	rule->predicate = p;
	rule->predicate_list = list;

	return rule;
}

std::vector<std::unique_ptr<Predicate>> Parser::query_list() {
	std::vector<std::unique_ptr<Predicate>> list;
	std::unique_ptr<Predicate> p;

	p = query();
	require(p);
	list.push_back(p);

	while ((p = query())) {
		list.push_back(p);
	}

	return list;
}

std::unique_ptr<Predicate> Parser::query() {
	auto p = predicate();
	expect(p);
	require(accept(Q_MARK));

	return p;
}

std::vector<std::unique_ptr<Predicate>> Parser::predicate_list() {
	std::vector<std::unique_ptr<Predicate>> list;

	auto p = predicate();
	require(p);
	list.push_back(p);

	while(accept(COMMA)) {
		auto p = predicate();
		require(p);
		list.push_back(p);
	}

	return list;
}

std::unique_ptr<Predicate> Parser::predicate() {
	std::unique_ptr<Predicate> predicate(new Predicate());

	expect(accept(ID, &predicate->identifier));

	require(accept(LEFT_PAREN));

	p->param_list = parameter_list();

	require(accept(RIGHT_PAREN));

	return p;
}

std::vector<Parameter*> Parser::parameter_list() {
	std::vector<Parameter*> params;

	// accept at least one paramters, separated by commas
	auto t = parameter();
	require(t);
	params.push_back(t);

	while (accept(COMMA)) {
		auto t = parameter();
		require(t);
		params.push_back(t);
	}

	return params;
}

Parameter* Parser::parameter() {
	Token token = tokens->front();
	if (token.type == ID || token.type == STRING) {
		tokens->pop();
		auto t = new Parameter(&token);
		return t;
	} else {
		return NULL;
	}
}

// PUBLIC METHODS

Parser::Parser(std::queue<Token>* t) {
	tokens = t;
}

void Parser::parse_tokens(DatalogProgram* prog) {
	program(prog);

	prog->determineDomain();
}
