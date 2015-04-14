#include "relation.h"

#include <algorithm>
#include <stdexcept>

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

bool Relation::has_column(std::string& column) {
	auto pos = std::find(schema.begin(), schema.end(), column);
	return (pos != schema.end());
}

void Relation::insert(std::vector<Literal>& values) {
	std::vector<std::string> vec;
	for (auto value : values)
		vec.push_back(value.str);
	rows.insert(vec);
}

void Relation::select(std::string* str, uint pos) {
	std::set<std::vector<std::string>> new_rows;

	for (auto i = rows.begin(); i != rows.end(); i++) {
		if ((*i)[pos] == *str)
			new_rows.insert(std::move(*i));
	}

	rows = std::move(new_rows);
}

void Relation::select(uint one, uint two) {
	std::set<std::vector<std::string>> new_rows;

	for (auto i = rows.begin(); i != rows.end(); i++) {
		if ((*i)[one] == (*i)[two]) {
			new_rows.insert(std::move(*i));
		}
	}

	rows = std::move(new_rows);
}

void Relation::project(uint pos) {
	dirty_flag = true;

	schema.erase(schema.begin() + pos);

	std::set<std::vector<std::string>> fixed;
	for (auto row : rows) {
		std::vector<std::string> vec = std::move(row);
		vec.erase(vec.begin() + pos);
		fixed.insert(std::move(vec));
	}

	rows = std::move(fixed);
}

void Relation::rename(std::string* str, uint pos) {
	dirty_flag = true;

	schema[pos] = *str;
}

void Relation::join(Relation& other) {
	std::set<std::vector<std::string>> new_set;
	std::vector<std::string> new_schema = std::move(schema);

	for (auto& column : other.schema)
		new_schema.push_back(std::move(column));

	for (auto& tuple : rows) {
		for (auto& other_tuple : other.rows) {
			std::vector<std::string> new_tuple;
			new_tuple.reserve(tuple.size() + other_tuple.size());

			new_tuple.insert(new_tuple.end(),
				tuple.begin(), tuple.end());
			new_tuple.insert(new_tuple.end(),
				other_tuple.begin(), other_tuple.end());

			new_set.insert(std::move(new_tuple));
		}
	}

	schema = std::move(new_schema);
	rows = std::move(new_set);

	// filter duplicates
	// A B C D A D C F
	for (uint i = 0; i < schema.size(); i++) {
		// have we encountered this before?
		for (int j = i - 1; j >= 0; j--) {
			if (schema[i] == schema[j]) {
				select(j, i);
				project(i);
				i--;
			}
		}
	}
}

bool Relation::merge(Relation& other, std::vector<Identifier> pattern) {
	std::set<std::vector<std::string>> additions;

	for (auto& tuple : other.rows) {
		std::vector<std::string> new_tuple;

		for (auto& identifier : pattern) {
			std::string& column = identifier.str;

			bool found = false;
			// find this column in the relation
			for (uint j = 0; j < other.schema.size(); j++) {
				if (other.schema[j] == column) {
					new_tuple.push_back(std::move(tuple[j]));
					found = true;
					break; // safe to break, assuming no duplicates?
				}
			}

			if (!found)
				throw std::runtime_error("could not find column in result of join!");
		}

		additions.insert(std::move(new_tuple));
	}

	uint before = rows.size();

	for (auto& tuple : additions) {
		rows.insert(std::move(tuple));
	}

	return before - rows.size() > 0;
}

