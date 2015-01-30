#pragma once

#include <deque>
#include <functional>
#include <tuple>
#include <vector>

#include "token.h"
#include "seeker.h"

/*
 * Implements a lexer. Generates a vector of Tokens based on the content of
 * an input file.
*/

class Lexer {
	Seeker input;
	std::deque<Token> tokens;

	static std::vector<std::pair<bool (*) (Seeker&), Token_type>> automata;

	// AUTOMATONs
	static bool read_newline(Seeker input);

	static bool read_whitespace(Seeker input);

	static bool read_multiline_comment(Seeker input);

	static bool read_comment(Seeker input);

	static bool read_comma(Seeker input);

	static bool read_period(Seeker input);

	static bool read_q_mark(Seeker input);

	static bool read_left_paren(Seeker input);

	static bool read_right_paren(Seeker input);

	static bool read_multiply(Seeker input);

	static bool read_add(Seeker input);

	static bool read_colon_dash(Seeker input);

	static bool read_colon(Seeker input);

	static bool read_string(Seeker input);

	static bool read_schemes(Seeker input);

	static bool read_facts(Seeker input);

	static bool read_rules(Seeker input);

	static bool read_queries(Seeker input);

	static bool read_identifier(Seeker input);
	// END AUTOMATONS

	// add a token with the given data to the output
	Token add(Token_type t);
public:
	// construct a Lexer, use `str` as input, putting the tokens in token_vec
	Lexer(std::string* str) : input(str) {};

	// lex the given string, overwrite token_vec to point to a vector of tokens
	// return the line of an error, or -1 if successful
	std::deque<Token> lex_file();
};
