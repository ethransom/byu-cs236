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

DatalogProgram Parser::program() {
	DatalogProgram prog;

	require(accept(SCHEMES));
	require(accept(COLON));
  scheme_list(&prog.scheme_list);

	require(accept(FACTS));
	require(accept(COLON));
	fact_list(&prog.fact_list);

	require(accept(RULES));
	require(accept(COLON));
	rule_list(&prog.rule_list);

	require(accept(QUERIES));
	require(accept(COLON));
	prog.query_list = query_list();

	if (tokens->size() != 0) {
		throw ParseError();
	}

	return prog;
}

void Parser::scheme_list(std::vector<Predicate>* list) {
	// accept at least one predicate
  Predicate p;
	require(predicate(&p));
	list->push_back(p);
  p.param_list.clear();


	while (predicate(&p)) {
		list->push_back(p);
    p.param_list.clear();
	}
}

void Parser::fact_list(std::vector<Predicate>* list) {
	Predicate p;

	// possibly accept a fact
	while(fact(&p)) {
		list->push_back(p);
    p.param_list.clear();
	}
}

bool Parser::fact(Predicate* p) {
	expect(predicate(p));
	require(accept(PERIOD));

  return true;
}

void Parser::rule_list(std::vector<Rule>* list) {
	Rule r;

	while(rule(&r)) {
		list->push_back(r);
	}
}

bool Parser::rule(Rule* rule) {
	Predicate p;
	expect(predicate(&p));
	require(accept(COLON_DASH));
	auto list = predicate_list();

	require(accept(PERIOD));

	rule->predicate = p;
	rule->predicate_list = list;

	return true;
}

std::vector<Predicate> Parser::query_list() {
	std::vector<Predicate> list;
	Predicate p;

	require(query(&p));
	list.push_back(p);

	while (query(&p)) {
		list.push_back(p);
	}

	return list;
}

bool Parser::query(Predicate* p) {
	expect(predicate(p));
	require(accept(Q_MARK));

	return true;
}

std::vector<Predicate> Parser::predicate_list() {
	std::vector<Predicate> list;

  Predicate p;
	require(predicate(&p));
	list.push_back(p);

	while(accept(COMMA)) {
    Predicate p;
    require(predicate(&p));
    list.push_back(p);
	}

	return list;
}

bool Parser::predicate(Predicate* p) {
	expect(accept(ID, &p->identifier));

	require(accept(LEFT_PAREN));

	parameter_list(&p->param_list);

	require(accept(RIGHT_PAREN));

	return true;
}

void Parser::parameter_list(std::vector<Parameter>* params) {
  Parameter param;
	// accept at least one paramters, separated by commas
	require(parameter(&param));
	params->push_back(param);

	while (accept(COMMA)) {
    Parameter param;
    require(parameter(&param));
		params->push_back(param);
	}
}

bool Parser::parameter(Parameter* p) {
	Token token = tokens->front();
	if (token.type == ID || token.type == STRING) {
		tokens->pop();
    Parameter param(&token);
		*p = param;
    return 1;
	} else {
		return NULL;
	}
}

// PUBLIC METHODS

Parser::Parser(std::queue<Token>* t) {
	tokens = t;
}

DatalogProgram Parser::parse_tokens() {
	auto prog = program();

	prog.determineDomain();

	return prog;
}
