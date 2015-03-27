#include "param_collector.h"

void ParamCollector::add(ParamType type, std::string& str) {
	params.emplace_back();
	auto param = params.back();

	param.type = type;
	param.str = str;
}
