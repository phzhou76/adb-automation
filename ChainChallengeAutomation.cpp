#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <Windows.h>

#include "ChainChallengeAutomation.h"

/**
 * Given the error code, prints the respective error, and exits the program.
 *
 * @param error The error code pertaining to the issue.
 */
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

/**
 * Prompts the player for required data to automate the Chain Challenge.
 */
void ChainChallengeAutomation::initializeData()
{
	// Read data for the Chain Challenge.
	std::string input;
	std::cout << "Please leave your screen on the menu of the Chain Challenge that you want to automate." << std::endl;
	std::cout << "Does your team die when auto-battling the Chain Challenge? (y/n) ";
	std::getline(std::cin, input);

	// Error checking on answer.
	if (input.compare("y") == 0)
		teamDies = true;
	else if (input.compare("n") == 0)
		teamDies = false;
	else
		printError(ChainChallengeError::INVALID_ANSWER);

	if (teamDies)
		std::cout << "Enter how long in seconds your team takes to auto-battle until death: ";
	else
		std::cout << "Enter how long in seconds your team takes to auto-battle until the last map: ";
	std::getline(std::cin, input);

	// Error checking on answer.
	if (!(std::stringstream(input) >> secondsToComplete))
		printError(ChainChallengeError::INVALID_ANSWER);

	if (secondsToComplete < 0)
		printError(ChainChallengeError::INVALID_COMPLETION_TIME);
}

/**
 * Calls the required ADB inputs to enter the Chain Challenge and enable auto-battle. In addition,
 * this function will adjust the stamina accordingly.
 */
void ChainChallengeAutomation::enterChainChallenge()
{
	// Select the Chain Challenge.
	system("adb.exe shell input tap 720 850");

	// Need to determine if player has sufficient stamina for Chain Challenge.
	checkSufficientStamina(30);
	setStamina(getStamina() - 30);

	// Start the Chain Challenge.
	std::this_thread::sleep_for(std::chrono::seconds(1));
	system("adb.exe shell input tap 720 1550");

	// Adjust stamina and wait for 7 seconds so screen initializes.
	std::this_thread::sleep_for(std::chrono::seconds(7));

	// Open up options.
	system("adb.exe shell input tap 150 2450");

	// Select autobattle.
	std::this_thread::sleep_for(std::chrono::seconds(1));
	system("adb.exe shell input tap 720 1300");

	// Confirm autobattle.
	std::this_thread::sleep_for(std::chrono::seconds(1));
	system("adb.exe shell input tap 720 1200");
}

/**
 * Calls the required ADB inputs to leave the Chain Challenge to return to the Chain Challenge menu.
 */
void ChainChallengeAutomation::leaveChainChallenge()
{
	if (teamDies)
	{
		// Tap out of the Lose screen.
		system("adb.exe shell input tap 500 1500");
	}
	else
	{
		// Cancel auto-battle.
		system("adb.exe shell input tap 150 2450");

		// Select options.
		std::this_thread::sleep_for(std::chrono::seconds(7));
		system("adb.exe shell input tap 150 2450");

		// Select surrender.
		std::this_thread::sleep_for(std::chrono::seconds(1));
		system("adb.exe shell input tap 720 1800");

		// Confirm surrender.
		std::this_thread::sleep_for(std::chrono::seconds(1));
		system("adb.exe shell input tap 720 1300");
	}

	// Select surrender on team-select.
	std::this_thread::sleep_for(std::chrono::seconds(1));
	system("adb.exe shell input tap 400 2200");

	// Confirm surrender.
	std::this_thread::sleep_for(std::chrono::seconds(1));
	system("adb.exe shell input tap 720 1225");
}

ChainChallengeAutomation::ChainChallengeAutomation() : ModeAutomation()
{
	// Prompt the user for any data required to automate the Chain Challenge.
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

		// First, enter the Chain Challenge map and start auto-battle.
		enterChainChallenge();

		// Let auto-battle do its work.
		std::this_thread::sleep_for(std::chrono::seconds(secondsToComplete));

		// Leave Chain Challenge and restart.
		leaveChainChallenge();
	}
}
