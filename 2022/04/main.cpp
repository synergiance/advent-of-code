#include <iostream>
#include <fstream>
#include <string>

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
	size_t dashPos = range.find('-');
	size_t begin2 = dashPos + 1;

	Bounds bounds{};
	bounds.low = atoi(range.substr(0, dashPos).c_str());
	bounds.high = atoi(range.substr(begin2, range.size() - begin2).c_str());
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

	while (std::getline(iFile, buffer)) {
		size_t bufferLength = buffer.size();
		if (bufferLength > 0 && buffer[bufferLength - 1] == '\r')
			buffer.erase(bufferLength - 1);

		if (buffer.empty()) continue;

		size_t commaPos = buffer.find(',');

		size_t range1Begin = 0;
		size_t range2Begin = commaPos + 1;

		Bounds range1Bounds = getBounds(buffer.substr(range1Begin, commaPos));
		Bounds range2Bounds = getBounds(buffer.substr(range2Begin, buffer.length() - range2Begin));

		if (checkFullyContained(range1Bounds, range2Bounds)) {
			numFullyContainedAssignments++;
			numPartiallyContainedAssignments++;
		} else if (checkPartiallyContained(range1Bounds, range2Bounds)) {
			numPartiallyContainedAssignments++;
		}
	}

	std::cout<<"Number of fully overlapping assignments: "<<numFullyContainedAssignments<<std::endl;
	std::cout<<"Number of partially overlapping assignments: "<<numPartiallyContainedAssignments<<std::endl;

	iFile.close();
	return 0;
}
