#include "seeker.h"

// bool Seeker::read_string(std::string value) {
// 	// CRIT: remove inefficiency
// 	std::string token = input->substr(seek_pos, value.length());

// 	if (token == value) { // input must start with desired string
// 		seek_pos += value.length();
// 		return true;
// 	} else {
// 		return false;
// 	}
// }

bool Seeker::read_string(std::string value) {
	for (auto c : value) {
		if (read() != c) return false;
	}

	return true;
}

bool Seeker::end_of_file() {
	return (seek_pos) >= input->length();
}

char Seeker::peek() {
	if (end_of_file())
		throw InputException();

	return input->at(seek_pos + 1);
}

char Seeker::read() {
	char c = check();

	seek_pos++;
	if (c == '\n') seek_line++;

	return c;
}

char Seeker::check() {
	if (end_of_file())
		throw InputException();

	return input->at(seek_pos);
}

std::string Seeker::slice() {
	return input->substr(commit_pos, (seek_pos - commit_pos));
}

uint Seeker::divergence() {
	return (seek_pos - commit_pos);
}

std::string Seeker::commit() {
	// TODO: string slices?
	std::string output = slice();

	commit_pos = seek_pos;
	commit_line = seek_line;

	return output;
}

void Seeker::reset() {
	seek_pos = commit_pos;
	seek_line = commit_line;
}

uint Seeker::get_current_line() {
	// return starting line of token
	return commit_line;
}