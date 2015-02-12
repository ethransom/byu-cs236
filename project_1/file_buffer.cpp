#include "file_buffer.h"

FileBuffer::FileBuffer(FILE* f) {
	file = f;

	refill();
}

bool FileBuffer::refill() {
	remaining = fread(buf, 1, sizeof(buf), file);
	pos = 0;
	return eof();
}

bool FileBuffer::eof() {
	return (pos + 1) > remaining;
}

bool FileBuffer::readChar(char c) {
	if (eof()) return false;

	return c == buf[pos++];
}