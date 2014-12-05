#include <string>
#include <set>

#include "scheme.h"
#include "tuple.h"
#include "datalogprogram.h"

class Relation
{
	// std::string name;
	Scheme scheme;
	std::set<Tuple> tuples;

public:
	Relation() {};
	Relation(Predicate* s) : /*name(s->identifier),*/ scheme(s->param_list) {};

	bool select();
	bool project();
	bool rename();

	void print(std::ostream* output);

};