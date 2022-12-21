#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>

#include "MixerHelper.h"

using namespace Syn;

struct MixerValue {
	int value;
	bool hasMixed;
};

const int GroveCoordinates[] = {
		1000,
		2000,
		3000
};

void RotateList(int distance, std::vector<MixerValue> &list) {
	int newDistance = distance % (int)list.size();
	if (newDistance < 0) newDistance += (int)list.size();

	for (int i = 0; i < newDistance; i++) {
		list.push_back(list.front());
		list.erase(list.cbegin());
	}
}

int main() {
	std::ifstream iFile("encrypted_coordinates.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	std::vector<MixerValue> numbers;
	MixerHelper mixer;
	MixerLink *zeroLink = nullptr;

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) continue;

		int number = atoi(buffer.c_str());
		numbers.push_back({number, false});
		mixer.Add(number);
		if (number == 0) zeroLink = mixer.LastAdded();
	}

	iFile.close();

	std::cout<<"Loaded "<<numbers.size()<<" numbers."<<std::endl;

	int currentPosition = 0;
	while (currentPosition < numbers.size()) {
		MixerValue currentVal = numbers[currentPosition];

		if (currentVal.hasMixed) {
			currentPosition++;
			continue;
		}

		currentVal.hasMixed = true;

		int newPosition = (currentVal.value + currentPosition) % (int)numbers.size();
		if (newPosition < 0) newPosition += numbers.size();

		int negativeOffset = currentVal.value > 0 ? 1 : 0;
		int oldPosition = currentPosition;

		if (currentVal.value == 0)
			numbers[currentPosition].hasMixed = true;
		if (newPosition != oldPosition)
			numbers.insert(numbers.cbegin() + newPosition + negativeOffset, currentVal);
		if (newPosition <= oldPosition)
			currentPosition++;
		if (newPosition != oldPosition)
			numbers.erase(numbers.cbegin() + currentPosition);
	}

	for (int i = 0; i < mixer.Length(); i++)
		mixer.MoveLink(mixer.GetOriginalLinkAt(i), (*mixer.GetOriginalLinkAt(i)).value);

	int zeroPos = -1;
	for (int i = 0; i < numbers.size(); i++) {
		if (numbers[i].value != 0) continue;
		zeroPos = i;
		break;
	}

	std::cout<<"Zero position: "<<zeroPos<<std::endl;

	int unmixedNumbers = 0;
	for (auto & number : numbers)
		if (!number.hasMixed) unmixedNumbers++;
	std::cout<<"Unmixed numbers found: "<<unmixedNumbers<<std::endl;

	int coordinateSum = 0;
	for (int position : GroveCoordinates) {
		int val = numbers[(position + zeroPos) % numbers.size()].value;
		std::cout<<"Value ("<<position<<"): "<<val<<std::endl;
		coordinateSum += val;
	}

	std::cout<<"Coordinate sum: "<<coordinateSum<<std::endl;

	coordinateSum = 0;
	int currentPosOffset = 0;
	MixerLink *currentLink = zeroLink;
	for (int positionOffset : GroveCoordinates) {
		currentLink = mixer.GetLinkOffset(currentLink, positionOffset - currentPosOffset);
		coordinateSum += currentLink->value;
		std::cout<<"Value ("<<positionOffset<<") (New method): "<<currentLink->value<<std::endl;
		currentPosOffset = positionOffset;
	}

	std::cout<<"Coordinate sum (New method): "<<coordinateSum<<std::endl;

	return 0;
}
