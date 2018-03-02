#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <chrono>

#include "ModeAutomation.h"

/**
 * Given the error code, prints the respective error, and exits the program upon keypress.
 *
 * @param error The error code pertaining to the issue.
 */
void ModeAutomation::printError(StaminaError error)
{
	switch (error)
	{
	case StaminaError::INVALID_STAMINA_TYPE:
		std::cerr << "Error: Stamina input was not an integer." << std::endl;
		break;
	case StaminaError::INVALID_STAMINA_AMOUNT:
		std::cerr << "Error: Stamina amount was invalid." << std::endl;
		break;
	case StaminaError::INVALID_TIME_FORMAT:
		std::cerr << "Error: Time input format was not in [minutes:seconds] format." << std::endl;
		break;
	case StaminaError::INVALID_TIME_TYPE:
		std::cerr << "Error: Time input type for either minutes or seconds was not an integer." << std::endl;
		break;
	case StaminaError::INVALID_TIME_AMOUNT:
		std::cerr << "Error: Time input amount for either minutes or seconds was invalid." << std::endl;
		break;
	default:
		break;
	}

	std::cout << "Press any key to end the program." << std::endl;
	std::getchar();
	exit(-1);
}

/**
 * Prompts the player for his or her current stamina and the time left until the
 * next point of stamina is added.
 */
void ModeAutomation::initializeStamina()
{
	// Read in the player's stamina amount.
	std::string input;
	std::cout << "Enter your current stamina amount: ";
	std::getline(std::cin, input);

	// Error checking on stamina values.
	if (!(std::stringstream(input) >> stamina))
		printError(StaminaError::INVALID_STAMINA_TYPE);
	else if (stamina > 99 || stamina < 0)
		printError(StaminaError::INVALID_STAMINA_AMOUNT);

	// Read in the player's time left until next stamina point refresh.
	std::cout << "Enter the time left until next stamina point refresh in [minutes:seconds] format: ";
	std::getline(std::cin, input);

	// Extract the minute and second fields, and do error checking on the inputs.
	std::size_t colonIndex = input.find(':');
	if (colonIndex == std::string::npos || colonIndex == input.length() - 1)
		printError(StaminaError::INVALID_TIME_FORMAT);

	int minutes = 0;
	int seconds = 0;

	// Read in the minutes first.
	if (!(std::stringstream(input.substr(0, colonIndex)) >> minutes))
		printError(StaminaError::INVALID_TIME_TYPE);

	// Read in the seconds second.
	if (!(std::stringstream(input.substr(colonIndex + 1)) >> seconds))
		printError(StaminaError::INVALID_TIME_TYPE);

	secondsLeft = minutes * 60 + seconds;

	// Neither minutes or seconds can be negative, and the total must be at most 300 seconds.
	if (minutes < 0 || seconds < 0 || secondsLeft > 300)
		printError(StaminaError::INVALID_TIME_AMOUNT);

	// Immediately start the countdown once values have been initialized.
	runStaminaThread();
}

/**
 * Determines if an external change is being queued up. Used for the condition
 * variable's condition test.
 *
 * @return True if no external changes are being queued up, false if there are.
 */
bool ModeAutomation::noExternalChanges()
{
	return !bExternalChange;
}

/**
 * Adjusts stamina amount based on the countdown timer.
 * Note: Atomic variables are only useful if two threads are running the same line of
 * code so that there are no race conditions within that single line. But if two threads
 * are running different pieces of code that affect a variable's value, need to use
 * mutex and condition variables.
 */
void ModeAutomation::adjustStamina()
{
	/*
	 * Acquire lock and proceed if there are no external changes to be made. Otherwise,
	 * release the lock and sleep until woken up by another thread.
	 */
	std::unique_lock<std::mutex> lock(mtx);
	cv.wait(lock, std::bind(&ModeAutomation::noExternalChanges, this));

	// Edge case - stamina cannot go above 99. Stop and reset countdown when it does.
	if (stamina >= 99)
	{
		stamina = 99;
		secondsLeft = 300;
		lock.unlock();
		return;
	}

	// Decrement the countdown timer.
	--secondsLeft;

	// A single point of stamina is 5 minutes, or 300 seconds.
	if (secondsLeft == 0)
	{
		++stamina;
		secondsLeft = 300;
	}

	// Release the lock.
	lock.unlock();
}

/**
 * Continuously runs AdjustStamina in the background while the program is running.
 * This function must be run via RunStaminaThread.
 */
void ModeAutomation::refreshStamina()
{
	while (bIsRunning)
	{
		// Sleep for one second to properly decrement the countdown timer.
		std::this_thread::sleep_for(std::chrono::seconds(1));
		adjustStamina();
	}
}

/**
 * Creates a new thread that runs RefreshStamina in the background. EndStaminaThread
 * must be called to allow the thread to finish its task and join back to the main thread.
 */
void ModeAutomation::runStaminaThread()
{
	thr = std::thread(&ModeAutomation::refreshStamina, this);
}

/**
 * Getter for the amount of stamina that the player has.
 *
 * @return The amount of stamina that the player has.
 */
int ModeAutomation::getStamina()
{
	std::lock_guard<std::mutex> lock(mtx);
	return stamina;
}

/**
 * Setter for the amount of stamina that the player has.
 *
 * @param staminaInput The new amount of stamina that the player should have.
 */
void ModeAutomation::setStamina(int staminaInput)
{
	// Alert worker thread to pause so that stamina can be updated.
	bExternalChange = true;

	std::unique_lock<std::mutex> lock(mtx);
	stamina = staminaInput;
	bExternalChange = false;

	lock.unlock();
	cv.notify_all();
}

/**
 * Getter for the amount of time left until stamina refresh.
 *
 * @return The amoount of time left until stamina refresh.
 */
int ModeAutomation::getSecondsLeft()
{
	std::lock_guard<std::mutex> lock(mtx);
	return secondsLeft;
}

/**
 * Setter for the amount of time left until stamina refresh.
 *
 * @param secondsLeftInput The new amount of time left until stamina refresh.
 */
void ModeAutomation::setSecondsLeft(int secondsLeftInput)
{
	// Alert worker thread to pause so that secondsLeft can be updated.
	bExternalChange = true;

	std::unique_lock<std::mutex> lock(mtx);
	secondsLeft = secondsLeftInput;
	bExternalChange = false;

	lock.unlock();
	cv.notify_all();
}

/**
* Default constructor for the mode automation class. Will prompt the user for the stamina
* and time left until stamina point refresh values to initialize class values properly.
*/
ModeAutomation::ModeAutomation()
{
	// Initialize all field variables.
	stamina = secondsLeft = 0;
	bIsRunning = true;
	bExternalChange = false;
	bDeviceAFK = false;

	// Prompt user input to properly set stamina and secondsLeft fields.
	initializeStamina();
}

/**
 * Used to end the continuous loop found in RefreshStamina and to join the created thread
 * back to the main thread.
 */
void ModeAutomation::endStaminaThread()
{
	bIsRunning = false;
	thr.join();
}

/**
* Prevents the Android device from sleeping by sending inputs.
*/
void ModeAutomation::preventDeviceSleep()
{
	while (bDeviceAFK)
	{
		// Prevent spamming of inputs.
		std::this_thread::sleep_for(std::chrono::seconds(1));
		system("adb.exe shell input tap 700 975");
	}
}