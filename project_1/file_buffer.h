#pragma once

#include <string>

static const uint FILE_BUF_SIZE = 128;

class FileBuffer {
	FILE* file;
	char buf[FILE_BUF_SIZE];
	uint pos;
	uint remaining;

	// fill the buffer from the file
	// return: was there a refil
	bool refill();
public:
	FileBuffer(FILE* f);

	// does the stream have characters?
	bool eof();

	// read the string from the buffer, returns the success of the operation
	bool readString(std::string str);

	// true if char is read, false if not
	bool readChar(char c);

	// read characters until they are not in the whitelist
	// returns the number of chars read
	uint readWhitelist(std::string);
};
