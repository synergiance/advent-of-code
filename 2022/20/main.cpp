#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>

#include "MixerHelper.h"

using namespace Syn;

const int GroveCoordinates[] = {
		1000,
		2000,
		3000
};

int main() {
	std::ifstream iFile("encrypted_coordinates.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	MixerHelper mixer;
	MixerLink *zeroLink = nullptr;

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) continue;

		int number = atoi(buffer.c_str());
		mixer.Add(number);
		if (number == 0) zeroLink = mixer.LastAdded();
	}

	iFile.close();

	std::cout<<"Loaded "<<mixer.Length()<<" numbers."<<std::endl;

	for (int i = 0; i < mixer.Length(); i++)
		mixer.MoveLink(mixer.GetOriginalLinkAt(i), (*mixer.GetOriginalLinkAt(i)).value);

	int coordinateSum = 0;
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
