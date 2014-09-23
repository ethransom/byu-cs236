#include <iostream>

#include "scanner.h"
#include "filestream.h"

/*
 * Main entry point. Deals with parsing and validation of command line arguments
*/
int main() {
	Filestream* fs = new Filestream("input.txt");

	Scanner::lex_file(fs);

	delete fs;

	return 0;
}
