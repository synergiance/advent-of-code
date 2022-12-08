//
// Created by Synergiance on 12/5/22.
//

#include "Reader.h"

namespace Syn {
	std::istream &Reader::getline(std::istream &is, std::string &str) {
		std::getline(is, str);

		if (str.empty()) return is;

		if (str.back() == '\r')
			str.erase(str.size() - 1);

		return is;
	}

	std::istream &Reader::getline(std::istream &is, std::string &str, char delim) {
		return std::getline(is, str, delim);
	}

	std::istream &Reader::ReadUntilEmptyLn(std::istream &is, std::vector<std::string> &lines) {
		std::string buffer;
		while (getline(is, buffer)) {
			if (buffer.empty()) break;
			lines.push_back(buffer);
		}
		return is;
	}

	std::istream &Reader::ReadEntireFile(std::istream &is, std::vector<std::string> &lines) {
		std::string buffer;
		while (getline(is, buffer)) lines.push_back(buffer);
		while (lines.back().empty()) lines.erase(lines.cend());
		return is;
	}
} // Syn