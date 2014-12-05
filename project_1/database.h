#include <map>
#include <string>

#include "datalogprogram.h"
#include "relation.h"

class Database
{
	std::map<std::string, Relation> relations;
public:
	// create a relation given a scheme
	void createRelation(Predicate* scheme);

	// insert the given fact into its corresponding relation
	void insert(Predicate* fact);

	// output the current state of the database
	void print(std::ostream* output);

	// evaluate the query and print status to the given ostream
	// TODO: not void
	void query(std::ostream* output, Predicate* query);

};