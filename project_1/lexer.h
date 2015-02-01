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

	// AUTOMATONs
	bool read_newline();

	bool read_whitespace();

	bool read_multiline_comment();

	bool read_comment();

	bool read_comma();

	bool read_period();

	bool read_q_mark();

	bool read_left_paren();

	bool read_right_paren();

	bool read_multiply();

	bool read_add();

	bool read_colon_dash();

	bool read_colon();

	bool read_string();

	bool read_schemes();

	bool read_facts();

	bool read_rules();

	bool read_queries();

	bool read_identifier();
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
