#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>

using namespace Syn;

enum Func {
	noop, addx
};

struct Instruction {
	Func func;
	int time;
	int param;
};

const int screenWidth = 40;
const int screenHeight = 6;
const int screenArraySz = screenHeight * screenWidth;

Instruction DecodeInstruction(const std::string &instStr) {
	Instruction inst{noop, 1, 0};
	std::vector<std::string> tokens;
	Parser::Tokenize(instStr, tokens);

	if (tokens.size() < 2 || tokens[0] == "noop")
		return inst;

	inst.func = addx;
	inst.param = atoi(tokens[1].c_str());
	inst.time = 2;

	return inst;
}

void ApplyInstruction(const Instruction &inst, int &x) {
	if (inst.func == noop) return;
	x += inst.param;
}

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

	int x = 1; // The CPU's only register

	char screenOutput[screenArraySz];
	Instruction inst = {noop, 1, 0};
	int instBegin = -1;

	for (int cycle = 0; cycle < screenArraySz; cycle++) {
		if (instBegin + inst.time - cycle == 0) {
			ApplyInstruction(inst, x);

			if (Reader::getline(iFile, buffer)) {
				inst = DecodeInstruction(buffer);
			} else {
				inst = {noop, 1, 0};
			}

			instBegin = cycle;
		}

		screenOutput[cycle] = CheckSprite(cycle % screenWidth, x) ? '#' : '.';
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
