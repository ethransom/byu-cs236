#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <regex>

class Filestream {
	std::ifstream input;

public:
	Filestream(std::string filename);
	~Filestream();

	// tries to read the 'input' string
	// returns string if successful
	// returns NULL if not
	std::string try_match_string(std::string input);
};
