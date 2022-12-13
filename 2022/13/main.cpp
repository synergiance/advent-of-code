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

int Compare(ListItem left, ListItem right) {
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

void SwapItems(std::vector<ListItem> &items, int leftPos) {
	ListItem item = items[leftPos];
	items.erase(items.begin() + leftPos);
	items.insert(items.begin() + leftPos + 1, item);
}

void CompareAndSwap(std::vector<ListItem> &items, int leftPos) {
	if (Compare(items[leftPos], items[leftPos + 1]) >= 0) return;
	std::cout<<"Swapping "<<(leftPos+1)<<" with "<<(leftPos+2)<<std::endl;
	SwapItems(items, leftPos);
	if (Compare(items[leftPos], items[leftPos + 1]) < 0)
		std::cerr<<"Warning: Something went wrong with the swap!";
}

void PrintItem(ListItem &item) {
	if (item.value != -1) {
		std::cout<<item.value;
		return;
	}

	std::cout<<"[";
	if (item.list.size() > 0) PrintItem(item.list[0]);
	for (int i = 1; i < item.list.size(); i++) {
		std::cout<<",";
		PrintItem(item.list[i]);
	}
	std::cout<<"]";
}

int main() {
	std::ifstream iFile("distress_call.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	std::vector<ListItem> items;

	while (iFile) {
		buffer.clear();
		Reader::getline(iFile, buffer);
		if (buffer.empty()) continue;

		ListItem newItem{};
		ReadArray(buffer, newItem);
		items.push_back(newItem);
	}

	for (int i = items.size() - 1; i > 0; i--)
		for (int j = 0; j < i; j++)
			CompareAndSwap(items, j);


	ListItem divider2{};
	ListItem divider6{};
	ReadArray("[[2]]", divider2);
	ReadArray("[[6]]", divider6);
	int divider2Pos;
	int divider6Pos;

	items.push_back(divider2);

	for (divider2Pos = items.size() - 1; divider2Pos > 0; divider2Pos--) {
		if (Compare(items[divider2Pos - 1], items[divider2Pos]) >= 0) break;
		SwapItems(items, divider2Pos - 1);
	}

	items.push_back(divider6);

	for (divider6Pos = items.size() - 1; divider6Pos > 0; divider6Pos--) {
		if (Compare(items[divider6Pos - 1], items[divider6Pos]) >= 0) break;
		SwapItems(items, divider6Pos - 1);
	}

	std::cout<<"Decoder Key: "<<(++divider2Pos * ++divider6Pos)<<std::endl;

	iFile.close();
	return 0;
}
