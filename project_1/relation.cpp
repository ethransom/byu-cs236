#include "relation.h"

#include <algorithm>

Relation::Relation(std::vector<Identifier>& values) {
	dirty_flag = false;

	for (auto val : values)
		schema.push_back(val.str);
}

void Relation::flatten(std::ostream& os) const {
	for (auto scheme : schema)
		os << scheme << ",";

	os << std::endl;

	for (auto row : rows) {
		for (auto val : row)
			os << val << ",";

		os << std::endl;
	}
}

void Relation::print(std::ostream& os) const {
	for (auto row : rows) {
		if (row.size() == 0)
			break;

		os << "  ";

		for (uint j = 0; j < row.size(); j++) {
			if (j != 0)
				os << ", ";

			os << schema[j] << "=" << row[j];
		}

		os << std::endl;
	}
}

uint Relation::size() {
	return rows.size();
}

bool Relation::is_dirty() {
	return dirty_flag;
}

void Relation::insert(std::vector<Literal>& values) {
	std::vector<std::string> vec;
	for (auto value : values)
		vec.push_back(value.str);
	rows.insert(vec);
}

void Relation::select(std::string* str, uint pos) {
	// return;
	for (auto i = rows.begin(); i != rows.end();) {
		if ((*i)[pos] != *str)
			rows.erase(i);
		i++;
	}
}

void Relation::select(uint one, uint two) {
	for (auto i = rows.begin(); i != rows.end();) {
		if ((*i)[one] != (*i)[two]) {
			rows.erase(i);
		}
		i++;
	}
}

void Relation::project(uint pos) {
	dirty_flag = true;

	schema.erase(schema.begin() + pos);

	std::set<std::vector<std::string>> fixed;
	for (auto row : rows) {
		std::vector<std::string> vec = row;
		vec.erase(vec.begin() + pos);
		fixed.insert(vec);
	}

	rows = fixed;
}

void Relation::rename(std::string* str, uint pos) {
	dirty_flag = true;

	schema[pos] = *str;
}