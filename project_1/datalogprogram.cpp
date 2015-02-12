#include "datalogprogram.h"

void Expression::collectDomain(std::set<std::string>& domain) const {
	left->collectDomain(domain);
	right->collectDomain(domain);
}

void Literal::collectDomain(std::set<std::string>& domain) const {
	domain.insert(str);
}

// std::ostream& Literal::operator<<(std::ostream& os) {
// 	os << str;

// 	return os;
// }

// std::ostream& Identifier::operator<<(std::ostream& os) {
// 	os << str;

// 	return os;
// }

std::ostream& operator<<(std::ostream& os, const Literal& literal) {
	os << literal.str;

	return os;
}

std::ostream& operator<<(std::ostream& os, const Identifier& identifier) {
	os << identifier.str;

	return os;
}

// std::ostream& Expression::operator<<(std::ostream& os) {
// 	os << left << type << right;

// 	return os;
// }

std::ostream& operator<<(std::ostream& os, const Predicate& predicate) {
	os << predicate.identifier << "(";

	for (size_t i = 0; i < predicate.param_list.size(); ++i) {
		if (i != 0)
			os << ",";
		predicate.param_list[i]->flatten(os);
	}

	os << ")";

	return os;
}

std::ostream& operator<<(std::ostream& os, const Scheme& scheme) {
	os << scheme.identifier << "(";

	for (size_t i = 0; i < scheme.param_list.size(); ++i) {
		if (i != 0)
			os << ",";
		scheme.param_list[i].flatten(os);
	}

	os << ")";

	return os;
}

std::ostream& operator<<(std::ostream& os, const Fact& fact) {
	os << fact.identifier << "(";

	for (size_t i = 0; i < fact.param_list.size(); ++i) {
		if (i != 0)
			os << ",";
		fact.param_list[i].flatten(os);
	}

	os << ")";

	return os;
}

std::ostream& operator<<(std::ostream& os, const Rule& rule) {
	os << rule.scheme << " :- ";

	for (size_t i = 0; i < rule.predicate_list.size(); ++i) {
		if (i != 0)
			os << ",";
		os << rule.predicate_list[i];
	}

	return os;
}

std::ostream& operator<<(std::ostream& os, const Query& query) {
	os << query.identifier << "(";

	for (size_t i = 0; i < query.param_list.size(); ++i) {
		if (i != 0)
			os << ",";
		query.param_list[i]->flatten(os);
	}

	os << ")";

	return os;
}

std::ostream& operator<<(std::ostream& os, const DatalogProgram& obj) {
	os << "Schemes(" << obj.scheme_list.size() << "):" << std::endl;
	for (auto i : obj.scheme_list) {
		os << "  " << i << std::endl;
	}

	os << "Facts(" << obj.fact_list.size() << "):" << std::endl;
	for (auto i : obj.fact_list) {
		os << "  " << i << "." << std::endl;
	}

	os << "Rules(" << obj.rule_list.size() << "):" << std::endl;
	for (auto i : obj.rule_list) {
		os << "  " << i << "." << std::endl;
	}

	os << "Queries(" << obj.query_list.size() << "):" << std::endl;
	for (auto i : obj.query_list) {
		os << "  " << i << "?" << std::endl;
	}

	return os;
}