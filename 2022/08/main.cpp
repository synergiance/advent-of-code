#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>

using namespace Syn;

struct Coordinate {
	int x;
	int y;
};

int **trees;
int width;
int height;

int getScenicScore(Coordinate treeCoordinate) {
	int visibleNorth = 0;
	int visibleSouth = 0;
	int visibleEast = 0;
	int visibleWest = 0;

	int treeHeight = trees[treeCoordinate.y][treeCoordinate.x];

	// North visibility
	for (int y = treeCoordinate.y - 1; y >= 0; y--) {
		visibleNorth++;
		if (treeHeight <= trees[y][treeCoordinate.x]) break;
	}

	// South visibility
	for (int y = treeCoordinate.y + 1; y < height; y++) {
		visibleSouth++;
		if (treeHeight <= trees[y][treeCoordinate.x]) break;
	}

	// East visibility
	for (int x = treeCoordinate.x + 1; x < width; x++) {
		visibleEast++;
		if (treeHeight <= trees[treeCoordinate.y][x]) break;
	}

	// West visibility
	for (int x = treeCoordinate.x - 1; x >= 0; x--) {
		visibleWest++;
		if (treeHeight <= trees[treeCoordinate.y][x]) break;
	}

	return visibleNorth * visibleSouth * visibleEast * visibleWest;
}

int main() {
	std::ifstream iFile("tree_heights.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	std::vector<std::string> treeInput;
	Reader::ReadUntilEmptyLn(iFile, treeInput);

	height = (int)treeInput.size();
	width = (int)treeInput[0].size();

	trees = new int*[height];
	for (int i = 0; i < height; i++)
		trees[i] = new int[width];

	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
			trees[y][x] = treeInput[y][x] - '0';

	int highestScore = 0;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			Coordinate treeCoordinate{};
			treeCoordinate.x = x;
			treeCoordinate.y = y;

			int treeScore = getScenicScore(treeCoordinate);

			if (treeScore > highestScore)
				highestScore = treeScore;
		}
	}

	std::cout<<"Best Scenic Score: "<<highestScore<<std::endl;

	delete [] trees;
	iFile.close();
	return 0;
}
