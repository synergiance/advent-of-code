//
// Created by Synergiance on 12/9/22.
//

#pragma once

#include <Coordinate.h>

namespace Syn {

	template<typename T>
	class Grid {
	public:
		Grid(int width, int height);
		~Grid();

		T &operator[](Coordinate location);
		int GetWidth();
		int GetHeight();
	private:
		int mWidth;
		int mHeight;
		T *mpData;
	};

	template<typename T>
	int Grid<T>::GetWidth() {
		return mWidth;
	}

	template<typename T>
	int Grid<T>::GetHeight() {
		return mHeight;
	}

	template<typename T>
	Grid<T>::Grid(int width, int height) {
		mpData = new T[width * height];
		mWidth = width;
		mHeight = height;
	}

	template<typename T>
	Grid<T>::~Grid() = default;

	template<typename T>
	T &Grid<T>::operator[](Coordinate location) {
		return mpData[location.x + location.y * mWidth];
	}

} // Syn
