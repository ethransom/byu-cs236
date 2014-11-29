#include "parser.h"

#include <cstdlib>
#include <string>

const char* ParseError::what() const throw() {
	return "a parsing error occured";
}

extern std::string Token_type_human_readable[];

bool Parser::accept(Token_type type) {
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
	Token token = tokens->front();
	std::cout << "checking " << Token_type_human_readable[token.type]
		<< " with " << Token_type_human_readable[type] << std::endl;
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

AST* Parser::program() {
	AST* ast = new AST();
	require(accept(SCHEMES));
	require(accept(COLON));
	ast->scheme_list = scheme_list();

	require(accept(FACTS));
	require(accept(COLON));
	ast->fact_list = fact_list();

	require(accept(RULES));
	require(accept(COLON));
	ast->rule_list = rule_list();

	require(accept(QUERIES));
	require(accept(COLON));
	ast->query_list = query_list();

	return ast;
}

std::vector<Predicate*> Parser::scheme_list() {
	std::vector<Predicate*> list;

	// accept at least one predicate
	Predicate* p = predicate();
	require(p);
	list.push_back(p);

	while (p = predicate()) {
		list.push_back(p);
	}

	return list;
}

std::vector<Predicate*> Parser::fact_list() {
	std::vector<Predicate*> list;
	Predicate* p;

	// possibly accept a fact
	while(p = fact()) {
		list.push_back(p);
	}

	return list;
}

Predicate* Parser::fact() {
	auto p = predicate();
	expect(p);
	require(accept(PERIOD));

	return p;
}

std::vector<Rule*> Parser::rule_list() {
	std::vector<Rule*> list;
	Rule* r;

	while(r = rule()) {
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
	rule->predicate = *p;
	delete p;
	rule->predicate_list = list;

	return rule;
}

std::vector<Predicate*> Parser::query_list() {
	std::vector<Predicate*> list;
	Predicate* p;

	p = query();
	require(p);
	list.push_back(p);

	while (p = query()) {
		list.push_back(p);
	}

	return list;
}

Predicate* Parser::query() {
	auto p = predicate();
	expect(p);
	require(accept(Q_MARK));

	return p;
}

std::vector<Predicate*> Parser::predicate_list() {
	std::vector<Predicate*> list;

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

Predicate* Parser::predicate() {
	auto p = new Predicate();
	expect(accept(ID, &p->identifier));
	require(accept(LEFT_PAREN));
	auto list = parameter_list();
	// require(list);
	p->param_list = list;
	require(accept(RIGHT_PAREN));

	return p;
}

std::vector<Token*> Parser::parameter_list() {
	std::vector<Token*> tokens;

	// accept at least one paramters, separated by commas
	auto t = parameter();
	require(t);
	tokens.push_back(t);

	while (accept(COMMA)) {
		auto t = parameter();
		require(t);
		tokens.push_back(t);
	}

	return tokens;
}

Token* Parser::parameter() {
	Token token = tokens->front();
	if (token.type == ID || token.type == STRING) {
		tokens->pop();
		auto t = new Token();
		*t = token;
		return t;
	} else {
		return NULL;
	}
}

// PUBLIC METHODS

Parser::Parser(std::queue<Token>* t) {
	tokens = t;
}

AST* Parser::parse_tokens() {
	AST* ast = program();
	return ast;
}
