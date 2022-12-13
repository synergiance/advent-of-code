#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>

using namespace Syn;

struct ListItem {
	int value;
	std::vector<ListItem> list;
};

void ConvertToList(ListItem &item) {
	item.list.push_back(ListItem{item.value});
	item.value = -1;
}

int Compare(ListItem &left, ListItem &right) {
	if ((left.value == -1) != (right.value == -1)) {
		if (left.value != -1) ConvertToList(left);
		if (right.value != -1) ConvertToList(right);
	}

	if (left.value != -1) {
		if (left.value == right.value) return 0;
		return left.value < right.value ? 1 : -1;
	}

	int idx = 0;
	while (true) {
		bool ranOutRight = idx >= right.list.size();
		bool ranOutLeft = idx >= left.list.size();

		if (ranOutLeft && ranOutRight) return 0;
		if (ranOutLeft) return 1;
		if (ranOutRight) return -1;

		int ret = Compare(left.list[idx], right.list[idx]);
		if (ret != 0) return ret;

		idx++;
	}
}

int GetClosingBracket(const std::string &str) {
	int openingBracketCount = 0;
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == '[') {
			openingBracketCount++;
		} else if (str[i] == ']') {
			openingBracketCount--;
			if (openingBracketCount == 0) return i;
		}
	}
	return -1;
}

int GetNumLength(const std::string &str) {
	for (int i = 0; i < str.size(); i++)
		if (!std::isdigit(str[i])) return i;
	return str.size();
}

void ReadArray(std::string input, ListItem &item) {
	std::cout<<"Reading array: \""<<input<<"\""<<std::endl;
	if (input[0] != '[' || input[input.size()-1] != ']') return;

	input.erase(input.cbegin());
	input.pop_back();
	std::vector<std::string> numbers;
	item.value = -1;

	while (!input.empty()) {
		if (input[0] == '[') {
			int nextBracket = GetClosingBracket(input);
			ListItem newItem{};
			ReadArray(input.substr(0, nextBracket + 1), newItem);
			item.list.push_back(newItem);

			if (input.size() > nextBracket + 2)
				input = input.substr(nextBracket + 2);
			else input = "";
		} else {
			int numLen = GetNumLength(input);
			std::string numStr = input.substr(0, numLen);
			std::cout<<"Reading number: "<<numStr<<std::endl;
			item.list.push_back({atoi(numStr.c_str())});

			if (input.size() > numLen + 1)
				input = input.substr(numLen + 1);
			else input = "";
		}
	}
}

int main() {
	std::ifstream iFile("distress_call.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::vector<std::string> buffer;

	int total = 0;

	int idx = 1;
	while (iFile) {
		buffer.clear();
		Reader::ReadUntilEmptyLn(iFile, buffer);

		ListItem leftItem{};
		ListItem rightItem{};

		ReadArray(buffer[0], leftItem);
		ReadArray(buffer[1], rightItem);

		if (Compare(leftItem, rightItem) >= 0) total += idx;

		idx++;
	}

	std::cout<<"Total: "<<total<<std::endl;

	iFile.close();
	return 0;
}
