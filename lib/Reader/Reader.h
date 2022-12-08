//
// Created by Synergiance on 12/5/22.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace Syn {
	class Reader {
	public:
		static std::istream &getline(std::istream &is, std::string &str);
		static std::istream &getline(std::istream &is, std::string &str, char delim);

		static std::istream &ReadUntilEmptyLn(std::istream &is, std::vector<std::string> &lines);
		static std::istream &ReadEntireFile(std::istream &is, std::vector<std::string> &lines);
	};
} // Syn
