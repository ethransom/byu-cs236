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
#include "database.h"
#include "relation.h"

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
	bool dump_ast = false;
	char* filename = NULL;

	// arg parsing loop
	for (int i = 1; i < argc; ++i) {
		if (!strcmp(argv[i], "--tokens")) {
			dump_tokens = true;
		} else if (!strcmp(argv[i], "--ast")) {
			dump_ast = true;
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

	if (dump_ast) {
		std::cout << "Success!" << std::endl;
		std::cout << prog;

		std::cout << collector;

		return 0;
	}

	Database db;

	for (auto scheme : prog.scheme_list) {
		db.create(scheme);
	}

	for (auto fact : prog.fact_list) {
		db.insert(fact);
	}

	bool added;
	int passes = 0;
	do {
		added = false;
		std::cout << "Beginning pass...";
		for (auto rule : prog.rule_list) {
			if (db.interpret(rule))
				added = true;
		}
		std::cout << "done." << std::endl;
		passes++;
	} while (added && passes < 5);

	std::cout << "Schemes populated after " << passes
		<< " passes through the Rules." << std::endl;

	for (auto& query : prog.query_list) {
		auto relation = db.query(query);

		std::cout << query << "?";

		if (relation.size() > 0) {
			std::cout << " Yes(" << relation.size() << ")" << std::endl;
			if (relation.is_dirty())
				relation.print(std::cout);
		} else {
			std::cout << " No" << std::endl;
		}
	}

	return 0;
}
