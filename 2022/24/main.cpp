#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>
#include <Grid.h>
#include <unordered_set>

using namespace Syn;

struct Blizzard {
	int direction;
	Coordinate location;
};

const char ValleyWallChar = '#';
const char FlatGroundChar = '.';

const char StormNorthChar = '^';
const char StormSouthChar = 'v';
const char StormEastChar = '>';
const char StormWestChar = '<';

const char ExpeditionChar = 'E';

const Coordinate StormHeadings[] {
		{0, -1}, // North
		{1, 0},  // East
		{0, 1},  // South
		{-1, 0}  // West
};

const Coordinate PossibleMovements[] {
		{0, -1}, // North
		{1, 0},  // East
		{0, 1},  // South
		{-1, 0}, // West
		{0, 0}   // Stay
};

const char StormHeadingSymbols[] {
	StormNorthChar,
	StormEastChar,
	StormSouthChar,
	StormWestChar
};

Blizzard CreateBlizzard(Coordinate &pos, char inputChar) {
	Blizzard blizzard{};
	blizzard.location = pos;

	switch (inputChar) {
		case StormNorthChar:
			blizzard.direction = 0;
			break;
		case StormSouthChar:
			blizzard.direction = 2;
			break;
		case StormEastChar:
			blizzard.direction = 1;
			break;
		case StormWestChar:
			blizzard.direction = 3;
			break;
		default:
			blizzard.direction = -1;
			break;
	}

	return blizzard;
}

void RedrawStormOverlay(std::vector<Blizzard> &blizzards, Grid<char> &stormOverlay) {
	stormOverlay.Fill('\0');
	for (Blizzard &blizzard : blizzards) {
		switch (stormOverlay[blizzard.location]) {
			case '\0':
				stormOverlay[blizzard.location] = StormHeadingSymbols[blizzard.direction];
				break;
			case StormNorthChar:
			case StormSouthChar:
			case StormEastChar:
			case StormWestChar:
				stormOverlay[blizzard.location] = '2';
				break;
			case '2':
			case '3':
				stormOverlay[blizzard.location]++;
				break;
		}
	}
}

void SimulateStorm(Blizzard &blizzard, Grid<char> &valleyLayout) {
	do {
		blizzard.location += StormHeadings[blizzard.direction];
		blizzard.location %= {valleyLayout.GetWidth(), valleyLayout.GetHeight()};
		if (blizzard.location.x < 0) blizzard.location.x += valleyLayout.GetWidth();
		if (blizzard.location.y < 0) blizzard.location.y += valleyLayout.GetHeight();
	} while (valleyLayout[blizzard.location] == ValleyWallChar);
}

void SimulateWeather(std::vector<Blizzard> &blizzards, Grid<char> &stormOverlay, Grid<char> &valleyLayout) {
	for (Blizzard &blizzard : blizzards)
		SimulateStorm(blizzard, valleyLayout);
	RedrawStormOverlay(blizzards, stormOverlay);
}

void DrawPossibilityOverlay(std::unordered_set<Coordinate> &possibilities, Grid<char> &possibilityOverlay) {
	possibilityOverlay.Fill('\0');
	for (const Coordinate &possibility : possibilities)
		possibilityOverlay[possibility] = ExpeditionChar;
}

void PrintMaps(Grid<char> &valleyLayout, Grid<char> &stormOverlay, Grid<char> &possibilityOverlay) {
	Coordinate pos{};
	std::cout<<std::endl;
	for (pos.y = 0; pos.y < valleyLayout.GetHeight(); pos.y++) {
		for (pos.x = 0; pos.x < valleyLayout.GetWidth(); pos.x++) {
			char ch = valleyLayout[pos];
			if (stormOverlay[pos] != '\0') ch = stormOverlay[pos];
			if (possibilityOverlay[pos] != '\0') ch = possibilityOverlay[pos];
			std::cout<<ch;
		}
		std::cout<<std::endl;
	}
}

int main() {
	std::ifstream iFile("blizzard_map.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::vector<std::string> buffer;
	Reader::ReadUntilEmptyLn(iFile, buffer);
	iFile.close();

	Grid<char> valleyLayout((int)buffer[0].size(), (int)buffer.size());
	Grid<char> stormOverlay((int)buffer[0].size(), (int)buffer.size());
	std::vector<Blizzard> blizzards;

	for (int y = 0; y < valleyLayout.GetHeight(); y++) {
		for (int x = 0; x < valleyLayout.GetWidth(); x++) {
			Coordinate pos{x, y};
			char currentInput = buffer[y][x];
			switch (currentInput) {
				case ValleyWallChar:
				case FlatGroundChar:
					valleyLayout[pos] = currentInput;
					stormOverlay[pos] = '\0';
					break;
				case StormNorthChar:
				case StormSouthChar:
				case StormEastChar:
				case StormWestChar:
					stormOverlay[pos] = currentInput;
					valleyLayout[pos] = FlatGroundChar;
					blizzards.push_back(CreateBlizzard(pos, currentInput));
					break;
				default:
					valleyLayout[pos] = FlatGroundChar;
					stormOverlay[pos] = '\0';
					break;
			}
		}
	}

	std::cout<<"Loaded valley and storm data."<<std::endl;
	std::cout<<"Valley size: "<<valleyLayout.GetWidth()<<"x"<<valleyLayout.GetHeight()<<std::endl;
	std::cout<<"Blizzards: "<<blizzards.size()<<std::endl;

	Coordinate finalPos = {
			valleyLayout.GetWidth() - 2,
			valleyLayout.GetHeight() - 1
	};

	std::unordered_set<Coordinate> possiblePositions;
	std::unordered_set<Coordinate> nextPossibilities;

	Grid<char> possibilityOverlay((int)buffer[0].size(), (int)buffer.size());

	buffer.clear();

	nextPossibilities.insert({1, 0});
	int timePassed = 0;

	do {
		possiblePositions = nextPossibilities;
		nextPossibilities.clear();
		SimulateWeather(blizzards, stormOverlay, valleyLayout);

		for (Coordinate position : possiblePositions) {
			for (Coordinate movement : PossibleMovements) {
				Coordinate potentialCoordinate = movement + position;
				if (potentialCoordinate.y < 0 || potentialCoordinate.y >= valleyLayout.GetHeight()) continue;

				if (valleyLayout[potentialCoordinate] == ValleyWallChar) continue;
				if (stormOverlay[potentialCoordinate] != '\0') continue;

				nextPossibilities.insert(potentialCoordinate);
			}
		}

		timePassed++;
		DrawPossibilityOverlay(nextPossibilities, possibilityOverlay);
		PrintMaps(valleyLayout, stormOverlay, possibilityOverlay);
		std::cout<<"Possibilities found: "<<nextPossibilities.size()<<std::endl;
		if (nextPossibilities.contains(finalPos)) break;
	} while (!nextPossibilities.empty());

	std::cout<<"Lowest amount of time in valley: "<<timePassed<<std::endl;

	return 0;
}
