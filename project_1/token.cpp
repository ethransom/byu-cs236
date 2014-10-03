#include "token.h"

// this must exactly match the order of Token_type
extern const std::string Token_type_human_readable[] = {
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

std::ostream& operator << (std::ostream& os, const Token& token) {
	os << "("
		<< Token_type_human_readable[token.type] << ","
		<< "\"" << token.str << "\"" << ","
		<< token.line_num << ")";
	return os;
}
