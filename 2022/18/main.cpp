#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>
#include <Coordinate.h>
#include <unordered_set>

using namespace Syn;

Coordinate3D Directions[] {
		{ 1,  0,  0},
		{-1,  0,  0},
		{ 0,  1,  0},
		{ 0, -1,  0},
		{ 0,  0,  1},
		{ 0,  0, -1}
};

int main() {
	std::ifstream iFile("obsidian_scan.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	std::unordered_set<Coordinate3D> droplets;

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) continue;

		droplets.insert(Coordinate3D::Parse(buffer.c_str(), buffer.size()));
	}

	std::cout<<"Loaded "<<droplets.size()<<" droplets."<<std::endl;

	int exposedFaces = 0;
	for (Coordinate3D coordinate : droplets)
		for (auto Direction : Directions)
			if (!droplets.contains(coordinate + Direction))
				exposedFaces++;

	std::cout<<"Surface Area: "<<exposedFaces<<std::endl;

	iFile.close();
	return 0;
}
