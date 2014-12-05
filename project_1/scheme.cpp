#include "scheme.h"

void Scheme::interpolatedPrint(std::ostream* output, Tuple tuple) {
	for (int i = 0; i < attributes.size(); i++) {
		*output << attributes[i] << "=" << tuple.values[i] << " ";
	}
}