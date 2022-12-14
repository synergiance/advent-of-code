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

		void Fill(T fill);
		void DrawLine(Coordinate start, Coordinate finish, T fill);
	private:
		int mWidth;
		int mHeight;
		T *mpData;

		void DrawVertical(int x, int y1, int y2, T fill);
		void DrawHorizontal(int x1, int x2, int y, T fill);

		int calcIndex(Coordinate location);
	};

	template<typename T>
	void Grid<T>::DrawHorizontal(int x1, int x2, int y, T fill) {
		if (x1 > x2) {
			int t = x1;
			x1 = x2;
			x2 = t;
		}

		for (int x = x1; x <= x2; x++)
			mpData[calcIndex({x, y})] = fill;
	}

	template<typename T>
	void Grid<T>::DrawVertical(int x, int y1, int y2, T fill) {
		if (y1 > y2) {
			int t = y1;
			y1 = y2;
			y2 = t;
		}

		for (int y = y1; y <= y2; y++)
			mpData[calcIndex({x, y})] = fill;
	}

	template<typename T>
	void Grid<T>::DrawLine(Coordinate start, Coordinate finish, T fill) {
		if (start.x == finish.x) DrawVertical(start.x, start.y, finish.y, fill);
		if (start.y == finish.y) DrawHorizontal(start.x, finish.x, finish.y, fill);
	}

	template<typename T>
	void Grid<T>::Fill(T fill) {
		for (int i = 0; i < mWidth * mHeight; i++)
			mpData[i] = fill;
	}

	template<typename T>
	int Grid<T>::calcIndex(Coordinate location) {
		return location.x + location.y * mWidth;
	}

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
		return mpData[calcIndex(location)];
	}

} // Syn
