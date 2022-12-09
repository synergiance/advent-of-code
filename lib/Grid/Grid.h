//
// Created by Synergiance on 12/9/22.
//

#pragma once

#include "Coordinate.h"

namespace Syn {

	template<typename T>
	class Grid {
	public:
		Grid(int width, int height);
		~Grid();

		T &operator[](Coordinate location);
	private:
		int width;
		int height;
		T *data;
	};

} // Syn
