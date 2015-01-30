#pragma once

#include <stdint.h>
#include <string>

class InputException : std::exception {};

class Seeker {
	std::string* input;

	uint commit_pos;
	uint seek_pos;

	uint commit_line;
	uint seek_line;
public:
	Seeker(std::string* input) : 
		input(input),
		commit_pos(0),
		seek_pos(0),
		commit_line(1),
		seek_line(1) {};

	// copy constructor
	// note that we share a reference to the input string
	// TODO: make this a std::shared_ptr?
	Seeker(Seeker& other) : 
		input(other.input),
		commit_pos(other.commit_pos),
		seek_pos(other.seek_pos) {};

	// attempt to read the string from input
	// returns true if successful
	// if unsuccessful, seek_pos is at place where discrepancy occured
	bool read_string(std::string value);

	// have we exhausted our input?
	bool end_of_file();

	// returns 1 char ahead
	char peek();

	// reads 1 character, advances seek_pos
	char read();

	// returns current char, without advancing seek_pos
	char check();

	std::string slice();

	uint divergence();

	// advances the commit_pos to the seek_pos, and returns the chars in between.
	std::string commit();

	// reset seek_pos to commit_pos
	void reset();

	// what line of the input are we on?
	uint get_current_line();
};
