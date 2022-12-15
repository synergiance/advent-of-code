#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>
#include "Grid.h"

using namespace Syn;

const char gSensorChar = 'S';
const char gBeaconChar = 'B';
const char gAirChar = ' ';
const char gCoverChar = '#';

const int gBeaconRow = 2000000;

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

	int minimum = 0xFFFFFFF;
	int maximum = -0xFFFFFFF;

	UnboundedGrid<char> scan;
	scan.Fill(gAirChar);

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

		int distanceFromRow = abs(sensor.location.y - gBeaconRow);

		std::cout<<"Sensor located at x="<<sensor.location.x<<", y="<<sensor.location.y<<std::endl;
		std::cout<<"Sensor's beacon at x="<<sensor.closestBeacon.x<<", y="<<sensor.closestBeacon.y<<std::endl;
		std::cout<<"Distance from sensor to beacon: "<<sensor.distance<<std::endl;
		std::cout<<"Sensor is "<<distanceFromRow<<" units from scanned row."<<std::endl;
		std::cout<<"Sensor "<<(distanceFromRow <= sensor.distance ? "touches" : "does not touch")<<" the scanned row"<<std::endl<<std::endl;

		if (sensor.location.x - sensor.distance < minimum) minimum = sensor.location.x - sensor.distance;
		if (sensor.location.x + sensor.distance > maximum) maximum = sensor.location.x + sensor.distance;

		if (sensor.distance >= distanceFromRow) sensors.push_back(sensor);
	}

	int numCoveredSpots = 0;

	for (int x = minimum; x <= maximum; x++) {
		Coordinate currentCoordinate = {x, gBeaconRow};
		for (Sensor sensor: sensors) {
			if (GetManhattanDistance(currentCoordinate, sensor.location) > sensor.distance) continue;
			if (currentCoordinate != sensor.closestBeacon) numCoveredSpots++;
			break;
		}
	}

	std::cout<<"Number of sensors touching row: "<<sensors.size()<<std::endl;
	std::cout<<"Minimum Range: "<<minimum<<", Maximum Range: "<<maximum<<std::endl;
	std::cout<<"Number of spots that cannot contain a beacon: "<<numCoveredSpots<<std::endl;

	iFile.close();
	return 0;
}
