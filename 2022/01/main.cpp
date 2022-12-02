#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct elfInventory {
	int totalCalories;
	std::vector<int> caloriesPerItem;
};

int main() {
	std::ifstream iFile("elf_calories.dat");
	std::string buf;
	std::vector<elfInventory> elfCalories;

	if (!iFile.is_open()) {
		std::cerr<<"File could not be opened!"<<std::endl;
		return 1;
	}

	int highest = 0;
	int highestIndex = 0;
	auto *inventory = new struct elfInventory;
	inventory->totalCalories = 0;
	inventory->caloriesPerItem.clear();

	while (std::getline(iFile, buf)) {
		int bufferLength = buf.size();
		if (bufferLength > 0 && buf[bufferLength-1] == '\r')
			buf.erase(bufferLength-1);

		if (buf.empty()) {
			std::cout<<"Buffer empty, switching to next elf"<<std::endl;
			if (inventory->totalCalories > highest) {
				highest = inventory->totalCalories;
				highestIndex = elfCalories.size();
			}

			elfCalories.push_back(*inventory);
			std::cout<<"Carrying calories: "<<inventory->totalCalories<<std::endl;
			inventory = new struct elfInventory;
			inventory->totalCalories = 0;
			inventory->caloriesPerItem.clear();

			continue;
		}

		std::cout<<"Buffer: "<<buf<<std::endl;

		int numCalories = atoi(buf.c_str());
		inventory->totalCalories += numCalories;
		inventory->caloriesPerItem.push_back(numCalories);
		std::cout<<"Calories: "<<numCalories<<std::endl;
	}

	std::cout<<elfCalories[highestIndex].totalCalories;
	return 0;
}
