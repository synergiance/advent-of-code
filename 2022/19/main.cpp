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

int GetMaxGeodes(const Blueprint &blueprint, int timeLeft, GameState gameState) {
	std::cout<<"Minutes left: "<<timeLeft<<", Ore: ("<<gameState.numOre<<", "<<gameState.numOreRobots<<"), Clay: (";
	std::cout<<gameState.numClay<<", "<<gameState.numClayRobots<<"), Obsidian: ("<<gameState.numObsidian<<", ";
	std::cout<<gameState.numObsidianRobots<<"), Geodes: ("<<gameState.numGeode<<", "<<gameState.numGeodeRobots<<")"<<std::endl;

	if (timeLeft == 0) {
		return gameState.numGeode;
	}

	if (timeLeft == 1 && gameState.numGeodeRobots == 0)
		return 0;

	gameState.numOre += gameState.numOreRobots;
	gameState.numClay += gameState.numClayRobots;
	gameState.numObsidian += gameState.numObsidianRobots;
	gameState.numGeode += gameState.numGeodeRobots;

	GameState newGameState = gameState;
	int maxGeodes = 0;

	bool canDoEverything = true;

	if (gameState.numOre >= blueprint.oreRobotCost) {
		newGameState.numOre -= blueprint.oreRobotCost;
		newGameState.numOreRobots++;
		int newMax = GetMaxGeodes(blueprint, timeLeft - 1, newGameState);
		if (newMax > maxGeodes) maxGeodes = newMax;
		newGameState = gameState;
	} else {
		canDoEverything = false;
	}

	if (gameState.numOre >= blueprint.clayRobotCost) {
		newGameState.numOre -= blueprint.clayRobotCost;
		newGameState.numClayRobots++;
		int newMax = GetMaxGeodes(blueprint, timeLeft - 1, newGameState);
		if (newMax > maxGeodes) maxGeodes = newMax;
		newGameState = gameState;
	} else {
		canDoEverything = false;
	}

	if (gameState.numOre >= blueprint.obsidianRobotCostOre && gameState.numClay >= blueprint.obsidianRobotCostClay) {
		newGameState.numOre -= blueprint.obsidianRobotCostOre;
		newGameState.numClay -= blueprint.obsidianRobotCostClay;
		newGameState.numObsidianRobots++;
		int newMax = GetMaxGeodes(blueprint, timeLeft - 1, newGameState);
		if (newMax > maxGeodes) maxGeodes = newMax;
		newGameState = gameState;
	} else {
		canDoEverything = false;
	}

	if (gameState.numOre >= blueprint.geodeRobotCostOre && gameState.numObsidian >= blueprint.geodeRobotCostObsidian) {
		newGameState.numOre -= blueprint.geodeRobotCostOre;
		newGameState.numObsidian -= blueprint.geodeRobotCostObsidian;
		newGameState.numGeodeRobots++;
		int newMax = GetMaxGeodes(blueprint, timeLeft - 1, newGameState);
		if (newMax > maxGeodes) maxGeodes = newMax;
		newGameState = gameState;
	} else {
		canDoEverything = false;
	}

	if (!canDoEverything) {
		int newMax = GetMaxGeodes(blueprint, timeLeft - 1, newGameState);
		if (newMax > maxGeodes) maxGeodes = newMax;
	}

	return maxGeodes;
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
	}

	std::cout<<"Imported "<<blueprints.size()<<" blueprints."<<std::endl;

	static GameState initialState = {
			1, 0, 0, 0, 0, 0, 0, 0
	};

	int totalQuality = 0;

	for (int i = 0; i < blueprints.size(); i++) {
		std::cout<<"Testing blueprint "<<(i+1)<<"..."<<std::endl;
		int maxGeodes = GetMaxGeodes(blueprints[i], TIME_TO_WORK, initialState) * (i + 1);
		int blueprintQuality = maxGeodes * (i + 1);
		totalQuality += blueprintQuality;
		std::cout<<"Blueprint "<<(i+1)<<" yields "<<maxGeodes<<" geodes with quality level "<<blueprintQuality<<std::endl<<std::endl;
	}

	std::cout<<"Total quality level: "<<totalQuality<<std::endl;

	iFile.close();
	return 0;
}
