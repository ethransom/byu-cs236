#include <cctype>
#include <vector>

#include "lexer.h"

Token Lexer::add(Token_type t) {
	Token token(t, input.slice(), input.get_current_line());
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

bool Lexer::read_whitespace(Seeker& input) {
	return isspace(input.read());
}

bool Lexer::read_multiline_comment(Seeker& input) {
	// multi-line comments
	if (input.read_string("#|")) {
		while (!input.read_string("|#"))
			input.read();

		return true;
	}

	return false;
}

bool Lexer::read_comment(Seeker& input) {
	if (input.read() == '#') {
		while (input.peek() != '\n') input.read();

		// bring us just to the edge of the newline
		input.read();

		return true;
	}
	
	return false;
}

// punctuation
bool Lexer::read_comma(Seeker& input) {
	return input.read() == ',';
}
bool Lexer::read_period(Seeker& input) {
	return input.read() == '.';
}
bool Lexer::read_q_mark(Seeker& input) {
	return input.read() == '?';
}
bool Lexer::read_left_paren(Seeker& input) {
	return input.read() == '(';
}
bool Lexer::read_right_paren(Seeker& input) {
	return input.read() == ')';
}
bool Lexer::read_multiply(Seeker& input) {
	return input.read() == '*';
}
bool Lexer::read_add(Seeker& input) {
	return input.read() == '+';
}
bool Lexer::read_colon_dash(Seeker& input) {
	return input.read_string(":-"); // one of these things is not like the other
}
bool Lexer::read_colon(Seeker& input) {
	return input.read() == ':';
}

bool Lexer::read_string(Seeker& input) {
	if (input.read() != '\'') return false;

jklol:
	char c;
	while ((c = input.read()) != '\'') {

			input.read(); // consume 

		// if (c == '\n') line_delta++;
	}

	// two single quotes do not count. why? nobody knows
	// did we erroneously exit on the first of two? yes? keep going!
	if (input.peek() == '\'') {
		input.read(); // consume the extra quote
		goto jklol;
	}
	
	return true;
}

bool Lexer::read_schemes(Seeker& input) {
	return input.read_string("Schemes");
}

bool Lexer::read_facts(Seeker& input) {
	return input.read_string("Facts");
}

bool Lexer::read_rules(Seeker& input) {
	return input.read_string("Rules");
}

bool Lexer::read_queries(Seeker& input) {
	return input.read_string("Queries");
}

bool Lexer::read_identifier(Seeker& input) {
	if (isalpha(input.read())) {
		while (isalnum(input.read()));

		return true;
	} else {
		return false;
	}
}



std::vector<std::pair<bool (*) (Seeker&), Token_type>> Lexer::automata {
	// punctuation
	{read_comma, COMMA},
	{read_period, PERIOD},
	{read_q_mark, Q_MARK},
	{read_left_paren, LEFT_PAREN},
	{read_right_paren, RIGHT_PAREN},
	{read_colon_dash, COLON_DASH},
	{read_colon, COLON},
	{read_multiply, MULTIPLY},
	{read_add, ADD},

	{read_queries, QUERIES},
	{read_schemes, SCHEMES},
	{read_rules, RULES},
	{read_facts, FACTS},


	{read_identifier, ID},

	{read_string, STRING},

	{read_multiline_comment, COMMENT},
	{read_comment, COMMENT},

	{read_whitespace, NONE},
};

// returns tokens read from the Lexer's input
std::deque<Token> Lexer::lex_file() {
	while (!input.end_of_file()) {
		try {
			std::pair<bool (*) (Seeker&), Token_type> winner;
			uint winner_len = 0;

			for (auto pair : Lexer::automata) {
				auto automaton = pair.first;
				automaton(input);
				Token token = add(pair.second);

				uint len = token.str.length();
				if (len < winner_len) {
					winner_len = len;
					winner = pair;
				}
				
				input.reset();
			}


			if (winner_len == 0) {
				// if we get to this point, we have an undefined character
				input.read(); // consume the mysterious character
				throw InputException();
			}

			auto automaton = winner.first;
			automaton(input);
			Token winning_token = add(winner.second);
			if (winner.second != NONE) {
				tokens.push_back(winning_token);
			}
			for (uint i = 0; i < winning_token.str.length(); ++i)
				input.read();
			input.commit();
		} catch (const InputException& e) {
			add(UNDEFINED);
			input.commit();
		}
	}

	// always end with the EOF token
	add(END);

	return tokens;
}


