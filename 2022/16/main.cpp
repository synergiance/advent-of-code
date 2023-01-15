#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <Parser.h>
#include <Reader.h>
#include <unordered_map>
#include <unordered_set>

using namespace Syn;

enum Action { open, move };

struct ActionLog {
	Action action;
	std::string valve;

	friend std::ostream &operator<<(std::ostream &out, const ActionLog &actionLog) {
		out<<((actionLog.action == open) ? "Opened " : "Moved to ")<<actionLog.valve;
		return out;
	}
};

struct Valve {
	std::string name;
	int flowRate;
	std::vector<std::string> connected;
	std::unordered_map<std::string,int> valveTimes;

	void CalcValveTimes(std::unordered_map<std::string, Valve> &valves);
	static Valve Parse(const std::string &input);
};

struct BoardState {
	int timeLeft;
	int currentFlowRate;
	int pressureReleased;
	std::string currentLocation;
	std::unordered_set<std::string> openValves;
	std::vector<ActionLog> actionLog;

	void PrintValves() const {
		if (openValves.empty()) {
			std::cout<<"No open valves!"<<std::endl;
			return;
		}

		bool first = true;
		std::cout<<"Open valves: ";
		for (const std::string &valve : openValves) {
			if (first) {
				std::cout<<valve;
				first = false;
			} else {
				std::cout<<", "<<valve;
			}
		}

		std::cout<<std::endl;
	}

	void PrintLog() const {
		if (actionLog.empty()) {
			std::cout<<"No actions!"<<std::endl;
			return;
		}

		bool first = true;
		std::cout<<"Action Log: ";
		for (const ActionLog &log : actionLog) {
			if (first) {
				std::cout<<log;
				first = false;
			} else {
				std::cout << ", " << log;
			}
		}
		std::cout<<std::endl;
	}
};

struct ValveRoom {
	std::string name;
	int flowRate;
	std::vector<std::string> connected;
};

struct SeenState {
	std::unordered_set<std::string> openedValves;
	std::string currentValve;
	int ventedPressure;
	int timeLeft;

	bool operator==(const SeenState &other) const {
		if (timeLeft != other.timeLeft) return false;
		if (ventedPressure != other.ventedPressure) return false;
		if (currentValve != other.currentValve) return false;
		for (const std::string &valve : openedValves)
			if (!other.openedValves.contains(valve)) return false;
		for (const std::string &valve : other.openedValves)
			if (!openedValves.contains(valve)) return false;
		return true;
	}
};

namespace std {
	template<> struct hash<SeenState> {
		size_t operator()(const SeenState &seenState) const noexcept {
			size_t hash = std::hash<int>{}(seenState.timeLeft) << 3;
			hash ^= std::hash<int>{}(seenState.ventedPressure) << 2;
			hash ^= std::hash<std::string>{}(seenState.currentValve) << 1;
			for (const std::string &valve : seenState.openedValves)
				hash ^= std::hash<std::string>{}(valve);
			return hash;
		}
	};
}

const std::string gStartingRoom = "AA";
const int gTotalTime = 30;

int gBestFound = -1;

std::unordered_set<SeenState> gSeenStates;

ValveRoom ParseRoom(const std::string &input) {
	std::vector<std::string> tokens;
	Parser::Tokenize(input, tokens);

	if (tokens.size() < 10) return ValveRoom{"", 0};

	ValveRoom newRoom{ tokens[1], Parser::GetNumFromStr(tokens[4].c_str(), tokens[4].size()) };

	for (int i = 9; i < tokens.size(); i++) {
		Parser::Trim(tokens[i], ',');
		newRoom.connected.push_back(tokens[i]);
	}

	return newRoom;
}

Valve Valve::Parse(const std::string &input) {
	std::vector<std::string> tokens;
	Parser::Tokenize(input, tokens);

	if (tokens.size() < 10)
		return Valve{"", 0};

	Valve newValve {
			tokens[1],
			Parser::GetNumFromStr(tokens[4].c_str(), tokens[4].size())
	};

	for (int i = 9; i < tokens.size(); i++) {
		Parser::Trim(tokens[i], ',');
		newValve.connected.push_back(tokens[i]);
	}

	return newValve;
}

void Valve::CalcValveTimes(std::unordered_map<std::string, Valve> &valves) {
	// BFS search for all non-zero valves
	std::queue<std::string> valvesToSearch;
	std::unordered_map<std::string, int> allValveTimes;
	valvesToSearch.push(name);
	allValveTimes.insert({name, 1});

	while (!valvesToSearch.empty()) {
		std::string &valveName = valvesToSearch.front();
		int time = allValveTimes[valveName] + 1;
		if (time > gTotalTime) break;

		for (const std::string &connectedValve : valves[valveName].connected) {
			if (allValveTimes.contains(connectedValve)) continue;

			if (valves[connectedValve].flowRate > 0)
				valveTimes.insert({connectedValve, time});

			valvesToSearch.push(connectedValve);
			allValveTimes.insert({connectedValve, time});
		}

		valvesToSearch.pop();
	}
}

void Simulate(BoardState &boardState) {
	boardState.timeLeft--;
	boardState.pressureReleased += boardState.currentFlowRate;
}

BoardState FindBestFlow(const BoardState& currentBoardState, std::unordered_map<std::string, ValveRoom> &rooms) {
	if (currentBoardState.timeLeft == 0) {
		if (currentBoardState.pressureReleased > gBestFound) {
			gBestFound = currentBoardState.pressureReleased;
			std::cout<<"Found new candidate: "<<currentBoardState.pressureReleased<<std::endl;
			currentBoardState.PrintValves();
			currentBoardState.PrintLog();
			std::cout<<std::endl;
		}
		return currentBoardState;
	}

	if (currentBoardState.currentFlowRate < 20 && currentBoardState.timeLeft < 10)
		return { 0, -1, -1, gStartingRoom };

	ValveRoom &currentRoom = rooms[currentBoardState.currentLocation];
	BoardState bestState{0, -1, 0, gStartingRoom};

	if (currentRoom.flowRate > 0 && !currentBoardState.openValves.contains(currentBoardState.currentLocation)) {
		bestState = currentBoardState;
		Simulate(bestState);
		bestState.openValves.insert(currentBoardState.currentLocation);
		bestState.currentFlowRate += currentRoom.flowRate;
		bestState.actionLog.push_back({open, currentBoardState.currentLocation});
		bestState = FindBestFlow(bestState, rooms);
	}

	const std::vector<ActionLog> &currentActionLog = currentBoardState.actionLog;

	for (const std::string &connectedRoom : currentRoom.connected) {
		if (currentActionLog.size() >= 2 && currentActionLog[currentActionLog.size() - 2].valve == connectedRoom) continue;
		BoardState testState = currentBoardState;
		Simulate(testState);
		testState.currentLocation = connectedRoom;
		testState.actionLog.push_back({move, connectedRoom});
		testState = FindBestFlow(testState, rooms);
		if (testState.pressureReleased > bestState.pressureReleased)
			bestState = testState;
	}

	return bestState;
}

int main() {
	std::ifstream iFile("valve_scan.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	std::unordered_map<std::string, ValveRoom> rooms;

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) continue;

		ValveRoom room = ParseRoom(buffer);
		rooms.insert({room.name, room});

		std::cout<<"Imported room "<<room.name<<" with "<<room.connected.size();
		std::cout<<" connections and a flow rate of "<<room.flowRate<<std::endl;
	}

	BoardState bestFlow = FindBestFlow({
		gTotalTime,
		0,
		0,
		gStartingRoom
		}, rooms);

	std::cout<<"Most pressure released: "<<bestFlow.pressureReleased<<std::endl;

	iFile.close();
	return 0;
}
