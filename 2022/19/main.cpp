#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>

using namespace Syn;

#define TIME_TO_WORK 24

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
};

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
}

void PrintState(GameState &gameState) {
	std::cout<<"Ore: ("<<gameState.numOre<<", "<<gameState.numOreRobots<<"), Clay: ("<<gameState.numClay<<", ";
	std::cout<<gameState.numClayRobots<<"), Obsidian: ("<<gameState.numObsidian<<", "<<gameState.numObsidianRobots;
	std::cout<<"), Geodes: ("<<gameState.numGeode<<", "<<gameState.numGeodeRobots<<")"<<std::endl;
}

GameState GetMaxGeodes(const Blueprint &blueprint, int timeLeft, GameState gameState) {
	if (timeLeft == 0) {
		return gameState;
	}

	GameState newGameState = gameState;
	GameState bestState = {
			0,0,0,0,0,0,0,0
	};

	bool canDoEverything = true;
	bool noMoreTests = false;

	if (gameState.numOre >= blueprint.geodeRobotCostOre && gameState.numObsidian >= blueprint.geodeRobotCostObsidian) {
		noMoreTests = true;
		newGameState.numOre -= blueprint.geodeRobotCostOre;
		newGameState.numObsidian -= blueprint.geodeRobotCostObsidian;
		ApplyGameState(newGameState);
		newGameState.numGeodeRobots++;
		GameState potentialState = GetMaxGeodes(blueprint, timeLeft - 1, newGameState);
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
			GameState potentialState = GetMaxGeodes(blueprint, timeLeft - 1, newGameState);
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
			GameState potentialState = GetMaxGeodes(blueprint, timeLeft - 1, newGameState);
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
			GameState potentialState = GetMaxGeodes(blueprint, timeLeft - 1, newGameState);
			if (potentialState.numGeode > bestState.numGeode) bestState = potentialState;
			newGameState = gameState;
		}
	} else {
		canDoEverything = false;
	}

	if (!canDoEverything) {
		ApplyGameState(newGameState);
		GameState potentialState = GetMaxGeodes(blueprint, timeLeft - 1, newGameState);
		if (potentialState.numGeode > bestState.numGeode) bestState = potentialState;
	}

	return bestState;
}

int main() {
	std::ifstream iFile("test.dat");
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
	}

	std::cout<<"Imported "<<blueprints.size()<<" blueprints."<<std::endl;

	static GameState initialState = {
			1, 0, 0, 0, 0, 0, 0, 0
	};

	int totalQuality = 0;

	for (int i = 0; i < blueprints.size(); i++) {
		std::cout<<"Testing blueprint "<<(i+1)<<"..."<<std::endl;
		GameState bestState = GetMaxGeodes(blueprints[i], TIME_TO_WORK, initialState);
		int blueprintQuality = bestState.numGeode * (i + 1);
		totalQuality += blueprintQuality;
		std::cout<<"Blueprint "<<(i+1)<<" yields "<<bestState.numGeode<<" geodes with quality level "<< blueprintQuality<<std::endl;
		std::cout<<"Final state of blueprint:"<<std::endl;
		PrintState(bestState);
		std::cout<<std::endl;
	}

	std::cout<<"Total quality level: "<<totalQuality<<std::endl;

	iFile.close();
	return 0;
}
