#pragma once

#include <iostream>
#include <ostream>
#include <string>

typedef enum {
	COMMA,
	PERIOD,
	Q_MARK,
	LEFT_PAREN,
	RIGHT_PAREN,
	COLON,
	COLON_DASH,
	SCHEMES,
	FACTS,
	RULES,
	QUERIES,
	ID,
	STRING,
} Token_type;

class Token {
public:
	Token(Token_type t, std::string s, int l) {
		type = t;
		str = s;
		line_num = l;
	}

	Token() {};

	// print the token in the pattern '(TYPE, "string value", line_number)'
	friend std::ostream& operator<<(std::ostream& os, const Token& token);

	Token_type type;
	std::string str;
	int line_num;
};
