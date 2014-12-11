#include "datalogprogram.h"
#include "database.h"

void Predicate::toString(std::ostringstream& os) const {
	os << identifier << "(";

	for (uint i = 0; i < param_list.size(); i++) {
		const auto& param = param_list[i];
		param.toString(os);

		if ((i + 1) != param_list.size()) {
			// last iteration
			os << ",";
		}
	}

	os << ")";
}

std::string Predicate::toString() const {
	std::ostringstream os;

	toString(os);

	return os.str();
}

void Predicate::determineDomain(std::set<std::string>* domain) const {
	for (const auto& param : param_list) {
		param.determineDomain(domain);
	}
}

void Rule::toString(std::ostringstream& os) const {
	predicate.toString(os);
	os << " :- ";

	for (uint i = 0; i < predicate_list.size(); i++) {
		const auto& predicate = predicate_list[i];

		predicate.toString(os);

		if ((i + 1) != predicate_list.size()) {
			// last iteration
			os << ",";
		}
	}
}

void Rule::determineDomain(std::set<std::string>* domain) const {
	for (const auto& p : predicate_list) {
		p.determineDomain(domain);
	}
}

void Parameter::toString(std::ostringstream& os) const {
	if (type == STRING) {
		os << "'" << str << "'";
	} else if (type == ID) {
		os << str;
	}
}

void Parameter::determineDomain(std::set<std::string>* domain) const {
	if (type == STRING) {
		domain->insert(str);
	}
}

void DatalogProgram::determineDomain() {
	for (const auto& s : scheme_list)
		s.determineDomain(&domain);

	for (const auto& f : fact_list)
		f.determineDomain(&domain);

	for (const auto& r : rule_list)
		r.determineDomain(&domain);

	for (const auto& q : query_list)
		q.determineDomain(&domain);
}

std::string DatalogProgram::toString() const {
	std::ostringstream os;

	os << "Schemes(" << scheme_list.size() << "):" << std::endl;
	for (auto s : scheme_list) {
		os << "  ";
		s.toString(os);
		os << std::endl;
	}

	os << "Facts(" << fact_list.size() << "):" << std::endl;
	for (auto f : fact_list) {
		os << "  ";
		f.toString(os);
		os << std::endl;
	}

	os << "Rules(" << rule_list.size() << "):" << std::endl;
	for (auto r : rule_list) {
		os << "  ";
		r.toString(os);
		os << std::endl;
	}

	os << "Queries(" << query_list.size() << "):" << std::endl;
	for (auto q : query_list) {
		os << "  ";
		q.toString(os);
		os << std::endl;
	}

	os << "Domain(" << domain.size() << "):" << std::endl;
	for (auto str : domain) {
		os << "  '" << str << "'" << std::endl;
	}

	return os.str();
}

void DatalogProgram::evaluate(std::ostream* output) {
	Database database;

	*output << "Scheme Evaluation" << std::endl << std::endl;

	for (auto scheme : scheme_list) {
		// add an empty relation
		database.createRelation(&scheme);
	}

	// ===============================================

	for (auto fact : fact_list) {
		// add a tuple to a relation
		database.insert(&fact);
	}

	*output << "Fact Evaluation" << std::endl << std::endl;

	database.print(output);

	*output << std::endl;

	// ===============================================

	*output << "Query Evaluation" << std::endl << std::endl;

	for (auto query : query_list) {
		// use operations to eval query
		// output as we go
		database.query(output, &query);

		*output << std::endl;
	}
}
