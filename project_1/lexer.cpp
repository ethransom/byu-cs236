#include <cctype>
#include <vector>

#include "lexer.h"

Token Lexer::add(Token_type t) {
	// the token's line number is the line where the token starts, not ends
	// we need to get the line before we call commit
	uint line = input.get_current_line();
	Token token(t, input.commit(), line);
	tokens.push_back(token);

	return token;
}

/* ====================================================== */
/* =================== BEGIN AUTOMATA =================== */
/* ====================================================== */

// all automata attempt to read a token.
// if successful, they return true, possibly perfoming additional actions.
// if false, they return false.
// the input may be in an invalid state, which is why the main loop resets it
// after every automaton.

bool Lexer::read_whitespace() {
	return isspace(input.read());
}

bool Lexer::read_multiline_comment() {
	// multi-line comments
	if (input.read_string("#|")) {
		while (!(input.check() == '|' && input.peek() == '#'))
			input.read();

		input.read_string("|#"); // consume the close comment

		return true;
	}

	return false;
}

bool Lexer::read_comment() {
	if (input.read() == '#') {
		while (input.check() != '\n') input.read();

		return true;
	}

	return false;
}

// punctuation
bool Lexer::read_comma() {
	return input.read() == ',';
}
bool Lexer::read_period() {
	return input.read() == '.';
}
bool Lexer::read_q_mark() {
	return input.read() == '?';
}
bool Lexer::read_left_paren() {
	return input.read() == '(';
}
bool Lexer::read_right_paren() {
	return input.read() == ')';
}
bool Lexer::read_multiply() {
	return input.read() == '*';
}
bool Lexer::read_add() {
	return input.read() == '+';
}
bool Lexer::read_colon_dash() {
	return input.read_string(":-"); // one of these things is not like the other
}
bool Lexer::read_colon() {
	return input.read() == ':';
}

bool Lexer::read_string() {
	if (input.read() != '\'') return false;

jklol:
	while (input.read() != '\'');

	// two single quotes do not count. why? nobody knows
	// did we erroneously exit on the first of two? yes? keep going!
	if (input.check() == '\'') {
		input.read(); // consume the extra quote
		goto jklol;
	}

	return true;
}

bool Lexer::read_schemes() {
	return input.read_string("Schemes");
}

bool Lexer::read_facts() {
	return input.read_string("Facts");
}

bool Lexer::read_rules() {
	return input.read_string("Rules");
}

bool Lexer::read_queries() {
	return input.read_string("Queries");
}

bool Lexer::read_identifier() {
	if (isalpha(input.read())) {
		while (isalnum(input.check())) input.read();

		std::string str = input.slice();
		if (str == "Queries" || str == "Rules" ||
			str == "Facts" || str == "Schemes") return false;

		return true;
	} else {
		return false;
	}
}

/* ====================================================== */
/* ==================== END AUTOMATA ==================== */
/* ====================================================== */

#define TRY_AUTOMATON(automaton, type) \
	if (automaton) { \
		if (type != NONE) { add(type); } \
		else { input.commit(); } \
		continue; \
	} else input.reset();

// returns tokens read from the Lexer's input
std::deque<Token> Lexer::lex_file() {
	while (!input.end_of_file()) {
		try {
			// punctuation
			TRY_AUTOMATON(read_comma(), COMMA);
			TRY_AUTOMATON(read_period(), PERIOD);
			TRY_AUTOMATON(read_q_mark(), Q_MARK);
			TRY_AUTOMATON(read_left_paren(), LEFT_PAREN);
			TRY_AUTOMATON(read_right_paren(), RIGHT_PAREN);
			TRY_AUTOMATON(read_colon_dash(), COLON_DASH);
			TRY_AUTOMATON(read_colon(), COLON);
			TRY_AUTOMATON(read_multiply(), MULTIPLY);
			TRY_AUTOMATON(read_add(), ADD);

			// it's weird that this comes first, because Schemes, et all are
			// also identifiers. However, "SchemesQueries" is an identifier,
			// and we must read it as such. read_identifier() returns false if
			// the identifier it has read is a keyword
			TRY_AUTOMATON(read_identifier(), ID);

			TRY_AUTOMATON(read_queries(), QUERIES);
			TRY_AUTOMATON(read_schemes(), SCHEMES);
			TRY_AUTOMATON(read_rules(), RULES);
			TRY_AUTOMATON(read_facts(), FACTS);

			TRY_AUTOMATON(read_string(), STRING);

			TRY_AUTOMATON(read_multiline_comment(), COMMENT);
			TRY_AUTOMATON(read_comment(), COMMENT);

			TRY_AUTOMATON(read_whitespace(), NONE);

			// if we get to this point, we have an undefined character
			input.read(); // consume the mysterious character
			throw InputException();
		} catch (const InputException& e) {
			add(UNDEFINED);
		}
	}

	// always end with the EOF token
	add(END);

	return tokens;
}


