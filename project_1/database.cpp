#include "database.h"

#include <iostream>

void Database::createRelation(Predicate* scheme) {
	Relation relation(scheme);

	relations[scheme->identifier] = relation;
}

void Database::insert(Predicate* fact) {
	Relation& relation = relations.at(fact->identifier);

	// TODO: error check
	relation.insert(fact);
}

void Database::print(std::ostream* output) {
	for (auto &pair : relations) {
		*output << pair.first << std::endl;
		pair.second.print(output);
	}
}

void Database::query(std::ostream* output, Predicate* query) {
	auto pos = relations.find(query->identifier);
	if (pos == relations.end()) {
		*output << "ERROR: no such relation " << query->identifier << std::endl;
		*output << "  " << query->toString() << std::endl;

		return;
	}

	Relation& relation = pos->second;

	// evaluate the query, storing intermediate steps for later output
	auto select = relation.select(query->param_list);

	auto project = select.project(query->param_list);

	auto result = project.rename(query->param_list);

	bool success = result.size() > 0;

	// we've evaluated, now print
	*output << query->toString() << "?";
	if (success) {
		*output << " Yes(" << result.size() << ")" << std::endl;

		*output << "select" << std::endl;
		select.print(output);

		*output << "project" << std::endl;
		project.print(output);

		*output << "rename" << std::endl;
		result.print(output);
	} else {
		*output << " No" << std::endl;
	}
}
