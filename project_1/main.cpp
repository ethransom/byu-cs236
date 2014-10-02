#include <iostream>
#include <fstream>
#include <sstream>

#include "scanner.h"
#include "token.h"

std::string slurp(std::ifstream& in) {
    std::stringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

/*
 * Main entry point. Deals with parsing and validation of command line arguments
*/
int main(int argc, char** argv) {
	for (int i = 1; i < argc; ++i)
	{
		std::string filename(argv[i]);
		std::ifstream input(filename);

		std::string str = slurp(input);

		std::vector<Token> *tokens = Scanner::lex_file(str);

		if (tokens == NULL) return 1;

		for (std::vector<Token>::iterator i = tokens->begin(); i != tokens->end(); ++i) {
			i->print();
		}

		std::cout << "Total Tokens = " << tokens->size() << std::endl;
	}

	return 0;
}
