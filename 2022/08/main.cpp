#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>

using namespace Syn;

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

	int h = (int)treeInput.size();
	int w = (int)treeInput[0].size();

	int **trees = new int*[h];
	bool **treesVisible = new bool*[h];
	for (int i = 0; i < h; i++) {
		trees[i] = new int[w];
		treesVisible[i] = new bool[w];
	}

	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			trees[y][x] = treeInput[y][x] - '0';
			treesVisible[y][x] = false;
		}
	}

	// North-South visibility
	for (int x = 0; x < w; x++) {
		int tallestNorth = -1;
		int tallestSouth = -1;

		for (int y = 0; y < h; y++) {
			if (trees[y][x] > tallestNorth) {
				tallestNorth = trees[y][x];
				treesVisible[y][x] = true;
			}

			int yInv = h - y - 1;
			if (trees[yInv][x] > tallestSouth) {
				tallestSouth = trees[yInv][x];
				treesVisible[yInv][x] = true;
			}
		}
	}

	// East-West visibility
	for (int y = 0; y < h; y++) {
		int tallestEast = -1;
		int tallestWest = -1;

		for (int x = 0; x < w; x++) {
			if (trees[y][x] > tallestWest) {
				tallestWest = trees[y][x];
				treesVisible[y][x] = true;
			}

			int xInv = w - x - 1;
			if (trees[y][xInv] > tallestEast) {
				tallestEast = trees[y][xInv];
				treesVisible[y][xInv] = true;
			}
		}
	}

	int numVisible = 0;
	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
			if (treesVisible[y][x])
				numVisible++;

	std::cout<<"Number of visible trees: "<<numVisible<<std::endl;

	delete [] trees;
	delete [] treesVisible;
	iFile.close();
	return 0;
}
