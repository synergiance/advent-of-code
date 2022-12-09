//
// Created by Synergiance on 12/9/22.
//

#include "Grid.h"

namespace Syn {
	Grid::Grid(int width, int height) {
		this->width = width;
		this->height = height;
		data = new int[width * height];
	}

	Grid::~Grid() {
		delete [] data;
	}

	int &Grid::operator[](Coordinate location) {
		return data[location.x + location.y * width];
	}
} // Syn