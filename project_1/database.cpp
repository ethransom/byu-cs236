#include "database.h"

#include <iostream>

void Database::createRelation(Predicate* scheme) {
	Relation relation(scheme);

	relations[scheme->identifier] = relation;
}

void Database::insert(Predicate* fact) {
	Relation& relation = relations.at(fact->identifier);

	// TODO: error check
}

void Database::print(std::ostream* output) {
	*output << "Hello, I am a databse :)" << std::endl;

	for (auto &pair : relations) {
		*output << pair.first << std::endl;
		pair.second.print(output);
	}
}

void Database::query(std::ostream* output, Predicate* query) {
	*output << "Al ur queris are belong to us" << std::endl;
}
