#pragma once

#include <map>

#include "relation.h"

class Database {
	std::map<std::string, Relation*> relations;

public:
	// Database();
	// ~Database();

	void flatten(std::ostream& os) const;

	void create(Scheme&);

	void insert(Fact&);

	Relation query(Query&);
};