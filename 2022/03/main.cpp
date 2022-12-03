#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>

int itemToPriority(char item) {
	return (unsigned char)(item < 'a' ? item - 'A' + 27 : item - 'a' + 1);
}

int main() {
	std::ifstream iFile("rucksacks.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	int pocketPriorities = 0;
	int groupPriorities = 0;
	std::unordered_set<char> pocketContents;
	std::unordered_set<char> groupContents[3];

	int groupIndex = 0;

	while (std::getline(iFile, buffer)) {
		int bufferLength = buffer.size();
		if (bufferLength > 0 && buffer[bufferLength-1] == '\r')
			buffer.erase(bufferLength-1);

		if (buffer.empty()) continue;

		if (buffer.size() % 2 != 0) {
			std::cerr<<"Rupsack Contents Discrepancy!"<<std::endl;
		}

		int halfBufferSz = buffer.size() / 2;

		pocketContents.clear();

		int i;

		for (i = 0; i < halfBufferSz; i++)
			pocketContents.insert(buffer[i]);

		for (i = halfBufferSz; i < buffer.size(); i++)
			if (pocketContents.contains(buffer[i])) break;

		char item = buffer[i];
		int score = itemToPriority(item);
		std::cout<<"Found duplicate: "<<item<<", Score: "<<score<<std::endl;
		pocketPriorities += score;

		groupContents[groupIndex].clear();
		for (i = 0; i < buffer.size(); i++) {
			item = buffer[i];
			bool foundBadge = false;

			switch (groupIndex) {
				case 1:
					if (!groupContents[0].contains(item)) break;
				case 0:
					groupContents[groupIndex].insert(item);
					break;
				case 2:
					if (!groupContents[1].contains(item)) break;

					foundBadge = true;
					score = itemToPriority(item);
					std::cout<<"Found badge: "<<item<<", Score: "<<score<<std::endl;
					groupPriorities += score;
					break;
				default:
					std::cerr << "Unknown index: " << groupIndex << std::endl;
					break;
			}

			if (foundBadge) break;
		}

		groupIndex++;
		groupIndex %= 3;
	}

	std::cout << "Pocket priorities: " << pocketPriorities << std::endl;
	std::cout << "Group priorities: " << groupPriorities << std::endl;

	iFile.close();
	return 0;
}
