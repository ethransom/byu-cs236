#include "domain_collector.h"

void DomainCollector::collect(const Literal& literal) {
	domain.insert(literal.str);
}

void DomainCollector::collect(Parameter* const& param) {
	param->collectDomain(domain);
}

void DomainCollector::collect(const Predicate& predicate) {
	for (const auto& param : predicate.param_list)
		collect(param);
}

void DomainCollector::collect(const Fact& fact) {
	for (const auto literal : fact.param_list)
		collect(literal);
}

void DomainCollector::collect(const Rule& rule) {
	for (const auto predicate : rule.predicate_list)
		collect(predicate);
}

void DomainCollector::collect(const DatalogProgram& program) {
	// schemes have identifiers, no strings

	for (auto fact : program.fact_list)
		collect(fact);

	for (auto rule : program.rule_list)
		collect(rule);

	for (auto query : program.query_list)
		collect(query);
}

std::ostream& operator<<(std::ostream& os, const DomainCollector& collector) {
	os << "Domain(" << collector.domain.size() << "):" << std::endl;
	for (auto str : collector.domain) {
		// os << "  '" << str << "'" << std::endl;
		os << "  " << str << std::endl;
	}
	return os;
}
