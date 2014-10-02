#include <iostream>
#include <cctype>

#include "scanner.h"

static const std::string ALPHA_NUMERIC = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456789";

bool read_until(std::string* input, std::string delimiter, std::string* output) {
	size_t pos = 0;
	std::string token;
	pos = input->find(delimiter);

	if (pos != std::string::npos) {
		token = input->substr(0, pos);
		input->erase(0, pos+delimiter.length());
		*output = token;
		return true;
	} else {
		return false;
	}
}

bool read_string(std::string* input, std::string value, std::string* output) {
	size_t pos = 0;
	std::string token;
	pos = input->find(value);

	if (pos == 0) { // input must start with desired string
		token = input->substr(0, value.length());
		input->erase(0, value.length());
		*output = token;
		return true;
	} else {
		return false;
	}
}

std::vector<Token>* Scanner::lex_file(std::string str) {
	int line = 1;
	auto token_vec = new std::vector<Token>;

	while (str.length() != 0) {
		std::string output;
		Token_type type;
		if (read_string(&str, "\n", &output)) {
			line++;
			continue; // newlines aren't a token
		} else if ( // non-newline whitespace
			read_string(&str, "\t", &output) ||
			read_string(&str, " ", &output)
		) {
			continue;
		} else if (read_string(&str, "#", &output)) { // comments
			// TODO: make sure this handles EOF
			read_until(&str, "\n", &output);
			continue;
		} else if (read_string(&str, ",", &output)) {
			type = COMMA;
		} else if (read_string(&str, ".", &output)) {
			type = PERIOD;
		} else if (read_string(&str, "?", &output)) {
			type = Q_MARK;
		} else if (read_string(&str, "(", &output)) {
			type = LEFT_PAREN;
		} else if (read_string(&str, ")", &output)) {
			type = RIGHT_PAREN;
		} else if (read_string(&str, ":-", &output)) {
			type = COLON_DASH;
		} else if (read_string(&str, ":", &output)) {
			type = COLON;
		} else if (read_string(&str, ".", &output)) {
			type = PERIOD;
		} else if (read_string(&str, "'", &output)) { // strings
			type = STRING;
			// TODO: Make sure string doesn't have newlines
			if (read_until(&str, "'", &output)) {
				// string ended
			} else {
				// string did not end
				std::cout << "Input Error on line " << line << std::endl;
				return NULL;
			}
		} else if (isalpha(str.at(0))) { // identifiers
			size_t pos = str.find_first_not_of(ALPHA_NUMERIC);
			output = str.substr(0, pos);
			str.erase(0, pos);

			if (output == "Schemes") type = SCHEMES;
			else if (output == "Facts") type = FACTS;
			else if (output == "Rules") type = RULES;
			else if (output == "Queries") type = QUERIES;
			else type = ID;
		} else {
			std::cout << "Input Error on line " << line << std::endl;
			return NULL;
		}

		Token token(type, output, line);
		token_vec->push_back(token);
	}

	return token_vec;
}

/*
if (read_string(&str, "\n", &output)) {
			line++;
		}
		*/