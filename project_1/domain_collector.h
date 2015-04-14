#pragma once

#include <set>
#include <ostream>

#include "datalogprogram.h"

class DomainCollector {
	std::set<std::string> domain;

	void collect(const Literal&);

	void collect(Parameter* const&);

	void collect(const Predicate&);

	void collect(const Fact&);

	void collect(const Rule&);

public:
	void collect(const DatalogProgram&);

	friend std::ostream& operator<<(std::ostream& os,
		const DomainCollector& dc);
};
