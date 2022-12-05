#include <iostream>
#include <fstream>
#include <string>

int main() {
	std::ifstream iFile("file.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	while (std::getline(iFile, buffer)) {
		size_t bufferLength = buffer.size();
		if (bufferLength > 0 && buffer[bufferLength - 1] == '\r')
			buffer.erase(bufferLength - 1);

		if (buffer.empty()) continue;
	}

	iFile.close();
	return 0;
}
