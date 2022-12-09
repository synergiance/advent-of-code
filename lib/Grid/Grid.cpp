//
// Created by Synergiance on 12/9/22.
//

#include "Grid.h"

namespace Syn {
	template<typename T>
	Grid<T>::Grid(int width, int height) {
		this->width = width;
		this->height = height;
		data = new T[width * height];
	}

	template<typename T>
	Grid<T>::~Grid() {
		delete [] data;
	}

	template<typename T>
	T &Grid<T>::operator[](Coordinate location) {
		return data[location.x + location.y * width];
	}
} // Syn