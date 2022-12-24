#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>
#include <Coordinate.h>
#include <unordered_set>

using namespace Syn;

Coordinate3D StormHeadings[] {
		{ 1,  0,  0},
		{-1,  0,  0},
		{ 0,  1,  0},
		{ 0, -1,  0},
		{ 0,  0,  1},
		{ 0,  0, -1}
};

bool CheckBounds(Coordinate3D &toCheck, Coordinate3D &lowBound, Coordinate3D &highBound) {
	if (toCheck.x < lowBound.x) return false;
	if (toCheck.x > highBound.x) return false;
	if (toCheck.y < lowBound.y) return false;
	if (toCheck.y > highBound.y) return false;
	if (toCheck.z < lowBound.z) return false;
	if (toCheck.z > highBound.z) return false;
	return true;
}

int main() {
	std::ifstream iFile("obsidian_scan.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	std::unordered_set<Coordinate3D> droplets;

	Coordinate3D lows {
		0xFFFF,
		0xFFFF,
		0xFFFF
	};

	Coordinate3D highs {
		-0xFFFF,
		-0xFFFF,
		-0xFFFF
	};

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) continue;

		Coordinate3D coordinate = Coordinate3D::Parse(buffer.c_str(), buffer.size());
		droplets.insert(coordinate);

		if (coordinate.x < lows.x) lows.x = coordinate.x;
		if (coordinate.y < lows.y) lows.y = coordinate.y;
		if (coordinate.z < lows.z) lows.z = coordinate.z;
		if (coordinate.x > highs.x) highs.x = coordinate.x;
		if (coordinate.y > highs.y) highs.y = coordinate.y;
		if (coordinate.z > highs.z) highs.z = coordinate.z;
	}

	std::cout<<"Loaded "<<droplets.size()<<" droplets."<<std::endl;
	std::cout<<"Bounds: ("<<lows.x<<", "<<lows.y<<", "<<lows.z<<") - ("<<highs.x<<", "<<highs.y<<", "<<highs.z<<")"<<std::endl;

	std::unordered_set<Coordinate3D> exposedAir;
	std::vector<Coordinate3D> searchQueue;
	lows -= {1,1,1};
	highs += {1,1,1};

	searchQueue.push_back(lows);

	while (!searchQueue.empty()) {
		Coordinate3D currentCoordinate = searchQueue.front();
		searchQueue.erase(searchQueue.cbegin());

		for (Coordinate3D direction : StormHeadings) {
			Coordinate3D potentialCoordinate = currentCoordinate + direction;

			if (!CheckBounds(potentialCoordinate, lows, highs)) continue;
			if (exposedAir.contains(potentialCoordinate)) continue;
			if (droplets.contains(potentialCoordinate)) continue;

			searchQueue.push_back(potentialCoordinate);
			exposedAir.insert(potentialCoordinate);
		}
	}

	int exposedFaces = 0;
	for (Coordinate3D coordinate : droplets)
		for (auto Direction : StormHeadings)
			if (exposedAir.contains(coordinate + Direction))
				exposedFaces++;

	std::cout<<"Surface Area: "<<exposedFaces<<std::endl;

	iFile.close();
	return 0;
}
