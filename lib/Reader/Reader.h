//
// Created by Synergiance on 12/5/22.
//

#pragma once

#include <iostream>

namespace Syn {
	class Reader {
	public:
		static std::istream &getline(std::istream &is, std::string &str);
		static std::istream &getline(std::istream &is, std::string &str, char delim);
	};
} // Syn
