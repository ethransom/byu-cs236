#include "scheme.h"

Scheme::Scheme(std::vector<Parameter> param_list) {
	for (auto param : param_list) {
		attributes.push_back(param.str);
	}
}

void Scheme::interpolatedPrint(std::ostream* output, Tuple tuple) {
	for (uint i = 0; i < attributes.size(); i++) {
		*output << attributes[i] << "='" << tuple.values[i] << "'";

		if ((i + 1) != attributes.size()) *output << " ";
	}
}

void Scheme::removeAttribute(uint pos) {
	attributes.erase(attributes.begin() + pos);
}

void Scheme::renameAttribute(uint pos, std::string newval) {
	attributes[pos] = newval;
}

uint Scheme::size() {
	return attributes.size();
}
