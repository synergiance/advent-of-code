//
// Created by Synergiance on 12/5/22.
//

#include "Parser.h"

namespace Syn {
	void Parser::Tokenize(const std::string &input, std::vector<std::string> &tokens, char delimiter) {
		for (int i = 0; i < input.length(); i++) {
			int substrBegin = i;
			i = (int)input.find(delimiter, substrBegin);
			if (i == std::string::npos) i = (int)input.size();
			tokens.push_back(input.substr(substrBegin, i - substrBegin));
		}
	}
} // Syn