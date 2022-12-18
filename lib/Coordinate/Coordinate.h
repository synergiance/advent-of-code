//
// Created by Synergiance on 12/9/22.
//

#pragma once

#include <cstddef>
#include <functional>

namespace Syn {

	struct Coordinate {
		int x;
		int y;

		[[nodiscard]] bool isDiagonal() const;
		static int Distance(const Coordinate &a, const Coordinate &b);
		[[nodiscard]] int Magnitude() const;
		[[nodiscard]] Coordinate Flattened() const;
		[[nodiscard]] Coordinate Absolute() const;
		[[nodiscard]] static Coordinate Parse(const char *input, size_t len = 0x20);

		size_t operator()(const Coordinate &coordinate) const noexcept;
		bool operator==(const Coordinate &other) const;
		bool operator!=(const Coordinate &other) const;
		Coordinate &operator+=(const Coordinate &other);
		Coordinate operator+(const Coordinate &other) const;
		Coordinate &operator-=(const Coordinate &other);
		Coordinate operator-(const Coordinate &other) const;
		Coordinate &operator*=(const Coordinate &other);
		Coordinate operator*(const Coordinate &other) const;
		Coordinate &operator/=(const Coordinate &other);
		Coordinate operator/(const Coordinate &other) const;
		Coordinate &operator%=(const Coordinate &other);
		Coordinate operator%(const Coordinate &other) const;
	};

	struct Coordinate3D {
		int x;
		int y;
		int z;

		[[nodiscard]] static Coordinate3D Parse(const char *input, size_t len = 0x20);

		size_t operator()(const Coordinate3D &coordinate) const noexcept;
		bool operator==(const Coordinate3D &other) const;
		bool operator!=(const Coordinate3D &other) const;
		Coordinate3D &operator+=(const Coordinate3D &other);
		Coordinate3D operator+(const Coordinate3D &other) const;
		Coordinate3D &operator-=(const Coordinate3D &other);
		Coordinate3D operator-(const Coordinate3D &other) const;
	};

} // Syn

namespace std {
	template<> struct hash<Syn::Coordinate> {
		size_t operator()(const Syn::Coordinate &coordinate) const noexcept;
	};

	template<> struct hash<Syn::Coordinate3D> {
		size_t operator()(const Syn::Coordinate3D &coordinate) const noexcept;
	};

	template<> struct less<Syn::Coordinate> {
		bool operator()(const Syn::Coordinate &lhs, const Syn::Coordinate &rhs) const {
			return (lhs.x + lhs.y) < (rhs.x + rhs.y);
		}
	};
}