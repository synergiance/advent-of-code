#include <iostream>
#include <fstream>
#include <string>
#include <Parser.h>
#include <Reader.h>

using namespace Syn;

struct Bounds {
	int low;
	int high;
};

bool checkFullyContained(Bounds &obj1, Bounds &obj2) {
	return (obj1.low >= obj2.low && obj1.high <= obj2.high)
	    || (obj2.low >= obj1.low && obj2.high <= obj1.high);
}

bool checkPartiallyContained(Bounds &obj1, Bounds &obj2) {
	return (obj1.high >= obj2.low && obj2.high >= obj1.high)
	    || (obj2.high >= obj1.low && obj1.high >= obj2.high);
}

Bounds getBounds(const std::string& range) {
	std::vector<std::string> numStrings;
	Parser::Tokenize(range, numStrings, '-');

	Bounds bounds{};
	bounds.low = atoi(numStrings[0].c_str());
	bounds.high = atoi(numStrings[1].c_str());
	return bounds;
}

int main() {
	std::ifstream iFile("cleanup_assignments.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	int numFullyContainedAssignments = 0;
	int numPartiallyContainedAssignments = 0;

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) continue;

		std::vector<std::string> rangeStrings;
		Parser::Tokenize(buffer, rangeStrings, ',');

		std::vector<Bounds> rangeBounds;
		for (const auto& rangeString : rangeStrings)
			rangeBounds.push_back(getBounds(rangeString));

		if (checkFullyContained(rangeBounds[0], rangeBounds[1])) {
			numFullyContainedAssignments++;
			numPartiallyContainedAssignments++;
		} else if (checkPartiallyContained(rangeBounds[0], rangeBounds[1])) {
			numPartiallyContainedAssignments++;
		}
	}

	std::cout<<"Number of fully overlapping assignments: "<<numFullyContainedAssignments<<std::endl;
	std::cout<<"Number of partially overlapping assignments: "<<numPartiallyContainedAssignments<<std::endl;

	iFile.close();
	return 0;
}
