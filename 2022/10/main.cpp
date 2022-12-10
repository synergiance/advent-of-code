#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>

using namespace Syn;

enum Instruction {
	noop, addx
};

int samplePoints[] = {
		20, 60, 100, 140, 180, 220
};

int main() {
	std::ifstream iFile("cpu_instructions.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	int x = 1; // The CPU's only register
	int cycle = 0;

	int signalTotal = 0;
	int sampleIdx = 0;

	while (sampleIdx < 6) {
		Instruction inst = noop;
		int param = 0;
		int instTime = 1;

		if (Reader::getline(iFile, buffer)) {
			std::vector<std::string> tokens;
			Parser::Tokenize(buffer, tokens);

			if (tokens.size() >= 2 && tokens[0] == "addx") {
				inst = addx;
				param = atoi(tokens[1].c_str());
				instTime = 2;
			}
		}

		cycle += instTime;

		if (cycle >= samplePoints[sampleIdx])
			signalTotal += x * samplePoints[sampleIdx++];

		if (inst == addx)
			x += param;
	}

	std::cout<<"Signal total: "<<signalTotal<<std::endl;

	iFile.close();
	return 0;
}
