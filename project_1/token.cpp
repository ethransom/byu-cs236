#include "token.h"

// this must exactly match the order of Token_type
std::string Token_type_human_readable[] = {
	"COMMA",
	"PERIOD",
	"Q_MARK",
	"LEFT_PAREN",
	"RIGHT_PAREN",
	"COLON",
	"COLON_DASH",
	"SCHEMES",
	"FACTS",
	"RULES",
	"QUERIES",
	"ID",
	"STRING",
};

void Token::print() {
	std::cout << "("
		<< Token_type_human_readable[type] << ","
		<< "\"" << str << "\"" << ","
		<< line_num << ")" << std::endl;
}
