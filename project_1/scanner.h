#pragma once

#include <queue>

#include "token.h"

/*
 * Implements a lexer. Generates a vector of Tokens based on the content of
 * an input file.
*/

class Scanner {
	std::string* input;
	std::vector<Token> tokens;
	uint line;
	uint commit_pos;
	uint seek_pos;

	bool read_until(std::string delimiter);
	bool read_past(std::string delimiter);
	bool read_string(std::string value);

	inline bool read_identifier(Token_type* type);
	inline bool read_punctuation(Token_type* type);

	// advances the commit_pos to the seek_pos, and fills output with
	// the chars in between.
	inline std::string commit();

	// add a token with the given data to the output
	void add(Token_type t);

	// have we exhausted our input?
	bool end_of_file();

	// peeks ahead by 1 character
	char peek();

	// reads 1 character, advances seek_pos
	char read();
public:
	// construct a scanner, use `input` as input, putting the tokens in token_vec
	Scanner(std::string* input);

	// lex the given string, overwrite token_vec to point to a vector of tokens
	// return the line of an error, or -1 if successful
	std::vector<Token> lex_file();
};
