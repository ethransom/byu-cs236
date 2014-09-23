#pragma once

#include "filestream.h"

/*
 * Implements a lexer. Generates a vector of Tokens based on the content of
 * an input file.
*/

class Scanner {
public:
	static void lex_file(Filestream *input);
};
