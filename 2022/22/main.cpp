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

const int gCubeSize = 50;

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

struct CubeEdge {
	int connectedFace;
	int rotationOffset;
};

struct CubeFaceData {
	CubeEdge edges[4];
};

/**
 * .12
 * .4.
 * 67.
 * 9..
 */

const CubeFaceData gCubeFaces[] {
	{},
	{
		{
			{2, 0},
			{4, 0},
			{6, 2},
			{9, 1}
		}
	},
	{
		{
			{7, 2},
			{4, 1},
			{1, 0},
			{9, 0}
		}
	},
	{},
	{
		{
			{2, 3},
			{7, 0},
			{6, 3},
			{1, 0}
		}
	},
	{},
	{
		{
			{7, 0},
			{9, 0},
			{1, 2},
			{4, 1}
		}
	},
	{
		{
			{2, 2},
			{9, 1},
			{6, 0},
			{4, 0}
		}
	},
	{},
	{
		{
			{7, 3},
			{2, 0},
			{1, 3},
			{6, 0}
		}
	},
	{},
	{}
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

int GetCubeFaceIdx(Coordinate &coordinate) {
	return coordinate.x / gCubeSize + (coordinate.y / gCubeSize) * 3;
}

Coordinate GetCubeOrigin(int cubeIdx) {
	if (cubeIdx < 0 || cubeIdx >= 12)
		std::cerr<<"Index out of bounds! "<<cubeIdx<<std::endl;
	return {
		(cubeIdx % 3) * gCubeSize,
		(cubeIdx / 3) * gCubeSize
	};
}

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
		int potentialDirection = currentDirection;

		for (int i = 0; i < instruction.moveDistance; i++) {
			potentialPosition += directionOffset;
			potentialPosition.x %= map.GetWidth();
			potentialPosition.y %= map.GetHeight();
			if (potentialPosition.x < 0) potentialPosition.x += map.GetWidth();
			if (potentialPosition.y < 0) potentialPosition.y += map.GetHeight();

			if (map[potentialPosition] == gNullChar) {
				int cubeIdx = GetCubeFaceIdx(currentPosition);
				int nextCubeIdx = gCubeFaces[cubeIdx].edges[currentDirection].connectedFace;
				int rotationOffset = gCubeFaces[cubeIdx].edges[currentDirection].rotationOffset;
				int posOnEdge;

				switch (currentDirection) {
					case 0:
						posOnEdge = currentPosition.y % gCubeSize;
						break;
					case 1:
						posOnEdge = gCubeSize - (currentPosition.x % gCubeSize) - 1;
						break;
					case 2:
						posOnEdge = gCubeSize - (currentPosition.y % gCubeSize) - 1;
						break;
					case 3:
						posOnEdge = currentPosition.x % gCubeSize;
						break;
					default:
						posOnEdge = 0;
						break;
				}

				potentialDirection += rotationOffset;
				potentialDirection %= 4;

				Coordinate cubeOffset{0, 0};
				switch (potentialDirection) {
					case 0:
						cubeOffset.y = posOnEdge;
						cubeOffset.x = 0;
						break;
					case 1:
						cubeOffset.x = gCubeSize - posOnEdge - 1;
						cubeOffset.y = 0;
						break;
					case 2:
						cubeOffset.y = gCubeSize - posOnEdge - 1;
						cubeOffset.x = gCubeSize - 1;
						break;
					case 3:
						cubeOffset.x = posOnEdge;
						cubeOffset.y = gCubeSize - 1;
						break;
					default:
						cubeOffset.x = cubeOffset.y = 0;
						break;
				}

				potentialPosition = GetCubeOrigin(nextCubeIdx) + cubeOffset;
				std::cout<<"Potentially passing from cube "<<cubeIdx<<" to cube "<<nextCubeIdx<<std::endl;
				std::cout<<"with old coordinate ("<<currentPosition.x+1<<", "<<currentPosition.y+1;
				std::cout<<") and new coordinate ("<<potentialPosition.x+1<<", "<<potentialPosition.y+1<<")"<<std::endl;
			}

			if (map[potentialPosition] == gWallChar) break;
			currentPosition = potentialPosition;
			currentDirection = potentialDirection;
			directionOffset = gDirections[currentDirection];
		}
	}

	std::cout<<"Ending coordinate: ("<<currentPosition.x+1<<", "<<currentPosition.y+1<<")"<<std::endl;
	std::cout<<"Ending rotation: "<<gDirectionStr[currentDirection]<<std::endl;

	int password = (currentPosition.x+1) * gColumnMultiplier + (currentPosition.y+1) * gRowMultiplier + currentDirection;
	std::cout<<"Password: "<<password<<std::endl;

	return 0;
}
