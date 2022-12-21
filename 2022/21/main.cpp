#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>
#include <unordered_map>

using namespace Syn;

enum MathOp {
	none, add, subtract, multiply, divide, equals
};

struct Monkey {
	std::string name;
	MathOp op;
	long num;
	std::string lhs;
	std::string rhs;
};

const std::string finalMonkey = "root";
const std::string me = "humn";

Monkey ParseMonkey(const std::string &input) {
	Monkey monkey{};
	std::vector<std::string> tokens;
	Parser::Tokenize(input, tokens);
	monkey.name = tokens[0].substr(0, 4);

	if (monkey.name == me) {
		monkey.num = -1;
		monkey.op = none;
	} else if (monkey.name == finalMonkey) {
		monkey.lhs = tokens[1];
		monkey.rhs = tokens[3];
		monkey.op = equals;
		monkey.num = 0;
	} else if (tokens.size() == 2) {
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

	if (lhs == -1 || rhs == -1) return -1;

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

long GetEqualValue(long lhs, long rhs, long product, MathOp op) {
	if (lhs < 0) {
		switch (op) {
			case add:
				return product - rhs;
			case subtract:
				return product + rhs;
			case multiply:
				return product / rhs;
			case divide:
				return product * rhs;
		}
	} else if (rhs < 0) {
		switch (op) {
			case add:
				return product - lhs;
			case subtract:
				return lhs - product;
			case multiply:
				return product / lhs;
			case divide:
				return lhs / product;
		}
	}
	return -1;
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

	Monkey currentMonkey = monkeys[finalMonkey];
	long product = GetVal(currentMonkey.lhs, monkeys);
	std::string nextMonkey = currentMonkey.rhs;
	if (product < 0) {
		product = GetVal(currentMonkey.rhs, monkeys);
		nextMonkey = currentMonkey.lhs;
	}

	while (nextMonkey != me) {
		currentMonkey = monkeys[nextMonkey];

		long lhs = GetVal(currentMonkey.lhs, monkeys);
		long rhs = GetVal(currentMonkey.rhs, monkeys);

		if (lhs < 0) {
			nextMonkey = currentMonkey.lhs;
		} else if (rhs < 0) {
			nextMonkey = currentMonkey.rhs;
		} else {
			std::cout<<"Error: could not get monkey tree!"<<std::endl;
			break;
		}
		product = GetEqualValue(lhs, rhs, product, currentMonkey.op);
	}

	std::cout<<"My value: "<<product<<std::endl;

	iFile.close();
	return 0;
}
