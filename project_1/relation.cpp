#include "relation.h"

bool Relation::select() {
	// code...
	return true;
}

bool Relation::project() {
	// code...
	return true;
}

bool Relation::rename() {
	// code...
	return true;
}

void Relation::print(std::ostream* output) {
	*output << name << std::endl;

	for (auto tuple : tuples) {
		scheme.interpolatedPrint(output, tuple);
		*output << std::endl;
	}
}