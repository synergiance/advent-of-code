#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>

using namespace Syn;

int main() {
	std::ifstream iFile("input.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	// TODO: Set up

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) continue;

		// TODO: Process
	}

	// TODO: Output

	iFile.close();
	return 0;
}
