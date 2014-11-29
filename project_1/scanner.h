#pragma once

#include <queue>

#include "token.h"

/*
 * Implements a lexer. Generates a vector of Tokens based on the content of
 * an input file.
*/

class Scanner {
public:
	// lex the given string, overwrite token_vec to point to a vector of tokens
	// return the line of an error, or -1 if successful
	static int lex_file(std::string str, std::queue<Token>* token_vec);
};
