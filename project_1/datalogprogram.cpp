#include "datalogprogram.h"

std::string Predicate::toString() {
	std::ostringstream os;
	os << identifier << "(";

	for (int i = 0; i < param_list.size(); i++) {
		os << param_list[i]->toString();

		if ((i + 1) != param_list.size()) {
			// last iteration
			os << ",";
		}
	}

	os << ")";

	return os.str();
}

void Predicate::determineDomain(std::set<std::string>* domain) {
	for (auto param : param_list) {
		param->determineDomain(domain);
	}
}

std::string Rule::toString() {
	std::ostringstream os;
	os << predicate->toString() << " :- ";

	for (int i = 0; i < predicate_list.size(); i++) {
		os << predicate_list[i]->toString();

		if ((i + 1) != predicate_list.size()) {
			// last iteration
			os << ",";
		}
	}

	return os.str();
}

void Rule::determineDomain(std::set<std::string>* domain) {
	for (std::vector<Predicate*>::iterator i = predicate_list.begin(); i != predicate_list.end(); ++i) {
		(*i)->determineDomain(domain);
	}
}

std::string Parameter::toString() {
	std::ostringstream os;

	if (type == STRING) {
		os << "'" << str << "'";
	} else if (type == ID) {
		os << str;
	}

	return os.str();
}

void Parameter::determineDomain(std::set<std::string>* domain) {
	if (type == STRING) {
		domain->insert(str);
	}
}

DatalogProgram::~DatalogProgram() {
	for (auto s : scheme_list) {
		delete s;
	}
	scheme_list.clear();

	for (auto f : fact_list) {
		delete f;
	}
	fact_list.clear();

	for (auto r : rule_list) {
		delete r;
	}
	rule_list.clear();

	for (auto q : query_list) {
		delete q;
	}
	query_list.clear();
}

void DatalogProgram::determineDomain() {
	for (int i = 0; i < scheme_list.size(); i++)
		scheme_list[i]->determineDomain(&domain);

	for (int i = 0; i < fact_list.size(); i++)
		fact_list[i]->determineDomain(&domain);

	for (int i = 0; i < rule_list.size(); i++)
		rule_list[i]->determineDomain(&domain);

	for (int i = 0; i < query_list.size(); i++)
		query_list[i]->determineDomain(&domain);
}

std::ostream& DatalogProgram::toString(std::ostream& os, const DatalogProgram& obj) {
	os << "Schemes(" << scheme_list.size() << "):" << std::endl;
	for (int i = 0; i < scheme_list.size(); i++) {
		os << "  " << scheme_list[i]->toString() << std::endl;
	}

	os << "Facts(" << fact_list.size() << "):" << std::endl;
	for (int i = 0; i < fact_list.size(); i++) {
		os << "  " << fact_list[i]->toString() << std::endl;
	}

	os << "Rules(" << rule_list.size() << "):" << std::endl;
	for (int i = 0; i < rule_list.size(); i++) {
		os << "  " << rule_list[i]->toString() << std::endl;
	}

	os << "Queries(" << query_list.size() << "):" << std::endl;
	for (int i = 0; i < query_list.size(); i++) {
		os << "  " << query_list[i]->toString() << std::endl;
	}

	os << "Domain(" << domain.size() << "):" << std::endl;
	for (auto str : domain) {
		os << "  '" << str << "'" << std::endl;
	}

	return os;
}