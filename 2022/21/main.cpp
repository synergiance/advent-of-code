#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>
#include <unordered_map>

using namespace Syn;

enum MathOp {
	none, add, subtract, multiply, divide
};

struct Monkey {
	std::string name;
	MathOp op;
	long num;
	std::string lhs;
	std::string rhs;
};

Monkey ParseMonkey(const std::string &input) {
	Monkey monkey{};
	std::vector<std::string> tokens;
	Parser::Tokenize(input, tokens);
	monkey.name = tokens[0].substr(0, 4);

	if (tokens.size() == 2) {
		monkey.num = atoi(tokens[1].c_str());
		monkey.op = none;
	} else {
		monkey.lhs = tokens[1];
		monkey.rhs = tokens[3];
		switch (tokens[2][0]) {
			case '+':
				monkey.op = add;
				break;
			case '-':
				monkey.op = subtract;
				break;
			case '*':
				monkey.op = multiply;
				break;
			case '/':
				monkey.op = divide;
				break;
			default:
				monkey.op = none;
				break;
		}
		monkey.num = 0;
	}
	return monkey;
}

long GetVal(const std::string &name, std::unordered_map<std::string, Monkey> &monkeys) {
	Monkey &currentMonkey = monkeys[name];
	if (currentMonkey.op == none)
		return currentMonkey.num;

	long lhs = GetVal(currentMonkey.lhs, monkeys);
	long rhs = GetVal(currentMonkey.rhs, monkeys);

	switch (currentMonkey.op) {
		case add:
			currentMonkey.num = lhs + rhs;
			break;
		case subtract:
			currentMonkey.num = lhs - rhs;
			break;
		case multiply:
			currentMonkey.num = lhs * rhs;
			break;
		case divide:
			currentMonkey.num = lhs / rhs;
			break;
		default:
			currentMonkey.num = 0;
			break;
	}

	currentMonkey.op = none;
	return currentMonkey.num;
}

int main() {
	std::ifstream iFile("monkeys.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	std::unordered_map<std::string, Monkey> monkeys;

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) continue;

		Monkey newMonkey = ParseMonkey(buffer);
		monkeys.insert({newMonkey.name, newMonkey});
	}

	std::cout<<"Root's value: "<<GetVal("root", monkeys)<<std::endl;

	iFile.close();
	return 0;
}
