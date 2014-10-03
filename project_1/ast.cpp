#include "ast.h"

std::ostream& operator << (std::ostream& os, const AST& ast) {
	os << "HELLO IDSLJKF" << std::endl;
	// os << "Schemes(" << ast.schemes_list.size() << "):" << std::endl;
	// for (std::vector<Scheme>::iterator i = ast.schemes_list.begin(); i != ast.schemes_list.end(); ++i) {
		// os << "scheme!" << std::endl;
	// }

	// os << "Facts(" << ast.facts_list.size() << "):" << std::endl;
	// for (std::vector<Fact>::iterator i = ast.facts_list.begin(); i != ast.facts_list.end(); ++i) {
	// 	os << "fact!" << std::endl;
	// }

	// os << "Rules(" << ast.rules_list.size() << "):" << std::endl;
	// for (std::vector<Rule>::iterator i = ast.rules_list.begin(); i != ast.rules_list.end(); ++i) {
	// 	os << "rule!" << std::endl;
	// }

	// os << "Queries(" << ast.queries_list.size() << "):" << std::endl;
	// for (std::vector<Query>::iterator i = ast.queries_list.begin(); i != ast.queries_list.end(); ++i) {
	// 	os << "query!" << std::endl;
	// }

	return os;
}