#pragma once

#include <vector>
#include <string>

#include "datalogprogram.h"

class Tuple
{
public:
	std::vector<std::string> values;

	Tuple();
	Tuple(Predicate* predicate);

	bool operator<(const Tuple &t) const;
};