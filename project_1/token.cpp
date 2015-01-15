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

std::ostream& operator<<(std::ostream& os, const Token& obj) {
	os << "("
		<< Token_type_human_readable[obj.type] << ","
		<< "\"" << obj.str << "\"" << ","
		<< obj.line_num << ")";
	return os;
}
