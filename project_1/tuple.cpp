#include "tuple.h"

Tuple::Tuple(Predicate* predicate) {
	for (auto param : predicate->param_list) {
		// TODO: ensure param is string?
		values.push_back(param.str);
	}
}

bool Tuple::operator<(const Tuple &t) const {
	return values < t.values;
}
