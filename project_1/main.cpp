#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <cstring>

#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "datalogprogram.h"
#include "domain_collector.h"

std::string slurp(std::ifstream& in) {
    std::stringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

/*
 * Main entry point. Deals with parsing and validation of command line arguments
*/
int main(int argc, char** argv) {
	bool dump_tokens = false;
	char* filename = NULL;

	// arg parsing loop
	for (int i = 1; i < argc; ++i) {
		if (!strcmp(argv[i], "--tokens")) {
			dump_tokens = true;
		} else {
			if (filename != NULL) {
				std::cout << "Too many input files!" << std::endl;
				return 0;
			}
			filename = argv[i];
		}
	}

	if (filename == NULL) {
		std::cout << "You must provide an input file" << std::endl;
		return 1;
	}

	std::string filestr(filename);
	std::ifstream input(filestr);
	std::string str = slurp(input);

	Lexer lexer(&str);
	auto tokens = lexer.lex_file();

	if (dump_tokens) {
		for (auto t : tokens) {
			std::cout << t << std::endl;
		}

		std::cout << "Total Tokens = " << tokens.size() << std::endl;

		return 0;
	}

	Parser parser(&tokens);

	DatalogProgram prog;
	Token* offending = parser.parse_tokens(prog);
	if (offending != NULL) {
		std::cout << "Failure!" << std::endl << "  " << *offending << std::endl;
		return 0;
	}

	DomainCollector collector;
	collector.collect(prog);

	std::cout << "Success!" << std::endl;
	std::cout << prog;

	std::cout << collector;

	return 0;
}
