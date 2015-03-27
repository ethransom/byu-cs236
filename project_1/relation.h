#pragma once

#include <set>
#include <vector>
#include <string>

#include "datalogprogram.h"

class Relation {
	std::vector<std::string> schema;

	std::set<std::vector<std::string>> rows;

	bool dirty_flag;
public:
	Relation(std::vector<Identifier>&);

	// debug print, in pseudo-table form
	void flatten(std::ostream& os) const;

	// clean, indented output print
	void print(std::ostream& os) const;

	// how many tuples are contained in the relation?
	uint size();

	// has the relation been projected or renamed?
	bool is_dirty();

	void insert(std::vector<Literal>&);

	void select(std::string* str, uint pos);

	// filter rows where the positions do not match
	void select(uint one, uint two);

	void project(uint pos);

	void rename(std::string* str, uint pos);
};
