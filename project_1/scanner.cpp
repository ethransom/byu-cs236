#include <iostream>
#include <cctype>

#include "scanner.h"

static const std::string ALPHA_NUMERIC = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

class InputException : std::exception {};

Scanner::Scanner(std::string* i) {
	input = i;
	line = 1;
	commit_pos = 0;
	seek_pos = 0;
}

bool Scanner::read_until(std::string delimiter) {
	size_t pos = input->find(delimiter, seek_pos);
	std::string token;

	if (pos != std::string::npos) {
		seek_pos = pos;
		return true;
	} else {
		return false;
	}
}

bool Scanner::read_past(std::string delimiter) {
	if (!read_until(delimiter)) return false;

	// seek_pos is at the beginning of the delimeter, put it at the end
	seek_pos += delimiter.length();

	return true;
}

bool Scanner::read_string(std::string value) {
	// CRIT: remove inefficiency
	std::string token = input->substr(seek_pos, value.length());

	if (token == value) { // input must start with desired string
		seek_pos += value.length();
		return true;
	} else {
		return false;
	}
}

bool Scanner::read_identifier(Token_type* type) {
	if (isalpha(input->at(seek_pos))) {
		size_t pos = input->find_first_not_of(ALPHA_NUMERIC, seek_pos);
		std::string output = input->substr(seek_pos, (pos - seek_pos));
		seek_pos = pos;

		if (output == "Schemes") *type = SCHEMES;
		else if (output == "Facts") *type = FACTS;
		else if (output == "Rules") *type = RULES;
		else if (output == "Queries") *type = QUERIES;
		else *type = ID;

		return true;
	} else {
		return false;
	}
}

bool Scanner::read_punctuation(Token_type* type) {
	if (read_string(",")) {
		*type = COMMA;
	} else if (read_string(".")) {
		*type = PERIOD;
	} else if (read_string("?")) {
		*type = Q_MARK;
	} else if (read_string("(")) {
		*type = LEFT_PAREN;
	} else if (read_string(")")) {
		*type = RIGHT_PAREN;
	} else if (read_string("*")) {
		*type = MULTIPLY;
	} else if (read_string("+")) {
		*type = ADD;
	} else if (read_string(":-")) {
		*type = COLON_DASH;
	} else if (read_string(":")) {
		*type = COLON;
	} else if (read_string(".")) {
		*type = PERIOD;
	} else {
		return false;
	}

	return true;
}

std::string Scanner::commit() {
	// TODO: string slices?
	std::string output = input->substr(commit_pos, (seek_pos - commit_pos));

	commit_pos = seek_pos;

	return output;
}

void Scanner::add(Token_type t) {
	Token token(t, commit(), line);
	tokens.push_back(token);
}

bool Scanner::end_of_file() {
	return (seek_pos + 1) > input->length();
}

char Scanner::peek() {
	if (end_of_file())
		throw InputException();

	return input->at(seek_pos + 1);
}

char Scanner::read() {
	char c = peek();
	seek_pos++;

	return c;
}

// returns tokens read from the scanner's input
std::vector<Token> Scanner::lex_file() {
	while (!end_of_file()) {
		try {
			Token_type type;

			if (read_string("\n")) {
				line++;
				commit();
				continue;
			}

			if (isspace(input->at(seek_pos))) {
				seek_pos++;
				commit();
				continue;
			}

			// multi-line comments
			if (read_string("#|")) {
				for (char c; read_string("|#"); c = read())
					if (c == '\n') line++;

				read_past("|#");

				type = COMMENT;
			}
			// single-line comments
			else if (read_string("#")) {
				// TODO: make sure this handles EOF
				read_until("\n");
				add(COMMENT);

				// consume the newline character
				line++;
				seek_pos++;
				commit();

				continue;
			} else if (read_punctuation(&type)) { // punctuation
				// do nothing, output and type have been populated
			} else if (read_string("'")) { // strings
				// for whatever reason, we consider the line number of the
				// string to be where it starts
				// thus, we need to keep a local delta
				uint line_delta = line;

				char c;
				while ((c = read()) != '\'') {

					// two single quotes do not count. why? nobody knows
					if (peek() == '\'')
						read(); // consume

					if (c == '\n') line_delta++;
				}

				add(STRING);
				line  = line_delta;
				continue;
			} else if (read_identifier(&type)) {
				// do nothing
			} else {
				seek_pos++; // consume the mysterious character
				throw InputException();
			}

			add(type);
		} catch (const InputException& e) {
			add(UNDEFINED);
		}
	}

	// add the EOF token
	add(END);

	return tokens;
}
