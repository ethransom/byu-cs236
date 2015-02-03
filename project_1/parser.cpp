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
		tokens->pop_front();
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
		tokens->pop_front();
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

bool Parser::program(DatalogProgram& prog) {
	require(accept(SCHEMES));
	require(accept(COLON));
	require(scheme_list(prog.scheme_list));

	require(accept(FACTS));
	require(accept(COLON));
	require(fact_list(prog.fact_list));

	require(accept(RULES));
	require(accept(COLON));
	require(rule_list(prog.rule_list));

	require(accept(QUERIES));
	require(accept(COLON));
	require(query_list(prog.query_list));

	if (tokens->size() != 0) {
		throw ParseError();
	}

	return true;
}

// bool Parser::scheme_list(std::vector<Predicate>& out) {
// 	// accept at least one predicate
// 	require(predicate(out));

// 	while (predicate(out));

// 	return true;
// }

bool Parser::fact_list(std::vector<Predicate>& out) {
	Predicate p;

	// possibly accept a fact
	while(fact(p))
		out.push_back(p); // MOVE

	return true;
}

bool Parser::fact(Predicate& p) {
	expect(predicate(p));
	require(accept(PERIOD));

	return true;
}

// bool Parser::rule_list(std::vector<Rule>& out) {
// 	while(rule(out));

// 	return true;
// }

// bool Parser::rule(std::vector<Rule>& out) {
// 	Rule rule();

// 	expect(predicate(rule.predicate()));
// 	require(accept(COLON_DASH));
// 	require(predicate_list(rule.predicate_list));
// 	require(accept(PERIOD));

// 	out.push_back(rule);

// 	return true;
// }

// bool Parser::query_list(std::vector<Predicate>& out) {
// 	Predicate query;
// 	require(query(query));
// 	list.push_back(query);

// 	while (query(query)) {
// 		list.push_back(query);
// 	}

// 	return true;
// }

// bool Parser::query(Predicate& out) {
// 	expect(predicate(out));
// 	require(accept(Q_MARK));

// 	return true;
// }

// bool Parser::predicate_list(std::vector<Predicate>&) {
// 	std::vector<Predicate*> list;

// 	auto p = predicate();
// 	require(p);
// 	list.push_back(p);

// 	while(accept(COMMA)) {
// 		auto p = predicate();
// 		require(p);
// 		list.push_back(p);
// 	}

// 	return true;
// }

bool Parser::predicate(Predicate&) {
	expect(accept(ID, &p.identifier));
	require(accept(LEFT_PAREN));

	require(parameter_list(p.param_list));

	require(accept(RIGHT_PAREN));

	return true;
}

bool Parser::parameter_list(std::vector<Parameter>& out) {
	std::vector<Parameter> params;

	// accept at least one paramters, separated by commas
	require(parameter(out));

	while (accept(COMMA)) {
		require(parameter(out));
	}

	return true;
}

bool Parser::parameter(std::vector<Parameter>& out) {
	Token token = tokens->front();
	if (token.type == ID || token.type == STRING) {
		tokens->pop_front();
		out.emplace_back(&token);
		return true;
	} else {
		return false;
	}
}

// PUBLIC METHODS

Parser::Parser(std::deque<Token>* t) {
	tokens = t;
}

Token* Parser::parse_tokens(DatalogProgram& prog) {
	if (!program(prog)) {
		return &tokens->front();
	}

	prog.determineDomain();

	return NULL;
}
