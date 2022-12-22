//
// Created by Synergiance on 12/5/22.
//

#pragma once

#include <string>
#include <vector>

namespace Syn {
	class Parser {
	public:
		static void Tokenize(const std::string &input, std::vector<std::string> &tokens, char delimiter = ' ');
		static void Trim(std::string &input, char trimmedChar = ' ');
		static void TrimAndTokenize(std::string input, std::vector<std::string> &tokens, char delimeter = ' ');
		static size_t GetNumberLength(const std::string &input, size_t offset = 0);
		static int GetNumFromStr(const char* str, size_t len = 0x20);
	};
} // Syn
