#pragma once

#include <iostream>
#include <string>

typedef enum {
	COMMA,
	PERIOD,
	Q_MARK,
	LEFT_PAREN,
	RIGHT_PAREN,
	MULTIPLY,
	ADD,
	COLON,
	COLON_DASH,
	SCHEMES,
	FACTS,
	RULES,
	QUERIES,
	ID,
	STRING,
	COMMENT,
	END, // apparently EOF is reserved in C++
	UNDEFINED,
} Token_type;

class Token {
public:
	Token(Token_type t, std::string s, int l) {
		type = t;
		str = s;
		line_num = l;
	}

	Token_type type;
	std::string str;
	int line_num;
};

std::ostream& operator<<(std::ostream& os, const Token& obj);
