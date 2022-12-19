#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>
#include <map>

using namespace Syn;

#define TIME_TO_WORK 32

struct Blueprint {
	int oreRobotCost;
	int clayRobotCost;
	int obsidianRobotCostOre;
	int obsidianRobotCostClay;
	int geodeRobotCostOre;
	int geodeRobotCostObsidian;
};

struct GameState {
	int numOreRobots;
	int numClayRobots;
	int numObsidianRobots;
	int numGeodeRobots;

	int numOre;
	int numClay;
	int numObsidian;
	int numGeode;

	int timeLeft;

	size_t operator()(const GameState &gameState) const noexcept {
		size_t val = gameState.numOreRobots << 5;
		val ^= gameState.numClayRobots << 9;
		val ^= gameState.numObsidianRobots << 13;
		val ^= gameState.numGeodeRobots << 17;
		val ^= gameState.numOre << 8;
		val ^= gameState.numClay << 12;
		val ^= gameState.numObsidian << 16;
		val ^= gameState.numGeode << 20;
		val ^= gameState.timeLeft << 25;
		return val;
	}

	bool operator==(const GameState &other) const {
		return numOreRobots == other.numOreRobots &&
		       numClayRobots == other.numClayRobots &&
		       numObsidianRobots == other.numObsidianRobots &&
		       numGeodeRobots == other.numGeodeRobots &&
		       numOre == other.numOre &&
		       numClay == other.numClay &&
		       numObsidian == other.numObsidian &&
		       numGeode == other.numGeode &&
		       timeLeft == other.timeLeft;
	}

	bool operator!=(const GameState &other) const {
		return numOreRobots != other.numOreRobots ||
		       numClayRobots != other.numClayRobots ||
		       numObsidianRobots != other.numObsidianRobots ||
		       numGeodeRobots != other.numGeodeRobots ||
		       numOre != other.numOre ||
		       numClay != other.numClay ||
		       numObsidian != other.numObsidian ||
		       numGeode != other.numGeode ||
		       timeLeft != other.timeLeft;
	}
};

namespace std {
	template <> struct less<GameState> {
		bool operator()(const GameState &lhs, const GameState &rhs) const {
			return lhs(lhs) < rhs(rhs);
		}
	};
}

std::map<GameState, GameState> endStates;

Blueprint ParseBlueprint(const std::string &input) {
	std::vector<std::string> tokens;
	Parser::Tokenize(input, tokens);
	Blueprint newBlueprint{};
	newBlueprint.oreRobotCost = atoi(tokens[6].c_str());
	newBlueprint.clayRobotCost = atoi(tokens[12].c_str());
	newBlueprint.obsidianRobotCostOre = atoi(tokens[18].c_str());
	newBlueprint.obsidianRobotCostClay = atoi(tokens[21].c_str());
	newBlueprint.geodeRobotCostOre = atoi(tokens[27].c_str());
	newBlueprint.geodeRobotCostObsidian = atoi(tokens[30].c_str());
	return newBlueprint;
}

void ApplyGameState(GameState &gameState) {
	gameState.numOre += gameState.numOreRobots;
	gameState.numClay += gameState.numClayRobots;
	gameState.numObsidian += gameState.numObsidianRobots;
	gameState.numGeode += gameState.numGeodeRobots;
	gameState.timeLeft--;
}

void PrintState(GameState &gameState) {
	std::cout<<"Ore: ("<<gameState.numOre<<", "<<gameState.numOreRobots<<"), Clay: ("<<gameState.numClay<<", ";
	std::cout<<gameState.numClayRobots<<"), Obsidian: ("<<gameState.numObsidian<<", "<<gameState.numObsidianRobots;
	std::cout<<"), Geodes: ("<<gameState.numGeode<<", "<<gameState.numGeodeRobots<<")"<<std::endl;
}

GameState GetMaxGeodes(const Blueprint &blueprint, const GameState &gameState) {
	if (gameState.timeLeft == 0) {
		return gameState;
	}

	if (endStates.contains(gameState)) return endStates[gameState];

	GameState newGameState = gameState;
	GameState bestState = {
			0,0,0,0,0,0,0,0
	};

	if (gameState.timeLeft == 1 && gameState.numGeodeRobots == 0)
		return bestState;

	if (gameState.timeLeft == 2 && gameState.numObsidianRobots == 0)
		return bestState;

	if (gameState.timeLeft == 3 && gameState.numClayRobots == 0)
		return bestState;

	bool canDoEverything = true;
	bool noMoreTests = false;

	if (gameState.numOre >= blueprint.geodeRobotCostOre && gameState.numObsidian >= blueprint.geodeRobotCostObsidian) {
		noMoreTests = true;
		newGameState.numOre -= blueprint.geodeRobotCostOre;
		newGameState.numObsidian -= blueprint.geodeRobotCostObsidian;
		ApplyGameState(newGameState);
		newGameState.numGeodeRobots++;
		GameState potentialState = GetMaxGeodes(blueprint, newGameState);
		if (potentialState.numGeode > bestState.numGeode) bestState = potentialState;
		newGameState = gameState;
	} else {
		canDoEverything = false;
	}

	if (gameState.numOre >= blueprint.obsidianRobotCostOre && gameState.numClay >= blueprint.obsidianRobotCostClay) {
		if (!noMoreTests) {
			noMoreTests = true;
			newGameState.numOre -= blueprint.obsidianRobotCostOre;
			newGameState.numClay -= blueprint.obsidianRobotCostClay;
			ApplyGameState(newGameState);
			newGameState.numObsidianRobots++;
			GameState potentialState = GetMaxGeodes(blueprint, newGameState);
			if (potentialState.numGeode > bestState.numGeode) bestState = potentialState;
			newGameState = gameState;
		}
	} else {
		canDoEverything = false;
	}

	if (gameState.numOre >= blueprint.clayRobotCost) {
		if (!noMoreTests) {
			newGameState.numOre -= blueprint.clayRobotCost;
			ApplyGameState(newGameState);
			newGameState.numClayRobots++;
			GameState potentialState = GetMaxGeodes(blueprint, newGameState);
			if (potentialState.numGeode > bestState.numGeode) bestState = potentialState;
			newGameState = gameState;
		}
	} else {
		canDoEverything = false;
	}

	if (gameState.numOre >= blueprint.oreRobotCost) {
		if (!noMoreTests) {
			newGameState.numOre -= blueprint.oreRobotCost;
			ApplyGameState(newGameState);
			newGameState.numOreRobots++;
			GameState potentialState = GetMaxGeodes(blueprint, newGameState);
			if (potentialState.numGeode > bestState.numGeode) bestState = potentialState;
			newGameState = gameState;
		}
	} else {
		canDoEverything = false;
	}

	if (!canDoEverything) {
		ApplyGameState(newGameState);
		GameState potentialState = GetMaxGeodes(blueprint, newGameState);
		if (potentialState.numGeode > bestState.numGeode) bestState = potentialState;
	}

	endStates.insert({gameState, bestState});

	return bestState;
}

int main() {
	std::ifstream iFile("blueprints.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	std::vector<Blueprint> blueprints;

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) continue;

		blueprints.push_back(ParseBlueprint(buffer));
		if (blueprints.size() == 3) break;
	}

	iFile.close();

	std::cout<<"Imported "<<blueprints.size()<<" blueprints."<<std::endl;

	static GameState initialState = {
			1, 0, 0, 0, 0, 0, 0, 0, TIME_TO_WORK
	};

	int geodeMultiplier = 1;

	for (int i = 0; i < blueprints.size(); i++) {
		endStates.clear();
		std::cout<<"Testing blueprint "<<(i+1)<<"..."<<std::endl;
		GameState bestState = GetMaxGeodes(blueprints[i], initialState);
		geodeMultiplier *= bestState.numGeode;
		std::cout<<"Blueprint "<<(i+1)<<" yields "<<bestState.numGeode<<" geodes."<<std::endl;
		std::cout<<"Final state of blueprint:"<<std::endl;
		PrintState(bestState);
		std::cout<<std::endl;
	}

	std::cout << "Geode Multiplier: " << geodeMultiplier << std::endl;
	return 0;
}
