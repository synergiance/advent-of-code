#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>
#include <Coordinate.h>
#include <unordered_set>

using namespace Syn;

#define NUM_KNOTS 10

int main() {
	std::ifstream iFile("head_motions.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	Coordinate knotLocations[NUM_KNOTS];
	for (auto & knotLocation : knotLocations)
		knotLocation = {0,0};

	std::unordered_set<Coordinate> visitedLocations;
	visitedLocations.insert({0,0});

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) continue;

		std::vector<std::string> tokens;
		Parser::Tokenize(buffer, tokens);
		Coordinate direction{0,0};

		switch (tokens[0][0]) {
			case 'U':
			case 'u':
				direction.y = 1;
				break;
			case 'D':
			case 'd':
				direction.y = -1;
				break;
			case 'L':
			case 'l':
				direction.x = -1;
				break;
			case 'R':
			case 'r':
				direction.x = 1;
				break;
			default:
				break;
		}

		int distance = atoi(tokens[1].c_str());
		for (int i = 0; i < distance; i++) {
			Coordinate lastKnotLocation = knotLocations[0];
			knotLocations[0] += direction;
			Coordinate lastKnotDirection = direction;
			bool tailMoved = true;

			for (int j = 1; j < NUM_KNOTS; j++) {
				Coordinate offset = knotLocations[j-1] - knotLocations[j];

				if (offset.Magnitude() <= 1) {
					tailMoved = false;
					break;
				}

				Coordinate stashedKnotLocation = lastKnotLocation;
				lastKnotLocation = knotLocations[j];

				if (lastKnotDirection.isDiagonal()) {
					knotLocations[j] += lastKnotDirection * offset.Flattened().Absolute();
				} else {
					knotLocations[j] = stashedKnotLocation;
				}

				lastKnotDirection = knotLocations[j] - lastKnotLocation;
			}

			if (!tailMoved) continue;
			visitedLocations.insert(knotLocations[NUM_KNOTS-1]);
		}
	}

	std::cout<<"Locations visited: "<<visitedLocations.size()<<std::endl;

	iFile.close();
	return 0;
}
