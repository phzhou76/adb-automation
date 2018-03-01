#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <Windows.h>

#include "ChainChallengeAutomation.h"

void ChainChallengeAutomation::printError(ChainChallengeError error)
{
	switch (error)
	{
	case ChainChallengeError::INVALID_ANSWER:
		std::cerr << "Error: Input was of invalid form." << std::endl;
		break;
	case ChainChallengeError::INVALID_COMPLETION_TIME:
		std::cerr << "Error: Completion time has an invalid value." << std::endl;
		break;
	default:
		break;
	}

	std::cout << "Press any key to end the program." << std::endl;
	std::getchar();
	exit(-1);
}

void ChainChallengeAutomation::initializeData()
{
	// Read data for the chain challenge.
	std::string input;
	std::cout << "Does your team die when auto-battling the chain challenge? (y/n) ";
	std::getline(std::cin, input);

	// Error checking on answer.

}

ChainChallengeAutomation::ChainChallengeAutomation() : ModeAutomation()
{
	// Prompt the user for any data required to automate the chain challenge.
	initializeData();
}

void ChainChallengeAutomation::run()
{
	bool exit = false;

	while (!exit) {
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			exit = true;
			endStaminaThread();
		}
	}
}
