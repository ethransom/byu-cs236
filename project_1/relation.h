#include <string>
#include <set>
#include <vector>

#include "scheme.h"
#include "tuple.h"
#include "datalogprogram.h"

class Relation
{
	// std::string name;
	Scheme scheme;
	std::set<Tuple> tuples;

	// remove tuples that don't have str at pos
	void filterLiteral(uint pos, std::string str);

	// remove tuples that don't have matching values at pos1 and pos2
	void filterRepeat(uint pos1, uint pos2);

	void deleteColumn(uint pos);

	void renameColumn(uint pos, std::string newval);

public:
	Relation() {};
	Relation(Predicate* s) : scheme(s->param_list) {};

	void insert(Predicate* fact);

	void insert(Tuple tuple);

	Relation select(std::vector<Parameter> params);

	// throw out columns that aren't an ID in params
	Relation project(std::vector<Parameter> params);

	Relation rename(std::vector<Parameter> params);

	void print(std::ostream* output);

	uint size();
};