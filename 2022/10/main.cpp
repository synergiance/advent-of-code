#include <iostream>
#include <fstream>
#include <string>
#include <Reader.h>
#include <ElfProcessor.h>

using namespace Syn;

const int screenWidth = 40;
const int screenHeight = 6;
const int screenArraySz = screenHeight * screenWidth;

bool CheckSprite (int screenPos, int spritePos) {
	int relativePos = screenPos - spritePos;
	return relativePos >= -1 && relativePos <= 1;
}

int main() {
	std::ifstream iFile("cpu_instructions.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	Elf::Processor communicatorProcessor;

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) continue;

		communicatorProcessor.AddInstruction(Elf::Processor::DecodeInstruction(buffer));
	}

	char screenOutput[screenArraySz];

	for (int cycle = 0; cycle < screenArraySz; cycle++) {
		int spritePos = communicatorProcessor.GetRegisterX();
		screenOutput[cycle] = CheckSprite(cycle % screenWidth, spritePos) ? '#' : '.';
		communicatorProcessor.Cycle();
	}

	std::string str;
	for (int ln = 0; ln < screenHeight; ln++) {
		str.clear();
		for (int col = 0; col < screenWidth; col++)
			str += screenOutput[ln * screenWidth + col];
		std::cout<<"Line "<<ln<<": "<<str<<std::endl;
	}

	iFile.close();
	return 0;
}
