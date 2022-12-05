#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>

using namespace Syn;

struct MoveOperation {
	int beginStack;
	int endStack;
	int numToMove;
};

void DecodeStacks(std::vector<std::string> &stackBuffer, std::vector<std::vector<char>> &stacks) {
	int numStacks = (int)stackBuffer[stackBuffer.size() - 1].size() / 4 + 1;

	for (int i = 0; i < numStacks; i++) {
		stacks.emplace_back();
		int pos = i * 4 + 1;

		for (int j = (int)stackBuffer.size() - 2; j >= 0; j--) {
			if (pos >= stackBuffer[j].size()) break;
			char cargo = stackBuffer[j][pos];

			if (!isalpha(cargo)) break;

			stacks[i].push_back(cargo);
		}
	}
}

MoveOperation DecodeMove(const std::string& moveStr) {
	MoveOperation moveOperation{};
	std::vector<std::string> tokens;
	Parser::Tokenize(moveStr, tokens);
	moveOperation.numToMove = atoi(tokens[1].c_str());
	moveOperation.beginStack = atoi(tokens[3].c_str()) - 1;
	moveOperation.endStack = atoi(tokens[5].c_str()) - 1;
	return moveOperation;
}

void ApplyMove(MoveOperation move, std::vector<std::vector<char>> &stacks) {
	std::vector<char> crates;

	for (int i = 0; i < move.numToMove; i++) {
		crates.push_back(stacks[move.beginStack].back());
		stacks[move.beginStack].pop_back();
	}

	for (int i = 0; i < move.numToMove; i++) {
		stacks[move.endStack].push_back(crates.back());
		crates.pop_back();
	}
}

int main() {
	std::ifstream iFile("supply_stacks.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	std::vector<std::string> cargoBuffer;

	while (std::getline(iFile, buffer)) {
		size_t bufferLength = buffer.size();
		if (bufferLength > 0 && buffer[bufferLength - 1] == '\r')
			buffer.erase(bufferLength - 1);

		if (buffer.empty()) break;

		cargoBuffer.push_back(buffer);
	}

	std::vector<std::vector<char>> cargoStacks;
	DecodeStacks(cargoBuffer, cargoStacks);

	while (std::getline(iFile, buffer)) {
		size_t bufferLength = buffer.size();
		if (bufferLength > 0 && buffer[bufferLength - 1] == '\r')
			buffer.erase(bufferLength - 1);

		if (buffer.empty()) continue;

		ApplyMove(DecodeMove(buffer), cargoStacks);
	}

	std::string outputStr;
	for (auto &cargoStack : cargoStacks)
		outputStr += cargoStack.empty() ? ' ' : cargoStack.back();
	std::cout<<"Stack tops: "<<outputStr<<std::endl;

	iFile.close();
	return 0;
}
