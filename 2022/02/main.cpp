#include <iostream>
#include <fstream>
#include <string>

static std::string rpsNames[] = {
		"Rock    ",
		"Paper   ",
		"Scissors"
};

static std::string winState[] = {
		"Lose",
		"Draw",
		"Win "
};

/**
 * Calculates score of your play vs their play
 * @param yourPlay Your play (0 for Rock, 1 for Paper, 2 for Scissors)
 * @param theirPlay Their play (0 for Rock, 1 for Paper, 2 for Scissors)
 * @return The score you get given the plays
 */
int countEarnings(int yourPlay, int theirPlay) {
	// +4 is actually +1 but needs to result in a positive result because c++
	int win = (yourPlay + 4 - theirPlay) % 3;
	return win * 3 + yourPlay + 1;
}

/**
 * Determines what you should play to get the desired outcome
 * @param theirPlay Their play (0 for Rock, 1 for Paper, 2 for Scissors)
 * @param desiredOutcome Desired Outcome (0 for Lose, 1 for Draw, 2 for Win)
 * @return The play you should do for the desired outcome (0 for Rock, 1 for Paper, 2 for Scissors)
 */
int determinePlay(int theirPlay, int desiredOutcome) {
	// +2 is actually -1 but needs to result in a positive result because c++
	return (theirPlay + desiredOutcome + 2) % 3;
}

int main() {
	std::ifstream iFile("strategy_guide.dat");
	if (!iFile.is_open()) {
		std::cout<<"Could not open file!"<<std::endl;
		return 1;
	}

	std::cout<<"File successfully opened!"<<std::endl;
	std::string buffer;

	int totalScore = 0;

	while(std::getline(iFile, buffer)) {
		int bufferLength = buffer.size();
		if (bufferLength > 0 && buffer[bufferLength-1] == '\r')
			buffer.erase(bufferLength-1);

		if (buffer.empty()) continue;

		int theirPlay = (unsigned char)(buffer[0] - 'A');
		int desiredOutcome = (unsigned char)(buffer[2] - 'X');

		int yourPlay = determinePlay(theirPlay, desiredOutcome);
		int yourEarnings = countEarnings(yourPlay, theirPlay);

		std::cout<<"Their play: "<<rpsNames[theirPlay]<<"   Desired outcome: "<<winState[desiredOutcome]
		<<"     Your play: "<<rpsNames[yourPlay]<<"   Your earnings: "<<yourEarnings<<std::endl;

		totalScore += yourEarnings;
	}

	std::cout<<"Total earnings: "<<totalScore<<std::endl;

	return 0;
}