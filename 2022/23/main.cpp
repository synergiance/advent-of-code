#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>
#include <Coordinate.h>
#include <unordered_set>
#include <unordered_map>

using namespace Syn;

const int ProgressPoint = 10;
const char HasElfChar = '#';

const Coordinate ProposeOrder[] {
		{0, -1}, // North
		{0, 1},  // South
		{-1, 0}, // West
		{1, 0}   // East
};

Coordinate GetProposal(Coordinate elfLocation, int proposeOffset, const std::unordered_set<Coordinate> &elfLocations) {
	for (int i = 0; i < 4; i++) {
		int proposeIdx = (proposeOffset + i) % 4;
		Coordinate proposeLocation = elfLocation + ProposeOrder[proposeIdx];
		if (elfLocations.contains(proposeLocation)) continue;
		proposeIdx ^= 0b00000010;
		Coordinate testLocation = proposeLocation + ProposeOrder[proposeIdx];
		if (elfLocations.contains(testLocation)) continue;
		proposeIdx ^= 0b00000001;
		testLocation = proposeLocation + ProposeOrder[proposeIdx];
		if (elfLocations.contains(testLocation)) continue;

		return proposeLocation;
	}

	return elfLocation;
}

int main() {
	std::ifstream iFile("initial_elf_positions.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	Coordinate lowerBound = {0, 0};
	Coordinate upperBound = {-1, -1};
	std::unordered_set<Coordinate> elfLocations;

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) continue;

		int t = (int)buffer.size() - 1;
		if (t > upperBound.x)
			upperBound.x = t;
		upperBound.y++;

		for (int x = 0; x < buffer.size(); x++) {
			if (buffer[x] != HasElfChar) continue;
			elfLocations.insert({x, upperBound.y});
		}
	}

	iFile.close();
	std::cout<<"Loaded initial grid, dimensions: "<<upperBound.x+1<<"x"<<upperBound.y+1;
	std::cout<<", Elves: "<<elfLocations.size()<<std::endl;

	std::unordered_map<Coordinate, Coordinate> elfProposals;
	std::unordered_set<Coordinate> proposals;
	std::unordered_set<Coordinate> contestedProposals;

	int proposeOffset = 0;

	for (int i = 0; i < ProgressPoint; i++) {
		elfProposals.clear();
		proposals.clear();
		contestedProposals.clear();

		for (Coordinate elfLocation : elfLocations) {
			Coordinate elfProposal = GetProposal(elfLocation, proposeOffset, elfLocations);
			elfProposals.insert({elfLocation, elfProposal});

			if (proposals.contains(elfProposal)) {
				contestedProposals.insert(elfProposal);
			} else {
				proposals.insert(elfProposal);
			}
		}

		proposeOffset++;
		proposeOffset %= 4;

		elfLocations.clear();
		lowerBound = {0xFFFF, 0xFFFF};
		upperBound = {-0xFFFF, -0xFFFF};

		for (const auto &[elfLocation, elfProposal] : elfProposals) {
			auto newLocation = contestedProposals.contains(elfProposal) ? elfLocation : elfProposal;
			elfLocations.insert(newLocation);
			if (newLocation.x < lowerBound.x) lowerBound.x = newLocation.x;
			if (newLocation.y < lowerBound.y) lowerBound.y = newLocation.y;
			if (newLocation.x > upperBound.x) upperBound.x = newLocation.x;
			if (newLocation.y > upperBound.y) upperBound.y = newLocation.y;
		}
	}

	Coordinate dimensions = upperBound - lowerBound + Coordinate{1, 1};
	int totalTiles = dimensions.x * dimensions.y;
	int emptyTiles = totalTiles - (int)elfLocations.size();

	std::cout<<"Empty tiles: "<<emptyTiles<<std::endl;

	return 0;
}
