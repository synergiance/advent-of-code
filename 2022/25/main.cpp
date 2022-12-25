#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Parser.h>
#include <Reader.h>
#include <map>

using namespace Syn;

#define SNAFU_BASE 5

const char snafuDigits[] = {
		'=', '-', '0', '1', '2'
};

const std::map<char, long> snafuToLong {
		{'=', -2},
		{'-', -1},
		{'0', 0},
		{'1', 1},
		{'2', 2}
};

long DecodeSNAFU(const std::string &str) {
	long num = 0;
	for (const char &ch : str) {
		num *= SNAFU_BASE;
		num += snafuToLong.find(ch)->second;
	}
	return num;
}

std::string EncodeSNAFU(long num) {
	std::string str;
	while (num != 0) {
		num += 2;
		str.insert(str.cbegin(), snafuDigits[num % SNAFU_BASE]);
		num /= SNAFU_BASE;
	}
	return str;
}

int main() {
	std::ifstream iFile("fuel_requirements_encoded.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	// TODO: Set up

	while (Reader::getline(iFile, buffer)) {
		if (buffer.empty()) continue;

		long n = DecodeSNAFU(buffer);
		std::cout<<"Converting SNAFU to decimal to SNAFU: "<<buffer<<" --> "<<n<<" --> "<<EncodeSNAFU(n)<<std::endl;
	}

	// TODO: Output

	iFile.close();
	return 0;
}
