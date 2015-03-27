#include "parser.h"

// #include <cstdlib>

extern std::string Token_type_human_readable[];

bool Parser::peek(Token_type type) {
	return tokens->front().type == type;
}

bool Parser::match(Token_type type, std::string& dest) {
	Token token = tokens->front();
	// std::cout << "checking " << Token_type_human_readable[token.type]
		// << " with " << Token_type_human_readable[type] << std::endl;
	if (token.type == type) {
		tokens->pop_front();
		dest = token.str;
		return true;
	} else {
		return false;
	}
}

bool Parser::match(Token_type type) {
	if (tokens->front().type == type) {
		tokens->pop_front();
		return true;
	} else {
		return false;
	}
}

// very bad if this isn't there
#define require(val) if (!val) return false;

bool Parser::program(DatalogProgram& prog) {
	prog.scheme_list.emplace_back();
	Scheme& first_scheme = prog.scheme_list.back();
	prog.query_list.emplace_back();
	Query& first_query = prog.query_list.back();

	return (
		match(SCHEMES) &&
		match(COLON) &&
		scheme(first_scheme) &&
		scheme_list(prog.scheme_list) &&

		match(FACTS) &&
		match(COLON) &&
		fact_list(prog.fact_list) &&

		match(RULES) &&
		match(COLON) &&
		rule_list(prog.rule_list) &&

		match(QUERIES) &&
		match(COLON) &&
		query(first_query) &&
		query_list(prog.query_list) &&

		match(END)
	);
}

bool Parser::scheme(Scheme& scheme) {
	if (peek(ID)) {
		scheme.param_list.emplace_back();
		Identifier& first_param = scheme.param_list.back();

		return (
			match(ID, scheme.identifier) &&
			match(LEFT_PAREN) &&
			match(ID, first_param.str) &&
			id_list(scheme.param_list) &&
			match(RIGHT_PAREN)
		);
	}

	return false;
}

bool Parser::scheme_list(std::vector<Scheme>& schemes) {
	if (peek(ID)) {
		schemes.emplace_back();
		Scheme& s = schemes.back();
		return (
			scheme(s) &&
			scheme_list(schemes)
		);
	}

	if (peek(FACTS)) {
		return true;
	}

	return false;
}

bool Parser::id_list(std::vector<Identifier>& list) {
	if (peek(COMMA)) {
		list.emplace_back();
		Identifier& str = list.back();
		return (
			match(COMMA) &&
			match(ID, str.str) &&
			id_list(list)
		);
	}

	if (peek(RIGHT_PAREN)) {
		return true;
	}

	return false;
}

bool Parser::fact(Fact& fact) {
	if (peek(ID)) {
		fact.param_list.emplace_back();
		Literal& first_param = fact.param_list.back();
		return (
			match(ID, fact.identifier) &&
			match(LEFT_PAREN) &&
			match(STRING, first_param.str) &&
			string_list(fact.param_list) &&
			match(RIGHT_PAREN) &&
			match(PERIOD)
		);
	}

	return false;
}

bool Parser::fact_list(std::vector<Fact>& facts) {
	if (peek(ID)) {
		facts.emplace_back();
		Fact& f = facts.back();

		return (
			fact(f) &&
			fact_list(facts)
		);
	}

	if (peek(RULES)) {
		return true;
	}

	return false;
}

bool Parser::rule(Rule& rule) {
	if (peek(ID)) {
		rule.predicate_list.emplace_back();
		Predicate& first_predicate = rule.predicate_list.back();

		return (
			scheme(rule.scheme) &&
			match(COLON_DASH) &&
			predicate(first_predicate) &&
			predicate_list(rule.predicate_list) &&
			match(PERIOD)
		);
	}

	return false;
}

bool Parser::rule_list(std::vector<Rule>& rules) {
	if (peek(ID)) {
		rules.emplace_back();
		Rule& first = rules.back();

		return (
			rule(first) &&
			rule_list(rules)
		);
	}

	if (peek(QUERIES)) {
		return true;
	}

	return false;
}

// bool Parser::head_predicate(Predicate& predicate) {
// 	if (peek(ID)) {
// 		Identifier* first_id = new Identifier();
// 		predicate.param_list.push_back(first_id);

// 		return (
// 			match(ID, predicate.identifier) &&
// 			match(LEFT_PAREN) &&
// 			match(ID, first_id->str) &&
// 			id_list(predicate.param_list) &&
// 			match(RIGHT_PAREN)
// 		);
// 	}

// 	return false;
// }

bool Parser::predicate(Predicate& predicate) {
	if (peek(ID)) {
		predicate.param_list.emplace_back();
		Parameter*& first_param = predicate.param_list.back();

		return (
			match(ID, predicate.identifier) &&
			match(LEFT_PAREN) &&
			parameter(first_param) &&
			parameter_list(predicate.param_list) &&
			match(RIGHT_PAREN)
		);
	}

	return false;
}

bool Parser::predicate_list(std::vector<Predicate>& predicates) {
	if (peek(COMMA)) {
		predicates.emplace_back();
		Predicate& first = predicates.back();

		return (
			match(COMMA) &&
			predicate(first) &&
			predicate_list(predicates)
		);
	}

	if (peek(PERIOD)) {
		return true;
	}

	return false;
}

bool Parser::parameter(Parameter*& parameter) {
	if (peek(STRING)) {
		// Literal* literal = new Literal();
		parameter = new Parameter(LITERAL);
		// parameter->type = LITERAL;
		// parameter->literal();
		return (
			match(STRING, parameter->literal.str)
		);
	}

	if (peek(ID)) {
		// Identifier* identifier = new Identifier();
		parameter = new Parameter(IDENTIFIER);
		// parameter->type = IDENTIFIER;
		// parameter->identifier();
		return (
			match(ID, parameter->identifier.str)
		);
	}

	if (peek(LEFT_PAREN)) {
		// Expression* expr = new Expression();
		parameter = new Parameter(EXPRESSION);
		// parameter->type = EXPRESSION;
		// parameter->expression();
		return (
			expression(parameter->expression)
		);
	}

	return false;
}

bool Parser::parameter_list(std::vector<Parameter*>& parameters) {
	if (peek(COMMA)) {
		parameters.emplace_back();
		Parameter*& param = parameters.back();

		return (
			match(COMMA) &&
			parameter(param) &&
			parameter_list(parameters)
		);
	}

	if (peek(RIGHT_PAREN)) {
		return true;
	}

	return false;
}

bool Parser::expression(Expression& expression) {
	if (peek(LEFT_PAREN)) {
		return (
			match(LEFT_PAREN) &&
			parameter(expression.left) &&
			binary_operator(expression.type) &&
			parameter(expression.right) &&
			match(RIGHT_PAREN)
		);
	}

	return false;
}

bool Parser::binary_operator(char& op) {
	if (match(ADD)) {
		op = '+';

		return true;
	}

	if (match(MULTIPLY)) {
		op = '*';

		return true;
	}

	return false;
}

bool Parser::query(Query& query) {
	if (peek(ID)) {
		query.param_list.emplace_back();
		Parameter*& param = query.param_list.back();

		// TODO: call to predicate() instead?
		return (
			match(ID, query.identifier) &&
			match(LEFT_PAREN) &&
			parameter(param) &&
			parameter_list(query.param_list) &&
			match(RIGHT_PAREN) &&
			match(Q_MARK)
		);
	}

	return false;
}

bool Parser::query_list(std::vector<Query>& out) {
	if (peek(ID)) {
		out.emplace_back();
		Query& first = out.back();
		return (
			query(first) &&
			query_list(out)
		);
	}

	if (peek(END)) {
		return true;
	}

	return false;
}

bool Parser::string_list(std::vector<Literal>& strings) {
	if (peek(COMMA)) {
		strings.emplace_back();
		Literal& str = strings.back();

		return (
			match(COMMA) &&
			match(STRING, str.str) &&
			string_list(strings)
		);
	}

	if (peek(RIGHT_PAREN)) {
		return true;
	}
	return false;
}

// PUBLIC METHODS

Parser::Parser(std::deque<Token>* t) {
	tokens = t;
}

Token* Parser::parse_tokens(DatalogProgram& prog) {
	if (!program(prog)) {
		return &tokens->front();
	}

	return NULL;
}
