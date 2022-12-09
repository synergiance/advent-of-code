#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>
#include <unordered_set>

using namespace Syn;

#define NUM_KNOTS 10

struct Coordinate {
	int x;
	int y;

	[[nodiscard]] bool isDiagonal() const {
		return x != 0 && y != 0;
	}

	size_t operator()(const Coordinate &coordinate) const noexcept {
		return (coordinate.x << 16) + coordinate.y;
	}

	bool operator==(const Coordinate &other) const {
		return x == other.x && y == other.y;
	}

	Coordinate &operator+=(const Coordinate &other) {
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	Coordinate operator+(const Coordinate &other) const {
		Coordinate newCoordinate = *this;
		newCoordinate += other;
		return newCoordinate;
	}

	Coordinate &operator-=(const Coordinate &other) {
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}

	Coordinate operator-(const Coordinate &other) const {
		Coordinate newCoordinate = *this;
		newCoordinate -= other;
		return newCoordinate;
	}

	Coordinate &operator*=(const Coordinate &other) {
		this->x *= other.x;
		this->y *= other.y;
		return *this;
	}

	Coordinate operator*(const Coordinate &other) const {
		Coordinate newCoordinate = *this;
		newCoordinate *= other;
		return newCoordinate;
	}

	static int Distance(const Coordinate &a, const Coordinate &b) {
		int x = a.x - b.x;
		int y = a.y - b.y;
		if (x < 0) x *= -1;
		if (y < 0) y *= -1;
		return x < y ? y : x;
	}

	[[nodiscard]] int Magnitude() const {
		return Distance(*this, {0,0});
	}

	[[nodiscard]] Coordinate Flattened() const {
		Coordinate flattenedCoordinate = *this;
		if (x < -1) flattenedCoordinate.x = -1;
		if (x > 1) flattenedCoordinate.x = 1;
		if (y < -1) flattenedCoordinate.y = -1;
		if (y > 1) flattenedCoordinate.y = 1;
		return flattenedCoordinate;
	}

	[[nodiscard]] Coordinate Absolute() const {
		Coordinate absoluteCoordinate = *this;
		if (x < 0) absoluteCoordinate.x *= -1;
		if (y < 0) absoluteCoordinate.y *= -1;
		return absoluteCoordinate;
	}
};

namespace std {
	template<> struct hash<Coordinate> {
		size_t operator()(const Coordinate &coordinate) const noexcept {
			return coordinate(coordinate);
		}
	};
}

/*
class Array2D {
private:
	int width;
	int height;
	int *data;
public:
	Array2D(int width, int height) {
		this->width = width;
		this->height = height;
		data = new int[width * height];
	}

	~Array2D() {
		delete [] data;
	}

	int &operator[](Coordinate location) {
		return data[location.x + location.y * width];
	}
};
*/

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
