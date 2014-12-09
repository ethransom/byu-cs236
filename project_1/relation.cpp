#include "relation.h"

#include <algorithm>

void Relation::filterLiteral(uint pos, std::string str) {
	for (auto tuple : tuples) {
		if (tuple.values[pos] != str) {
			tuples.erase(tuple);
		}
	}
}

void Relation::filterRepeat(uint pos1, uint pos2) {
	for (auto tuple : tuples) {
		if (tuple.values[pos1] != tuple.values[pos2]) {
			tuples.erase(tuple);
		}
	}
}

void Relation::deleteColumn(uint pos) {
	scheme.removeAttribute(pos);

	for (auto tuple : tuples) {
		tuple.values.erase(tuple.values.begin() + pos);
	}
}

void Relation::renameColumn(uint pos, std::string newval) {
	scheme.renameAttribute(pos, newval);
}

// PUBLIC

void Relation::insert(Predicate* fact) {
	Tuple t(fact);

	tuples.insert(t);
}

Relation Relation::select(std::vector<Parameter> params) {
	// copy ourselves
	Relation result = *this;

	for (uint i = 0; i < params.size(); i++) {
		auto param = params[i];

		if (param.type == STRING) {
			result.filterLiteral(i, param.str);
		} else if (param.type == ID) {
			// find if there is another occurence of this identifier
			for (uint j = (i + 1); j < params.size(); j++) {
				if (params[j].type == ID && params[j].str == param.str) {
					result.filterRepeat(i, j);
					break;
					// you'd think we'd get any additional duplicates in the
					// space here, but we don't
					// this may be unexpected to you, dear reader
					// the element that j is pointing to will eventually be
					// pointed to by i, and the process will begin again.
				}
			}
		}
	}

	return result;
}

Relation Relation::project(std::vector<Parameter> params) {
	Relation result = *this;
	std::vector<std::string*> dups;

	for (uint i = 0; i < params.size(); i++) {
		auto param = params[i];
		if (std::find(dups.begin(), dups.end(), &param.str) != dups.end()) {
			result.deleteColumn(i);
		} else if (param.type != ID) {
			result.deleteColumn(i);
		} else {
			dups.push_back(&param.str);
		}
	}

	return result;
}

Relation Relation::rename(std::vector<Parameter> params) {
	Relation result = *this;

	uint id_count = 0;
	for (uint i = 0; i < params.size(); i++) {
		if (params[i].type == ID) {
			result.renameColumn(i, params[i].str);
			id_count++;
		}
	}

	return result;
}

void Relation::print(std::ostream* output) {
	if (scheme.size() == 0) return;

	for (auto tuple : tuples) {
		*output << "  ";
		scheme.interpolatedPrint(output, tuple);
		*output << std::endl;
	}
}

uint Relation::size() {
	return tuples.size();
}