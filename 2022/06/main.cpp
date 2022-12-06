#include <iostream>
#include <fstream>
#include <string>
#include <vector>

static const int headerSz = 14;

int main() {
	std::ifstream iFile("communication.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;

	char c;
	char header[headerSz];

	for (int i = 0; i < headerSz && iFile.get(c); i++)
		header[i] = c;

	size_t charsRead = headerSz;

	while (iFile.get(c)) {
		header[charsRead % headerSz] = c;
		bool duplicate = false;

		for (int i = 1; i < headerSz; i++)
			for (int j = 0; j < i; j++)
				duplicate |= header[i] == header[j];

		charsRead++;
		if (!duplicate) break;
	}

	std::string code;
	for (int i = 0; i < headerSz; i++)
		code.push_back(header[(charsRead + i) % headerSz]);

	std::cout<<"Start of message: "<<code<<std::endl;
	std::cout<<"Number of bytes read: "<<charsRead<<std::endl;

	iFile.close();
	return 0;
}