//
// Created by Synergiance on 12/5/22.
//

#include "Parser.h"

namespace Syn {
	void Parser::Tokenize(const std::string &input, std::vector<std::string> &tokens, char delimiter) {
		for (size_t i = 0; i < input.length(); i++) {
			size_t substrBegin = i;
			i = input.find(delimiter, substrBegin);
			if (i == std::string::npos) i = input.size();
			tokens.push_back(input.substr(substrBegin, i - substrBegin));
		}
	}

	void Parser::Trim(std::string &input, char trimmedChar) {
		while (input.starts_with(trimmedChar))
			input.erase(input.cbegin());
		while (input.ends_with(trimmedChar))
			input.erase(input.cend()-1);
	}

	void Parser::TrimAndTokenize(std::string input, std::vector<std::string> &tokens, char delimeter) {
		Trim(input, delimeter);
		Tokenize(input, tokens, delimeter);
	}

	size_t Parser::GetNumberLength(const std::string &input, size_t offset) {
		for (size_t i = offset; i < input.size(); i++)
			if (!std::isdigit(input[i])) return i - offset;
		return input.size() - offset;
	}

	int Parser::GetNumFromStr(const char* str, size_t len) {
		size_t i;
		for (i = 0; i < len; i++)
			if (std::isdigit(str[i]) || str[i] == '-') break;
		return atoi(str + i);
	}
} // Syn