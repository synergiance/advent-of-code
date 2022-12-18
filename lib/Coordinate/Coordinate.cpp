//
// Created by Synergiance on 12/9/22.
//

#include "Coordinate.h"
#include <cstdlib>

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

	Coordinate Coordinate::Parse(const char *input, size_t len) {
		Coordinate newCoordinate{};
		int i;
		for (i = 0; i < len; i++)
			if (input[i] == ',') break;
		i++;
		newCoordinate.x = atoi(input);
		newCoordinate.y = atoi(input+i);
		return newCoordinate;
	}

	size_t Coordinate::operator()(const Coordinate &coordinate) const noexcept {
		return (coordinate.x << 16) + coordinate.y;
	}

	bool Coordinate::operator==(const Coordinate &other) const {
		return x == other.x && y == other.y;
	}

	bool Coordinate::operator!=(const Coordinate &other) const {
		return x != other.x || y != other.y;
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

	Coordinate &Coordinate::operator/=(const Coordinate &other) {
		this->x /= other.x;
		this->y /= other.y;
		return *this;
	}

	Coordinate Coordinate::operator/(const Coordinate &other) const {
		Coordinate newCoordinate = *this;
		newCoordinate /= other;
		return newCoordinate;
	}

	Coordinate &Coordinate::operator%=(const Coordinate &other) {
		this->x %= other.x;
		this->y %= other.y;
		return *this;
	}

	Coordinate Coordinate::operator%(const Coordinate &other) const {
		Coordinate newCoordinate = *this;
		newCoordinate %= other;
		return newCoordinate;
	}

	Coordinate3D Coordinate3D::Parse(const char *input, size_t len) {
		Coordinate3D newCoordinate{};
		int i;
		newCoordinate.x = atoi(input);
		for (i = 0; i < len; i++)
			if (input[i] == ',') break;
		i++;
		newCoordinate.y = atoi(input+i);
		for (; i < len; i++)
			if (input[i] == ',') break;
		i++;
		newCoordinate.z = atoi(input+i);
		return newCoordinate;
	}

	size_t Coordinate3D::operator()(const Coordinate3D &coordinate) const noexcept {
		return ((coordinate.x << 8) ^ (coordinate.y << 4) ^ (coordinate.z));
	}

	bool Coordinate3D::operator==(const Coordinate3D &other) const {
		return x == other.x && y == other.y && z == other.z;
	}

	bool Coordinate3D::operator!=(const Coordinate3D &other) const {
		return x != other.x || y != other.y || z != other.z;
	}

	Coordinate3D &Coordinate3D::operator+=(const Coordinate3D &other) {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		return *this;
	}

	Coordinate3D Coordinate3D::operator+(const Coordinate3D &other) const {
		Coordinate3D newCoordinate = *this;
		newCoordinate += other;
		return newCoordinate;
	}
} // Syn

size_t std::hash<Syn::Coordinate>::operator()(const Syn::Coordinate &coordinate) const noexcept {
	return coordinate(coordinate);
}

size_t std::hash<Syn::Coordinate3D>::operator()(const Syn::Coordinate3D &coordinate) const noexcept {
	return coordinate(coordinate);
}