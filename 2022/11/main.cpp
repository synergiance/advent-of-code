#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>

using namespace Syn;

struct Operation {
	int operand;
	bool multiply;
};

struct Monkey {
	std::vector<long long> items;
	Operation operation;
	int test;
	int trueMonkey;
	int falseMonkey;
	int timesInspected;
};

const int NumRounds = 10000;

Monkey ParseMonkey(const std::vector<std::string>& monkeyDefinition) {
	if (monkeyDefinition.size() != 6) {
		std::cerr<<"Monkey definition malformed!"<<monkeyDefinition.size()<<std::endl;
		return Monkey{};
	}

	Monkey newMonkey{};

	std::vector<std::string> tokens;
	Parser::TrimAndTokenize(monkeyDefinition[1], tokens);
	tokens.erase(tokens.cbegin());
	tokens.erase(tokens.cbegin());
	for (std::string &itemToken : tokens) {
		Parser::Trim(itemToken, ',');
		newMonkey.items.push_back(atoi(itemToken.c_str()));
	}

	tokens.clear();
	Parser::TrimAndTokenize(monkeyDefinition[2], tokens);
	newMonkey.operation.multiply = tokens[4] == "*";
	newMonkey.operation.operand = tokens[5] == "old" ? -1 : atoi(tokens[5].c_str());

	tokens.clear();
	Parser::TrimAndTokenize(monkeyDefinition[3], tokens);
	newMonkey.test = atoi(tokens[3].c_str());

	tokens.clear();
	Parser::TrimAndTokenize(monkeyDefinition[4], tokens);
	newMonkey.trueMonkey = atoi(tokens[5].c_str());

	tokens.clear();
	Parser::TrimAndTokenize(monkeyDefinition[5], tokens);
	newMonkey.falseMonkey = atoi(tokens[5].c_str());

	newMonkey.timesInspected = 0;

	return newMonkey;
}

void TakeTurn(int monkeyIdx, std::vector<Monkey> &monkeys, long long modNum) {
	Monkey &monkey = monkeys[monkeyIdx];
	while (!monkey.items.empty()) {
		long long item = monkey.items[0];
		monkey.items.erase(monkey.items.cbegin());

		long long operand = monkey.operation.operand == -1 ? item : monkey.operation.operand;

		if (monkey.operation.multiply)
			item *= operand;
		else
			item += operand;

		//item /= 3;
		item %= modNum;

		if (item % monkey.test == 0)
			monkeys[monkey.trueMonkey].items.push_back(item);
		else
			monkeys[monkey.falseMonkey].items.push_back(item);

		monkey.timesInspected++;
	}
}

int main() {
	std::ifstream iFile("monkeys.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::vector<std::string> buffer;

	std::vector<Monkey> monkeys;
	long long modNum = 1;

	while (true) {
		Reader::ReadUntilEmptyLn(iFile, buffer);
		if (buffer.empty()) break;

		monkeys.push_back(ParseMonkey(buffer));
		modNum *= monkeys.back().test;
		buffer.clear();
	}

	for (int i = 0; i < NumRounds; i++)
		for (int j = 0; j < monkeys.size(); j++)
			TakeTurn(j, monkeys, modNum);

	long long mostTimesInspected = 0;
	long long secondMostTimesInspected = 0;

	for (auto & monkey : monkeys) {
		if (monkey.timesInspected > mostTimesInspected) {
			secondMostTimesInspected = mostTimesInspected;
			mostTimesInspected = monkey.timesInspected;
		} else if (monkey.timesInspected > secondMostTimesInspected) {
			secondMostTimesInspected = monkey.timesInspected;
		}
	}

	long long monkeyBusiness = mostTimesInspected * secondMostTimesInspected;

	std::cout<<"Monkey business: "<<monkeyBusiness<<std::endl;

	iFile.close();
	return 0;
}
