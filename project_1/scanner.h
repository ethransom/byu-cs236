#pragma once

#include <vector>

#include "token.h"

/*
 * Implements a lexer. Generates a vector of Tokens based on the content of
 * an input file.
*/

class Scanner {
public:
	static std::vector<Token>* lex_file(std::string str);
};
