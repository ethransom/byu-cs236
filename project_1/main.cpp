#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

#include "lexer.h"
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
	if (argc != 3) {
		std::cout << "You must provide an input and output file!" << std::endl;
		return 1;
	}

	std::streambuf* buf;
	std::ofstream of;

	if (!strcmp(argv[2], "-")) {
		buf = std::cout.rdbuf();
	} else {
		of.open(argv[2]);
		buf = of.rdbuf();
	}

	std::ostream output(buf);

	std::string filename(argv[1]);
	std::ifstream input(filename);
	std::string str = slurp(input);

	Lexer lexer(&str);
	auto tokens = lexer.lex_file();

	for (auto t : tokens) {
		output << t << std::endl;
	}

	output << "Total Tokens = " << tokens.size() << std::endl;

	return 0;
}
