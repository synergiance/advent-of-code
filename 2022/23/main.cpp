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

struct ElfData {
	int elvesWantingToMove;
	int proposeOffset;
	Coordinate lowerBound;
	Coordinate upperBound;
	std::unordered_set<Coordinate> elfLocations;
	std::unordered_map<Coordinate, Coordinate> elfProposals;
	std::unordered_set<Coordinate> proposals;
	std::unordered_set<Coordinate> contestedProposals;
};

const int ProgressPoint = 10;
const char HasElfChar = '#';
const char EmptyGroundChar = '.';
const char ProposedMoveChar = 'p';
const char ConflictedProposalChar = 'c';
const int ExpandPrintAreaBy = 2;

const Coordinate ProposeOrder[] {
		{0, -1}, // North
		{0, 1},  // South
		{-1, 0}, // West
		{1, 0}   // East
};

const Coordinate CheckedCoordinates[] {
		{-1, -1},
		{-1, 0},
		{-1, 1},
		{0, 1},
		{1, 1},
		{1, 0},
		{1, -1},
		{0, -1}
};

Coordinate GetProposal(Coordinate elfLocation, int proposeOffset, const std::unordered_set<Coordinate> &elfLocations) {
	bool wantsToMove = false;

	for (const Coordinate &checkedCoordinate : CheckedCoordinates) {
		if (elfLocations.contains(elfLocation + checkedCoordinate)) {
			wantsToMove = true;
			break;
		}
	}

	if (!wantsToMove) return elfLocation;

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

void CalcProposals(ElfData &elfData, int proposeOffset) {
	elfData.elfProposals.clear();
	elfData.proposals.clear();
	elfData.contestedProposals.clear();
	elfData.elvesWantingToMove = 0;

	for (Coordinate elfLocation : elfData.elfLocations) {
		Coordinate elfProposal = GetProposal(elfLocation, proposeOffset, elfData.elfLocations);
		elfData.elfProposals.insert({elfLocation, elfProposal});
		if (elfProposal != elfLocation) elfData.elvesWantingToMove++;

		if (elfData.proposals.contains(elfProposal)) {
			elfData.contestedProposals.insert(elfProposal);
		} else {
			elfData.proposals.insert(elfProposal);
		}
	}
}

void MoveElvesToProposals(ElfData &elfData) {
	elfData.elfLocations.clear();
	elfData.lowerBound = {0xFFFF, 0xFFFF};
	elfData.upperBound = {-0xFFFF, -0xFFFF};

	for (const auto &[elfLocation, elfProposal] : elfData.elfProposals) {
		auto newLocation = elfData.contestedProposals.contains(elfProposal) ? elfLocation : elfProposal;
		elfData.elfLocations.insert(newLocation);
		if (newLocation.x < elfData.lowerBound.x) elfData.lowerBound.x = newLocation.x;
		if (newLocation.y < elfData.lowerBound.y) elfData.lowerBound.y = newLocation.y;
		if (newLocation.x > elfData.upperBound.x) elfData.upperBound.x = newLocation.x;
		if (newLocation.y > elfData.upperBound.y) elfData.upperBound.y = newLocation.y;
	}
}

void PrintElfData(ElfData &elfData) {
	Coordinate pos{};
	std::cout<<std::endl;
	for (pos.y = elfData.lowerBound.y - ExpandPrintAreaBy; pos.y <= elfData.upperBound.y + ExpandPrintAreaBy; pos.y++) {
		for (pos.x = elfData.lowerBound.x - ExpandPrintAreaBy; pos.x <= elfData.upperBound.x + ExpandPrintAreaBy; pos.x++) {
			char ch = EmptyGroundChar;
			if (elfData.proposals.contains(pos)) ch = ProposedMoveChar;
			if (elfData.contestedProposals.contains(pos)) ch = ConflictedProposalChar;
			if (elfData.elfLocations.contains(pos)) ch = HasElfChar;
			std::cout<<ch;
		}
		std::cout<<std::endl;
	}
	std::cout<<"Wanting to move: "<<elfData.elvesWantingToMove<<std::endl;
}

void CommenceRound(ElfData &elfData) {
	CalcProposals(elfData, elfData.proposeOffset);
	//PrintElfData(elfData);

	elfData.proposeOffset++;
	elfData.proposeOffset %= 4;

	MoveElvesToProposals(elfData);
}

int main() {
	std::ifstream iFile("initial_elf_positions.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	ElfData elfData{1, 0, {0, 0}, {-1, -1}};

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) continue;

		int t = (int)buffer.size() - 1;
		if (t > elfData.upperBound.x)
			elfData.upperBound.x = t;
		elfData.upperBound.y++;

		for (int x = 0; x < buffer.size(); x++) {
			if (buffer[x] != HasElfChar) continue;
			elfData.elfLocations.insert({x, elfData.upperBound.y});
		}
	}

	iFile.close();
	std::cout<<"Loaded initial grid, dimensions: "<<elfData.upperBound.x+1<<"x"<<elfData.upperBound.y+1;
	std::cout<<", Elves: "<<elfData.elfLocations.size()<<std::endl;

	for (int i = 0; i < ProgressPoint; i++)
		CommenceRound(elfData);

	Coordinate dimensions = elfData.upperBound - elfData.lowerBound + Coordinate{1, 1};
	int totalTiles = dimensions.x * dimensions.y;
	int emptyTiles = totalTiles - (int)elfData.elfLocations.size();

	int roundCount = 10;

	while (elfData.elvesWantingToMove > 0) {
		CommenceRound(elfData);
		roundCount++;
	}

	elfData.proposals.clear();
	elfData.contestedProposals.clear();
	elfData.elfProposals.clear();
	//PrintElfData(elfData);

	std::cout<<std::endl<<"Empty tiles after "<<ProgressPoint<<" rounds: "<<emptyTiles<<std::endl;
	std::cout<<"Number of rounds: "<<roundCount<<std::endl;

	return 0;
}
