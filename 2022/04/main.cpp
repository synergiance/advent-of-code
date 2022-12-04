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

	// Code

	iFile.close();
	return 0;
}
