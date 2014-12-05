#include "datalogprogram.h"
#include "database.h"

std::string Predicate::toString() {
	std::ostringstream os;
	os << identifier << "(";

	auto last = param_list.back();
	for (auto param : param_list) {
		os << param.toString();

		if (param != last) {
			// last iteration
			os << ",";
		}
	}

	os << ")";

	return os.str();
}

void Predicate::determineDomain(std::set<std::string>* domain) {
	for (auto param : param_list) {
		param.determineDomain(domain);
	}
}

std::string Rule::toString() {
	std::ostringstream os;
	os << predicate.toString() << " :- ";

	auto last = predicate_list.back();
	for (auto predicate : predicate_list) {
		os << predicate.toString();

		if (last != predicate) {
			// last iteration
			os << ",";
		}
	}

	return os.str();
}

void Rule::determineDomain(std::set<std::string>* domain) {
	for (auto p : predicate_list) {
		p.determineDomain(domain);
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

void DatalogProgram::determineDomain() {
	for (auto s : scheme_list)
		s.determineDomain(&domain);

	for (auto f : fact_list)
		f.determineDomain(&domain);

	for (auto r : rule_list)
		r.determineDomain(&domain);

	for (auto q : query_list)
		q.determineDomain(&domain);
}

std::string DatalogProgram::toString() {
	std::ostringstream os;

	os << "Schemes(" << scheme_list.size() << "):" << std::endl;
	for (auto s : scheme_list) {
		os << "  " << s.toString() << std::endl;
	}

	os << "Facts(" << fact_list.size() << "):" << std::endl;
	for (auto f : fact_list) {
		os << "  " << f.toString() << std::endl;
	}

	os << "Rules(" << rule_list.size() << "):" << std::endl;
	for (auto r : rule_list) {
		os << "  " << r.toString() << std::endl;
	}

	os << "Queries(" << query_list.size() << "):" << std::endl;
	for (auto q : query_list) {
		os << "  " << q.toString() << std::endl;
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
		database.createRelation(scheme);
	}

	// ===============================================

	for (auto fact : fact_list) {
		// add a tuple to a relation
		database.insert(fact);
	}

	*output << "Fact Evaluation" << std::endl << std::endl;

	database.print(output);

	// ===============================================

	for (auto query : query_list) {
		// use operations to eval query
		// output as we go
		database.query(output, query);
	}
}
