#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <cstring>

#include "token.h"
#include "lexer.h"
#include "parser.h"

std::string slurp(std::ifstream& in) {
    std::stringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

/*
 * Main entry point. Deals with parsing and validation of command line arguments
*/
int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "You must provide an input file" << std::endl;
		return 1;
	}

	std::string filename(argv[1]);
	std::ifstream input(filename);
	std::string str = slurp(input);

	Lexer lexer(&str);
	auto tokens = lexer.lex_file();

	// for (auto t : tokens) {
	// 	std::cout << t << std::endl;
	// }

	// std::cout << "Total Tokens = " << tokens.size() << std::endl;

	Parser parser(&tokens);

	DatalogProgram prog;
	Token* offending = parser.parse_tokens(prog);
	if (offending == NULL) {
		std::cout << "Failure!" << std::endl << "  " << offending << std::endl;
		return 0;
	}

	std::cout << "Success!" << std::endl << prog;

	return 0;
}
