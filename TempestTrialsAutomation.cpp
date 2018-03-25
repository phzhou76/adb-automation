#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <Windows.h>

#include "TempestTrialsAutomation.h"

/**
 * Given the error code, prints the respective error, and exits the program.
 *
 * @param error The error code pertaining to the issue.
 */
void TempestTrialsAutomation::printError(TempestTrialsError error)
{
	switch (error)
	{
	case TempestTrialsError::INVALID_TOTAL_TYPE:
		std::cerr << "Error: Total score is an invalid type." << std::endl;
		break;
	case TempestTrialsError::INVALID_TOTAL_AMOUNT:
		std::cerr << "Error: Total score is an invalid value." << std::endl;
		break;
	case TempestTrialsError::INVALID_SCORE_TYPE:
		std::cerr << "Error: Score gained is an invalid typed." << std::endl;
		break;
	case TempestTrialsError::INVALID_SCORE_AMOUNT:
		std::cerr << "Error: Score gained is an invalid value." << std::endl;
		break;
	case TempestTrialsError::INVALID_TT_TIME_TYPE:
		std::cerr << "Error: Completion time is an invalid type." << std::endl;
		break;
	case TempestTrialsError::INVALID_TT_TIME_AMOUNT:
		std::cerr << "Error: Completion time is an invalid value." << std::endl;
		break;
	default:
		break;
	}

	std::cout << "Press any key to end the program." << std::endl;
	std::getchar();
	exit(-1);
}

/**
 * Prompts the player for required data to automate the Tempest Trials.
 */
void TempestTrialsAutomation::initializeData()
{
	// Read data for the Tempest Trials.
	std::string input;
	std::cout << "Please leave your screen on the menu of the Tempest Trials." << std::endl;
	std::cout << "Enter your current score: ";
	std::getline(std::cin, input);

	// Error checking on the total score.
	if (!(std::stringstream(input) >> tempestScore))
		printError(TempestTrialsError::INVALID_TOTAL_TYPE);
	if (tempestScore < 0)
		printError(TempestTrialsError::INVALID_TOTAL_AMOUNT);

	std::cout << "Enter the score gained per run of Hard 4: ";
	std::getline(std::cin, input);

	// Error checking on the score gain.
	if (!(std::stringstream(input) >> scoreGain))
		printError(TempestTrialsError::INVALID_SCORE_TYPE);
	if (scoreGain < 0)
		printError(TempestTrialsError::INVALID_SCORE_AMOUNT);

	std::cout << "Enter the length of time in seconds to finish a run of Hard 4: ";
	std::getline(std::cin, input);

	// Error checking on the time taken.
	if (!(std::stringstream(input) >> secondsToComplete))
		printError(TempestTrialsError::INVALID_TT_TIME_TYPE);
	if (secondsToComplete < 0)
		printError(TempestTrialsError::INVALID_TT_TIME_AMOUNT);
}

/**
 * Updates the Tempest Trials score, and checks if a reward tier has been hit.
 */
void TempestTrialsAutomation::updateScores()
{
	/*
	 * Edge case: If the current reward tier is the size of the array, that means
	 * that all rewards have been collected.
	 */
	if (nextReward >= rewardPoints.at(0).size())
		return;

	tempestScore += scoreGain;

	if (tempestScore >= rewardPoints.at(0).at(nextReward))
	{
		// Exit reward screen.
		std::this_thread::sleep_for(std::chrono::seconds(5));
		system("adb.exe shell input tap 720 1700");
		++nextReward;
	}
}

/**
 * Calls the required ADB inputs to enter the Tempest Trials and enable auto-battle. In addition,
 * this function will adjust the stamina accordingly.
 */
void TempestTrialsAutomation::enterMode()
{
	// Need to determine if player has sufficient stamina for Chain Challenge.
	checkSufficientStamina(20);
	setStamina(getStamina() - 20);

	// Select Hard 4.
	std::this_thread::sleep_for(std::chrono::seconds(4));
	system("adb.exe shell input tap 720 2150");

	// Confirm selection.
	std::this_thread::sleep_for(std::chrono::seconds(5));
	system("adb.exe shell input tap 720 1700");

	enableAutobattle();
}

/**
 * Calls the required ADB inputs to leave the Tempest Trials to return to the Tempest
 * Trials menu.
 */
void TempestTrialsAutomation::leaveMode()
{
	// Exit clear screen.
	system("adb.exe shell input tap 720 1200");

	// Exit score screen.
	std::this_thread::sleep_for(std::chrono::seconds(6));
	system("adb.exe shell input tap 720 2200");

	// Update and check scores.
	updateScores();

	// Wait past possible loading.
	std::this_thread::sleep_for(std::chrono::seconds(8));
}

/**
 * Constructor for the Tempest Trials automator.
 */
TempestTrialsAutomation::TempestTrialsAutomation(bool mini) : ModeAutomation()
{
	isMini = mini;
	rewardPoints.push_back({ 100, 200, 300, 400, 500, 700, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000,
		5500, 6000, 7000, 8000, 9000, 10000, 11000, 12000, 14000, 16000, 18000, 20000, 22500, 25000, 27500, 30000,
		32500, 35000, 37500, 40000, 42500, 45000, 47500, 50000, 52500, 55000, 57500, 60000, 62500, 65000, 67500,
		70000, 72500, 75000, 77500, 80000, 82500, 85000, 87500, 90000, 92500, 95000, 97500, 99999 });

	// Prompt the user for any data required to automate the Tempest Trials.
	initializeData();

	// Determine what the current reward tier is.
	nextReward = 0;
	while (nextReward < rewardPoints.at(0).size() && rewardPoints.at(0).at(nextReward) <= tempestScore)
		++nextReward;

	if (nextReward < rewardPoints.at(0).size())
		std::cout << "Current reward tier: " << rewardPoints.at(0).at(nextReward) << std::endl;
}

/**
 * Runs the Tempest Trials automator. Press ESC to exit the program.
 */
void TempestTrialsAutomation::run()
{
	bool exit = false;

	while (!exit) {
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			exit = true;
			endStaminaThread();
		}

		// First, enter the Chain Challenge map and start auto-battle.
		enterMode();

		// Let auto-battle do its work.
		std::this_thread::sleep_for(std::chrono::seconds(secondsToComplete));

		// Leave Chain Challenge and restart.
		leaveMode();
	}
}
