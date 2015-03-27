#pragma once

#include <vector>
#include <string>

enum ParamType {
	IDENTIFIER,
	LITERAL
};

struct CollectedParam {
	ParamType type;
	std::string str;
};

class ParamCollector {
public:
	void add(ParamType type, std::string& str);
	std::vector<CollectedParam> params;
};