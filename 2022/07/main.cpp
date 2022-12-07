#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>

using namespace Syn;

int main() {
	std::ifstream iFile("file.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	std::vector<std::string> cargoBuffer;
	Reader::ReadUntilEmptyLn(iFile, cargoBuffer);

	//

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) continue;

		//
	}

	//

	iFile.close();
	return 0;
}
