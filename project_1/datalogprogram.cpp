#include "datalogprogram.h"

std::ostream& operator<<(std::ostream& os, const Predicate& obj) {
	os << obj.identifier << "(";

	for (uint i = 0; i < obj.param_list.size(); i++) {
		os << obj.param_list[i];

		if ((i + 1) != obj.param_list.size()) {
			// last iteration
			os << ",";
		}
	}

	os << ")";

	return os;
}

void Predicate::determineDomain(std::set<std::string>* domain) {
	for (auto param : param_list) {
		param.determineDomain(domain);
	}
}

std::ostream& operator<<(std::ostream& os, const Rule& obj) {
	os << obj.predicate << " :- ";

	for (uint i = 0; i < obj.predicate_list.size(); i++) {
		os << obj.predicate_list[i];

		if ((i + 1) != obj.predicate_list.size()) {
			// last iteration
			os << ",";
		}
	}

	return os;
}

void Rule::determineDomain(std::set<std::string>* domain) {
	for (auto p : predicate_list) {
		p.determineDomain(domain);
	}
}

std::ostream& operator<<(std::ostream& os, const Parameter& obj) {
	if (obj.type == STRING) {
		os << "'" << obj.str << "'";
	} else if (obj.type == ID) {
		os << obj.str;
	}

	return os;
}

void Parameter::determineDomain(std::set<std::string>* domain) {
	if (type == STRING) {
		domain->insert(str);
	}
}

void DatalogProgram::determineDomain() {
	for (auto i : scheme_list)
		i.determineDomain(&domain);

	for (auto i : fact_list)
		i.determineDomain(&domain);

	for (auto i : rule_list)
		i.determineDomain(&domain);

	for (auto i : query_list)
		i.determineDomain(&domain);
}

std::ostream& operator<<(std::ostream& os, const DatalogProgram& obj) {
	os << "Schemes(" << obj.scheme_list.size() << "):" << std::endl;
	for (auto i : obj.scheme_list) {
		os << "  " << i << std::endl;
	}

	os << "Facts(" << obj.fact_list.size() << "):" << std::endl;
	for (auto i : obj.fact_list) {
		os << "  " << i << std::endl;
	}

	os << "Rules(" << obj.rule_list.size() << "):" << std::endl;
	for (auto i : obj.rule_list) {
		os << "  " << i << std::endl;
	}

	os << "Queries(" << obj.query_list.size() << "):" << std::endl;
	for (auto i : obj.query_list) {
		os << "  " << i << std::endl;
	}

	os << "Domain(" << obj.domain.size() << "):" << std::endl;
	for (auto str : obj.domain) {
		os << "  '" << str << "'" << std::endl;
	}

	return os;
}