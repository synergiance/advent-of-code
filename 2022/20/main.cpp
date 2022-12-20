#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>
#include <unordered_set>

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

int main() {
	std::ifstream iFile("test.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	std::vector<MixerValue> numbers;

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) continue;

		int number = atoi(buffer.c_str());
		numbers.push_back({number, false});
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

		if (newPosition != oldPosition)
			numbers.insert(numbers.cbegin() + newPosition + negativeOffset, currentVal);
		if (newPosition <= oldPosition)
			currentPosition++;
		if (newPosition != oldPosition)
			numbers.erase(numbers.cbegin() + currentPosition);
	}

	int zeroPos = -1;
	for (int i = 0; i < numbers.size(); i++) {
		if (numbers[i].value != 0) continue;
		zeroPos = i;
		break;
	}

	std::cout<<"Zero position: "<<zeroPos<<std::endl;

	int coordinateSum = 0;
	for (int position : GroveCoordinates) {
		int val = numbers[(position + zeroPos) % numbers.size()].value;
		std::cout<<"Value ("<<position<<"): "<<val<<std::endl;
		coordinateSum += val;
	}

	std::cout<<"Coordinate sum: "<<coordinateSum<<std::endl;

	return 0;
}
