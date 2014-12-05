#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <cstring>

#include "scanner.h"
#include "token.h"
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
	if (argc != 3) {
		std::cout << "You must provide an input and output file!" << std::endl;
		return 1;
	}

	std::streambuf * buf;
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

	// std::ofstream output(argv[2]);

	std::queue<Token> tokens;

	int lex_line_of_error = Scanner::lex_file(str, &tokens);

	if (lex_line_of_error > -1) {
		output << "Error during scanning on line " << lex_line_of_error << std::endl;
		return 1;
	}

	// for (std::vector<Token>::iterator i = tokens->begin(); i != tokens->end(); ++i) {
	// 	output << *i << std::endl;
	// }

	Parser parser(&tokens);

	DatalogProgram prog;
	try {
		parser.parse_tokens(&prog);
	} catch (ParseError e) {
		output << "Failure!" << std::endl << "  " << tokens.front() << std::endl;
		return 0;
	}

	// output << "Success!" << std::endl << prog.toString();

	prog->evaluate(&output);

	return 0;
}
