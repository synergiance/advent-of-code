#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>
#include <Grid.h>

using namespace Syn;

const long NumRocksFalling = 1000000000000;
const int MaxRockDimension = 4;
const int RoomWidth = 7;
const int MaxRoomHeight = 0x4000000;

const char RockChar = '#';
const char AirChar = '.';

const Coordinate DownDirection = {0, -1};
const Coordinate LeftDirection = {-1, 0};
const Coordinate RightDirection = {1, 0};

const Coordinate RockOffset = {2, 4};

bool CheckCollision(Coordinate coordinate, Grid<bool> &currentRock, Grid<bool> &room) {
	for (int x = 0; x < MaxRockDimension; x++) {
		for (int y = 0; y < MaxRockDimension; y++) {
			if (!currentRock[{x, y}]) continue;
			Coordinate globalCoordinate = coordinate + Coordinate{x, y};
			if (globalCoordinate.x >= RoomWidth) return true;
			if (room[globalCoordinate]) return true;
		}
	}
	return false;
}

bool AttemptMove(Coordinate &coordinate, Coordinate direction, Grid<bool> &currentRock, Grid<bool> &room) {
	Coordinate possibleCoordinate = coordinate + direction;
	if (possibleCoordinate.y < 0) return false;
	if (possibleCoordinate.x < 0) return false;
	if (CheckCollision(possibleCoordinate, currentRock, room)) return false;
	coordinate = possibleCoordinate;
	return true;
}

void ApplyStone(Coordinate &coordinate, Grid<bool> &currentRock, Grid<bool> &room) {
	for (int x = 0; x < MaxRockDimension; x++)
		for (int y = 0; y < MaxRockDimension; y++)
			room[coordinate + Coordinate{x, y}] |= currentRock[{x, y}];
}

int GetLowestColumnHeight(Grid<bool> &room) {
	int lowest = MaxRoomHeight;
	for (int x = 0; x < RoomWidth; x++) {
		for (int y = MaxRoomHeight - 1; y >= 0; y--) {
			if (!room[{x, y}]) continue;
			if (y + 1 < lowest) lowest = y + 1;
			break;
		}
	}
	return lowest;
}

int main() {
	std::ifstream rockFile("rock_formations.dat");
	if (!rockFile.is_open()) {
		std::cout<<"Could not open rock file!"<<std::endl;
		return 1;
	}

	std::cout<<"Rock file successfully opened!"<<std::endl;
	std::vector<std::string> buffer;

	std::vector<Grid<bool>> rockFormations;

	while (rockFile) {
		buffer.clear();
		Reader::ReadUntilEmptyLn(rockFile, buffer);
		if (buffer.empty()) continue;

		rockFormations.emplace_back(MaxRockDimension, MaxRockDimension);
		rockFormations.back().Fill(false);

		for (int i = 0; i < buffer.size() && i < MaxRockDimension; i++) {
			int ln = (int)buffer.size() - i - 1;
			for (int j = 0; j < MaxRockDimension && j < buffer[ln].size(); j++) {
				if (buffer[ln][j] == RockChar)
					rockFormations.back()[{j, i}] = true;
			}
		}
	}

	std::cout<<"Imported all rocks"<<std::endl;
	rockFile.close();

	std::ifstream iFile("wind_pattern.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open wind file!"<<std::endl;
		return 1;
	}

	std::cout<<"Wind file successfully opened!"<<std::endl;

	std::string windStr;
	Reader::getline(iFile, windStr);
	iFile.close();

	long totalRoomHeight = NumRocksFalling * MaxRockDimension;
	long roomOffset = 0;
	int roomHeight = (totalRoomHeight > (long)MaxRoomHeight) ? MaxRoomHeight : (int)totalRoomHeight;

	Grid<bool> room(RoomWidth, roomHeight);
	int highestRock = -1;
	int currentWindIdx = 0;

	for (long i = 0; i < NumRocksFalling; i++) {
		Grid<bool> &currentRock = rockFormations[i % rockFormations.size()];
		Coordinate rockLocation = Coordinate{0, highestRock} + RockOffset;

		do {
			Coordinate windDirection = windStr[currentWindIdx++] == '<' ? LeftDirection : RightDirection;
			AttemptMove(rockLocation, windDirection, currentRock, room);
			currentWindIdx %= (int)windStr.size();
		} while (AttemptMove(rockLocation, DownDirection, currentRock, room));

		ApplyStone(rockLocation, currentRock, room);

		for (int j = highestRock + 1; j < rockLocation.y + MaxRockDimension + 1; j++) {
			bool hasRock = false;
			for (int k = 0; k < RoomWidth; k++) {
				if (!room[{k,j}]) continue;
				hasRock = true;
				break;
			}

			if (hasRock) continue;

			highestRock = j - 1;
			break;
		}
	}

	std::cout<<"Highest Rock: "<<(highestRock + 1)<<std::endl;

	return 0;
}
