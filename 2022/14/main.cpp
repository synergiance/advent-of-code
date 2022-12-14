#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>
#include <Coordinate.h>
#include <Grid.h>

using namespace Syn;

#define DIMENSIONS 1000

const char gRock = '#';
const char gSand = 'o';
const char gAir = ' ';
const char gSandRift = '+';
const char gFlowingSand = '~';

const Coordinate gSandRiftPos = {500,0};

const Coordinate gSandFlow[] = {
		{0, 1},
		{-1, 1},
		{1, 1}
};

void ImportRockPath(const std::string &input, Grid<char> &map, Coordinate &topLeft, Coordinate &bottomRight) {
	std::vector<Coordinate> path;
	std::vector<std::string> tokens;
	Parser::Tokenize(input, tokens);

	for (int i = 0; i < tokens.size(); i += 2) {
		Coordinate newCoordinate = Coordinate::Parse(tokens[i].c_str());
		path.push_back(newCoordinate);

		if (newCoordinate.x < topLeft.x) topLeft.x = newCoordinate.x;
		if (newCoordinate.y < topLeft.y) topLeft.y = newCoordinate.y;
		if (newCoordinate.x > bottomRight.x) bottomRight.x = newCoordinate.x;
		if (newCoordinate.y > bottomRight.y) bottomRight.y = newCoordinate.y;
	}

	for (int i = 1; i < path.size(); i++)
		map.DrawLine(path[i-1], path[i], gRock);
}

bool DropSand(Grid<char> &map) {
	Coordinate sandUnitPos = gSandRiftPos;

	bool canContinue = true;

	while (canContinue) {
		bool hasMoved = false;
		for (Coordinate sandFlow : gSandFlow) {
			Coordinate tmpPos = sandUnitPos + sandFlow;
			if (tmpPos.y >= DIMENSIONS) return false;
			if (map[tmpPos] != gAir && map[tmpPos] != gFlowingSand) continue;

			sandUnitPos = tmpPos;
			hasMoved = true;
			map[sandUnitPos] = gFlowingSand;
			break;
		}

		if (!hasMoved)
			canContinue = false;
	}

	map[sandUnitPos] = gSand;

	return true;
}

int main() {
	std::ifstream iFile("rock_scan.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	Grid<char> rockMap(DIMENSIONS, DIMENSIONS);
	rockMap.Fill(gAir);

	Coordinate topLeft{DIMENSIONS, DIMENSIONS};
	Coordinate bottomRight{0, 0};

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) continue;

		std::vector<Coordinate> newPath;
		ImportRockPath(buffer, rockMap, topLeft, bottomRight);
	}

	rockMap[gSandRiftPos] = gSandRift;

	if (topLeft.x > 0) topLeft.x--;
	if (topLeft.y > 0) topLeft.y = 0;
	if (bottomRight.x < DIMENSIONS - 1) bottomRight.x++;
	if (bottomRight.y < DIMENSIONS - 1) bottomRight.y++;

	int sandUnits = 0;
	while (DropSand(rockMap))
		sandUnits++;

	for (int y = topLeft.y; y <= bottomRight.y; y++) {
		for (int x = topLeft.x; x <= bottomRight.x; x++) {
			std::cout<<rockMap[{x,y}];
		}

		std::cout<<std::endl;
	}

	std::cout<<std::endl<<"Units of sand dropped: "<<sandUnits<<std::endl;

	iFile.close();
	return 0;
}
