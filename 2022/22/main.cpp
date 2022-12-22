#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>
#include <Grid.h>

using namespace Syn;

const char gWallChar = '#';
const char gWalkableChar = '.';
const char gNullChar = ' ';

const char gRight = 'R';
const char gLeft = 'L';

const int gRowMultiplier = 1000;
const int gColumnMultiplier = 4;

const Coordinate gDirections[] {
		{1, 0},  // Right
		{0, 1}, // Down
		{-1, 0}, // Left
		{0, -1}   // Up
};

const std::string gDirectionStr[] {
	"Right",
	"Down",
	"Left",
	"Up"
};

enum InstructionType {
	move, turn
};

enum TurnDirection {
	left, right
};

struct MapInstruction {
	InstructionType type;
	TurnDirection direction;
	int moveDistance;
};

int main() {
	std::ifstream iFile("monkey_map.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	std::vector<std::string> lines;
	int longestLine = 0;

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) break;

		if (buffer.size() > longestLine) longestLine = (int)buffer.size();
		lines.push_back(buffer);
	}

	std::cout<<"Loaded map of size: "<<longestLine<<"x"<<lines.size()<<std::endl;

	std::string instructionStr;
	Reader::getline(iFile, instructionStr);
	iFile.close();

	std::cout<<"Loaded instruction string of length: "<<instructionStr.size()<<std::endl;

	Grid<char> map(longestLine, (int)lines.size());
	map.Fill(gNullChar);

	for (int y = 0; y < lines.size(); y++)
		for (int x = 0; x < lines[y].size(); x++)
			map[{x, y}] = lines[y][x];

	std::cout<<"Map grid constructed!"<<std::endl;

	std::vector<MapInstruction> instructions;

	int iterator = 0;
	while (iterator < instructionStr.size()) {
		MapInstruction instruction{};

		if (std::isdigit(instructionStr[iterator])) {
			instruction.type = move;
			size_t numLen = Parser::GetNumberLength(instructionStr, iterator);
			instruction.moveDistance = Parser::GetNumFromStr(instructionStr.substr(iterator, numLen).c_str());
			iterator += (int)numLen;
		} else {
			instruction.type = turn;
			instruction.direction = instructionStr[iterator++] == gRight ? right : left;
		}

		instructions.push_back(instruction);
	}

	std::cout<<"Parsed "<<instructions.size()<<" instructions."<<std::endl;

	Coordinate currentPosition = {0, 0};
	for (int i = 0; i < map.GetWidth(); i++) {
		if (map[{i, 0}] != gWalkableChar) continue;
		currentPosition.x = i;
		break;
	}

	std::cout<<"Starting coordinate: ("<<currentPosition.x+1<<", 1)"<<std::endl;

	int currentDirection = 0;
	for (MapInstruction &instruction : instructions) {
		if (instruction.type == turn) {
			currentDirection += instruction.direction == right ? 1 : 3;
			currentDirection %= 4;
			continue;
		}

		Coordinate directionOffset = gDirections[currentDirection];
		Coordinate potentialPosition = currentPosition;

		for (int i = 0; i < instruction.moveDistance; i++) {
			do {
				potentialPosition += directionOffset;
				potentialPosition.x %= map.GetWidth();
				potentialPosition.y %= map.GetHeight();
				if (potentialPosition.x < 0) potentialPosition.x += map.GetWidth();
				if (potentialPosition.y < 0) potentialPosition.y += map.GetHeight();
			} while (map[potentialPosition] == gNullChar);

			if (map[potentialPosition] == gWallChar) break;
			currentPosition = potentialPosition;
		}
	}

	std::cout<<"Ending coordinate: ("<<currentPosition.x+1<<", "<<currentPosition.y+1<<")"<<std::endl;
	std::cout<<"Ending rotation: "<<gDirectionStr[currentDirection]<<std::endl;

	int password = (currentPosition.x+1) * gColumnMultiplier + (currentPosition.y+1) * gRowMultiplier + currentDirection;
	std::cout<<"Password: "<<password<<std::endl;

	return 0;
}
