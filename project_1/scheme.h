#include <vector>
#include <string>

#include "datalogprogram.h"
#include "tuple.h"

class Scheme
{
	std::vector<std::string> attributes;
public:
	Scheme() {};
	Scheme(std::vector<Parameter> param_list);

	void interpolatedPrint(std::ostream* output, Tuple tuple);

	void removeAttribute(uint pos);

	void renameAttribute(uint pos, std::string newval);

	uint size();
};