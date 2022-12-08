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

struct Visibility {
	int visibleNorth;
	int visibleSouth;
	int visibleEast;
	int visibleWest;

	int Total() {
		return visibleNorth + visibleSouth + visibleEast + visibleWest;
	}
};

struct TreeCandidate {
	Visibility visibility;
	Coordinate coordinate;
	int treeHeight;
	int scenicScore;

	static int CalcScenicScore(TreeCandidate &treeCandidate) {
		return treeCandidate.visibility.visibleNorth
		     * treeCandidate.visibility.visibleSouth
		     * treeCandidate.visibility.visibleEast
		     * treeCandidate.visibility.visibleWest;
	}
};

int **trees;
int width;
int height;

Visibility GetTreeVisibility(Coordinate &treeCoordinate, int &treeHeight) {
	Visibility visibility{};
	treeHeight = trees[treeCoordinate.y][treeCoordinate.x];

	// North visibility
	for (int y = treeCoordinate.y - 1; y >= 0; y--) {
		visibility.visibleNorth++;
		if (treeHeight <= trees[y][treeCoordinate.x]) break;
	}

	// South visibility
	for (int y = treeCoordinate.y + 1; y < height; y++) {
		visibility.visibleSouth++;
		if (treeHeight <= trees[y][treeCoordinate.x]) break;
	}

	// East visibility
	for (int x = treeCoordinate.x + 1; x < width; x++) {
		visibility.visibleEast++;
		if (treeHeight <= trees[treeCoordinate.y][x]) break;
	}

	// West visibility
	for (int x = treeCoordinate.x - 1; x >= 0; x--) {
		visibility.visibleWest++;
		if (treeHeight <= trees[treeCoordinate.y][x]) break;
	}

	return visibility;
}

TreeCandidate GetTreeCandidate(Coordinate &treeCoordinate) {
	TreeCandidate currentTree{};

	currentTree.visibility = GetTreeVisibility(treeCoordinate, currentTree.treeHeight);
	currentTree.scenicScore = TreeCandidate::CalcScenicScore(currentTree);
	currentTree.coordinate = treeCoordinate;

	return currentTree;
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

	TreeCandidate bestTreeCandidate{};
	bestTreeCandidate.visibility.visibleNorth = 0;
	bestTreeCandidate.visibility.visibleSouth = 0;
	bestTreeCandidate.visibility.visibleEast = 0;
	bestTreeCandidate.visibility.visibleWest = 0;
	bestTreeCandidate.treeHeight = 0;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			Coordinate treeCoordinate{};
			treeCoordinate.x = x;
			treeCoordinate.y = y;

			TreeCandidate treeCandidate = GetTreeCandidate(treeCoordinate);

			if (treeCandidate.scenicScore > bestTreeCandidate.scenicScore)
				bestTreeCandidate = treeCandidate;
		}
	}

	std::cout<<"The best tree has a height of "<<bestTreeCandidate.treeHeight;
	std::cout<<", and a scenic score of "<<bestTreeCandidate.scenicScore<<std::endl;
	std::cout<<"The tree can see "<<bestTreeCandidate.visibility.Total()<<" trees, which is"<<std::endl;
	std::cout<<bestTreeCandidate.visibility.visibleNorth<<" trees to the north, ";
	std::cout<<bestTreeCandidate.visibility.visibleSouth<<" trees to the south,"<<std::endl;
	std::cout<<bestTreeCandidate.visibility.visibleEast<<" trees to the east, and ";
	std::cout<<bestTreeCandidate.visibility.visibleWest<<" trees to the west."<<std::endl;
	std::cout<<"The tree is located at "<<bestTreeCandidate.coordinate.x+1<<",";
	std::cout<<bestTreeCandidate.coordinate.y+1<<std::endl;

	delete [] trees;
	iFile.close();
	return 0;
}
