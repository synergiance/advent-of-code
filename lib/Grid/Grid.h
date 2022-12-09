//
// Created by Synergiance on 12/9/22.
//

#pragma once

#include "Coordinate.h"

namespace Syn {

	class Grid {
	public:
		Grid(int width, int height);
		~Grid();

		int &operator[](Coordinate location);
	private:
		int width;
		int height;
		int *data;
	};

} // Syn
