#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>
#include "Grid.h"

using namespace Syn;

const int gRange = 4000000;

int GetNumFromStr(const char* str, size_t len = 0x20) {
	size_t i;
	for (i = 0; i < len; i++)
		if (std::isdigit(str[i]) || str[i] == '-') break;
	return atoi(str + i);
}

int GetManhattanDistance(Coordinate c1, Coordinate c2) {
	int xDist = abs(c1.x - c2.x);
	int yDist = abs(c1.y - c2.y);
	return xDist + yDist;
}

struct Sensor {
	Coordinate location;
	Coordinate closestBeacon;
	int distance;
};

int main() {
	std::ifstream iFile("sensor_beacons.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	std::vector<Sensor> sensors;

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) continue;

		std::vector<std::string> tokens;
		Parser::Tokenize(buffer, tokens);

		Sensor sensor{};

		sensor.location.x = GetNumFromStr(tokens[2].c_str(), tokens[2].size());
		sensor.location.y = GetNumFromStr(tokens[3].c_str(), tokens[3].size());
		sensor.closestBeacon.x = GetNumFromStr(tokens[8].c_str(), tokens[8].size());
		sensor.closestBeacon.y = GetNumFromStr(tokens[9].c_str(), tokens[9].size());

		sensor.distance = GetManhattanDistance(sensor.location, sensor.closestBeacon);

		std::cout<<"Sensor located at x="<<sensor.location.x<<", y="<<sensor.location.y<<std::endl;
		std::cout<<"Sensor's beacon at x="<<sensor.closestBeacon.x<<", y="<<sensor.closestBeacon.y<<std::endl;
		std::cout<<"Distance from sensor to beacon: "<<sensor.distance<<std::endl<<std::endl;

		sensors.push_back(sensor);
	}

	std::cout<<"Number of sensors: "<<sensors.size()<<std::endl;

	Coordinate foundCoordinate = {-1, -1};

	bool isOccupied = true;
	for (int y = 0; y < gRange && isOccupied; y++) {
		for (int x = 0; x <= gRange && isOccupied; x++) {
			Coordinate currentCoordinate = {x, y};
			isOccupied = false;
			for (Sensor sensor: sensors) {
				if (GetManhattanDistance(currentCoordinate, sensor.location) > sensor.distance) continue;
				x = sensor.location.x + sensor.distance - abs(y - sensor.location.y);
				isOccupied = true;
				break;
			}
			if (!isOccupied) foundCoordinate = currentCoordinate;
		}
	}

	if (foundCoordinate == Coordinate{-1, -1}) {
		std::cout<<"Could not find!"<<std::endl;
	} else {
		std::cout<<"Found location: x="<<foundCoordinate.x<<", y="<<foundCoordinate.y<<std::endl;
		std::cout<<"Tuning frequency: "<<((long long)foundCoordinate.x * (long long)gRange + (long long)foundCoordinate.y)<<std::endl;
	}

	iFile.close();
	return 0;
}
