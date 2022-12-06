#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>

using namespace Syn;

int main() {
	std::ifstream iFile("supply_stacks.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	// TODO: Pre parsing

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) continue;

		// TODO: While Parsing
	}

	// TODO: Post parsing

	iFile.close();
	return 0;
}