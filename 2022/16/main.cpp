#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>
#include <map>

using namespace Syn;

struct BoardState {
	std::string previousRoom;
	int maxReleaseRate;
	int maxReleased;
	int maxPotential;
	int worstTime;
	std::map<std::string, bool> roomStates;
};

struct ValveRoom {
	std::string name;
	int flowRate;
	std::vector<std::string> connected;
	BoardState bestState;
};

struct Connection {
	int distance;
	std::string other;
};

const int gTotalTime = 30;
const std::string gStartingRoom = "AA";

ValveRoom ParseRoom(const std::string &input) {
	std::vector<std::string> tokens;
	Parser::Tokenize(input, tokens);

	if (tokens.size() < 10) return ValveRoom{"", 0};

	ValveRoom newRoom{ tokens[1], Parser::GetNumFromStr(tokens[4].c_str(), tokens[4].size()) };

	for (int i = 9; i < tokens.size(); i++) {
		Parser::Trim(tokens[i], ',');
		newRoom.connected.push_back(tokens[i]);
	}

	newRoom.bestState = {
			"",
			-1,
			-1,
			-1,
			gTotalTime
	};

	return newRoom;
}

int main() {
	std::ifstream iFile("valve_scan.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	std::map<std::string, ValveRoom> rooms;
	std::string currentRoom = gStartingRoom;

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) continue;

		ValveRoom room = ParseRoom(buffer);
		rooms.insert({room.name, room});

		std::cout<<"Imported room "<<room.name<<" with "<<room.connected.size()<<" connections"<<std::endl;
	}

	std::vector<std::string> roomsToCheck;
	roomsToCheck.push_back(gStartingRoom);
	BoardState &firstState = rooms[gStartingRoom].bestState;
	firstState.maxReleaseRate = rooms[gStartingRoom].flowRate;
	firstState.worstTime = rooms[gStartingRoom].flowRate > 0 ? 2 : 1;
	firstState.maxReleased = 0;
	firstState.maxPotential = firstState.maxReleaseRate * (gTotalTime - firstState.worstTime);

	while (!roomsToCheck.empty()) {
		std::cout<<std::endl<<"Checking room "<<roomsToCheck[0]<<std::endl;
		for (const std::string& roomId : rooms[roomsToCheck[0]].connected) {
			std::cout<<"Looking at connected room "<<roomId<<std::endl;
			BoardState potentialState = rooms[roomsToCheck[0]].bestState;
			int flowRate = rooms[roomId].flowRate;
			potentialState.previousRoom = roomId;
			potentialState.worstTime++;
			potentialState.maxReleased += potentialState.maxReleaseRate;

			if (flowRate != 0) {
				potentialState.maxReleaseRate += flowRate;
				potentialState.worstTime++;
				potentialState.maxPotential = potentialState.maxReleaseRate * (gTotalTime - potentialState.worstTime);
			}

			std::cout<<"Potential state: Worst time: "<<potentialState.worstTime<<", Max Potential: "<<potentialState.maxPotential;
			std::cout<<", Max rate: "<<potentialState.maxReleaseRate<<", Max released: "<<potentialState.maxReleased<<std::endl;

			if (potentialState.maxPotential > rooms[roomId].bestState.maxPotential) {
				std::cout<<"Replacing state of "<<roomId<<" with better state ("<<potentialState.maxPotential;
				std::cout<<" > "<<rooms[roomId].bestState.maxPotential<<")"<<std::endl;
				rooms[roomId].bestState = potentialState;
				roomsToCheck.push_back(roomId);
			} else {
				std::cout<<"Loop detected"<<std::endl;
			}
		}

		roomsToCheck.erase(roomsToCheck.cbegin());
	}

	// TODO: Output

	iFile.close();
	return 0;
}
