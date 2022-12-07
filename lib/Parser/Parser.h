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
	};
} // Syn