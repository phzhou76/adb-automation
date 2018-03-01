#include "stdafx.h"
#include <iostream>
#include <Windows.h>

#include "ChainChallengeAutomation.h"

void ChainChallengeAutomation::initializeData()
{
	// Read data for the chain challenge.
	std::string input;
	// std::cout << 

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
