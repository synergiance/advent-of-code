#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>
#include <Grid.h>
#include <Coordinate.h>
#include <unordered_set>

using namespace Syn;

const Coordinate movementDirections[] = {
		{0, 1},
		{0, -1},
		{-1, 0},
		{1, 0}
};

const std::string movementSymbols[] = { "↓", "↑", "←", "→" };

std::string ToStringCoordinate(Coordinate coordinate) {
	std::string str = "(" + std::to_string(coordinate.x);
	str += "," + std::to_string(coordinate.y) + ")";
	return str;
}

void PrintPath(Grid<int> &path) {
	std::cout<<std::endl;
	for (int y = 0; y < path.GetHeight(); y++) {
		for (int x = 0; x < path.GetWidth(); x++) {
			int data = path[{x, y}];
			if (data > 0) {
				data /= 4;
				data %= 62;
			}
			char ch;
			if (data < 0) ch = ' ';
			else if (data < 26) ch = (char)data + 'a';
			else if (data < 52) ch = (char)(data - 26) + 'A';
			else if (data < 62) ch = (char)(data - 52) + '0';
			std::cout<<ch;
		}
		std::cout<<std::endl;
	}
}

bool TestCoordinates(Coordinate &current, Coordinate &test, Grid<int> &height, Grid<int> &path) {
	if (test.x < 0 || test.x >= height.GetWidth()) return false;
	if (test.y < 0 || test.y >= height.GetHeight()) return false;

	int currentDist = path[current];
	int testDist = path[test];

	if (testDist >= 0 && testDist <= currentDist + 1) return false;

	int diff = height[current] - height[test];
	if (diff > 1) return false;

	path[test] = currentDist + 1;
	return true;
}

void GeneratePathMap(Coordinate &targetPos, Grid<int> &height, Grid<int> &path) {
	std::vector<Coordinate> coordinateStack;
	std::unordered_set<Coordinate> coordinateSet;
	coordinateStack.push_back(targetPos);
	coordinateSet.insert(targetPos);
	path[targetPos] = 0;

	while (!coordinateStack.empty()) {
		Coordinate currentCoordinate = coordinateStack.front();
		coordinateStack.erase(coordinateStack.cbegin());
		coordinateSet.erase(currentCoordinate);

		for (Coordinate movement : movementDirections) {
			Coordinate testCoordinate = currentCoordinate + movement;
			if (!TestCoordinates(currentCoordinate, testCoordinate, height, path)) continue;
			if (coordinateSet.contains(testCoordinate))continue;

			coordinateStack.push_back(testCoordinate);
			coordinateSet.insert(testCoordinate);
		}
	}
}

int WalkPathMap(Coordinate position, Coordinate target, Grid<int> &height, Grid<int> &path, Grid<int> walk) {
	int numSteps = 0;
	while (path[position] > 0) {
		int lowestDistance = path[position] + 2;
		Coordinate nextDirection = {0,0};

		for (Coordinate direction : movementDirections) {
			Coordinate testLocation = direction + position;
			if (path[testLocation] >= lowestDistance) continue;
			if (path[position] - path[testLocation] > 1) continue;

			lowestDistance = path[testLocation];
			nextDirection = direction;
		}

		for (int i = 0; i < 4; i++) {
			if (nextDirection != movementDirections[i]) continue;
			walk[position] = i+1;
			break;
		}

		if (walk[position] == 0)
			walk[position] = 5;

		position += nextDirection;
		numSteps++;
	}

	return numSteps;
}

int main() {
	std::ifstream iFile("map.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;

	std::vector<std::string> input;
	Reader::ReadEntireFile(iFile, input);

	int width = (int)input[0].size();
	int height = (int)input.size();

	Grid<int> heightMap(width, height);
	Grid<int> pathMap(width, height);
	Grid<int> walkingMap(width, height);

	Coordinate startingPos{};
	Coordinate targetPos{};

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			char ch = input[y][x];

			if (ch == 'S') {
				startingPos = {x, y};
				ch = 'a';
			} else if (ch == 'E') {
				targetPos = {x, y};
				ch = 'z';
			}

			heightMap[{x, y}] = (int)(ch - 'a');
			pathMap[{x, y}] = -2;
			walkingMap[{x, y}] = 0;
		}
	}

	GeneratePathMap(targetPos, heightMap, pathMap);

	//PrintPath(pathMap);

	int numSteps = WalkPathMap(startingPos, targetPos, heightMap, pathMap, walkingMap);

	int lowestDistance = 0xFFFF;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			Coordinate current = {x, y};
			if (heightMap[current] != 0) continue;
			int dist = pathMap[current];
			if (dist < 0) continue;
			if (dist < lowestDistance)
				lowestDistance = dist;
		}
	}

	std::cout<<std::endl;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int direction = walkingMap[{x, y}];
			char ch = input[y][x];
			switch (direction) {
				case 1:
				case 2:
				case 3:
				case 4:
					//std::cout<<movementSymbols[direction - 1];
					std::cout<<"\x1b[31m"<<ch<<"\x1b[0m";
					break;
				case 0:
					std::cout<<ch;
					break;
				default:
					std::cout<<"\x1b[34m"<<ch<<"\x1b[0m";
					break;
			}
		}

		std::cout<<std::endl;
	}

	std::cout<<std::endl<<"Number of steps: "<<numSteps<<std::endl;
	std::cout<<"Shortest path: "<<lowestDistance<<std::endl;

	iFile.close();
	return 0;
}
