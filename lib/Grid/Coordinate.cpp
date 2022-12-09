//
// Created by Synergiance on 12/9/22.
//

#include "Coordinate.h"

namespace Syn {
	bool Coordinate::isDiagonal() const {
		return x != 0 && y != 0;
	}

	int Coordinate::Distance(const Coordinate &a, const Coordinate &b) {
		int x = a.x - b.x;
		int y = a.y - b.y;
		if (x < 0) x *= -1;
		if (y < 0) y *= -1;
		return x < y ? y : x;
	}

	int Coordinate::Magnitude() const {
		return Distance(*this, {0,0});
	}

	Coordinate Coordinate::Flattened() const {
		Coordinate flattenedCoordinate = *this;
		if (x < -1) flattenedCoordinate.x = -1;
		if (x > 1) flattenedCoordinate.x = 1;
		if (y < -1) flattenedCoordinate.y = -1;
		if (y > 1) flattenedCoordinate.y = 1;
		return flattenedCoordinate;
	}

	Coordinate Coordinate::Absolute() const {
		Coordinate absoluteCoordinate = *this;
		if (x < 0) absoluteCoordinate.x *= -1;
		if (y < 0) absoluteCoordinate.y *= -1;
		return absoluteCoordinate;
	}

	size_t Coordinate::operator()(const Coordinate &coordinate) const noexcept {
		return (coordinate.x << 16) + coordinate.y;
	}

	bool Coordinate::operator==(const Coordinate &other) const {
		return x == other.x && y == other.y;
	}

	Coordinate &Coordinate::operator+=(const Coordinate &other) {
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	Coordinate Coordinate::operator+(const Coordinate &other) const {
		Coordinate newCoordinate = *this;
		newCoordinate += other;
		return newCoordinate;
	}

	Coordinate &Coordinate::operator-=(const Coordinate &other) {
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}

	Coordinate Coordinate::operator-(const Coordinate &other) const {
		Coordinate newCoordinate = *this;
		newCoordinate -= other;
		return newCoordinate;
	}

	Coordinate &Coordinate::operator*=(const Coordinate &other) {
		this->x *= other.x;
		this->y *= other.y;
		return *this;
	}

	Coordinate Coordinate::operator*(const Coordinate &other) const {
		Coordinate newCoordinate = *this;
		newCoordinate *= other;
		return newCoordinate;
	}
} // Syn

size_t std::hash<Syn::Coordinate>::operator()(const Syn::Coordinate &coordinate) const noexcept {
	return coordinate(coordinate);
}