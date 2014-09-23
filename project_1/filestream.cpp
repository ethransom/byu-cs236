#include "filestream.h"

Filestream::Filestream(std::string filename) {
	input.open("text.txt");
}

Filestream::~Filestream() {
	input.close();
}

Filestream::try_match_string(std::string expected) {
  regex r("trololo");

  if (regex_match(input, r)) {
    std::cout << "CLOSING" << std::endl;
  }
}
