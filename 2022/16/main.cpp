#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>
#include <map>

using namespace Syn;

struct ValveRoom {
	std::string name;
	int flowRate;
	std::vector<std::string> connected;
};

const int gTotalTime = 30;
const std::string gStartingRoom = "AA";

ValveRoom ParseRoom(const std::string &input) {
	std::vector<std::string> tokens;
	Parser::Tokenize(input, tokens);

	if (tokens.size() < 10) return ValveRoom{"", 0};

	ValveRoom newRoom{};
	newRoom.name = tokens[1];
	newRoom.flowRate = Parser::GetNumFromStr(tokens[4].c_str(), tokens[4].size());

	for (int i = 9; i < tokens.size(); i++) {
		Parser::Trim(tokens[i], ',');
		newRoom.connected.push_back(tokens[i]);
	}

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

	// TODO: Output

	iFile.close();
	return 0;
}
