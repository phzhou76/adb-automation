#include "stdafx.h"
#include <stdlib.h>
#include <direct.h>
#include <iostream>
#include <sstream>

#include "ADBAutomation.h"
#include "TempestTrialsAutomation.h"
#include "ChainChallengeAutomation.h"

/**
 * Prints errors in the event of incorrect input.
 */
void ADBAutomation::printError()
{
	std::cout << "Error: Incorrect input." << std::endl;
	std::cout << "Press any key to end the program." << std::endl;
	std::getchar();
	exit(-1);
}

/**
* Initializes the current directory of the program to the directory where the
* ADB files are located.
*/
void ADBAutomation::initializeDirectory()
{
	_chdir("C:\\Users\\pzhou\\Documents\\Android SDK Platform Tools\\platform-tools");
}

/**
 * Initializes the correct mode. 
 */
void ADBAutomation::initializeMode()
{
	// Read input on which mode to automate.
	std::string input;
	std::cout << "1: Chain Challenge" << std::endl;
	std::cout << "2: Tempest Trials" << std::endl;
	std::cout << "Enter which mode you wish to automate: ";
	std::getline(std::cin, input);

	// Create the correct automator.
	if (input.compare("1") == 0)
		modeAutomation = new ChainChallengeAutomation();
	else if (input.compare("2") == 0)
		modeAutomation = new TempestTrialsAutomation(false);
	else
		printError();
}

/**
* Constructor for the ADBAutomation class.
*/
ADBAutomation::ADBAutomation()
{
	initializeDirectory();
	initializeMode();
}

/**
* Destructor for the ADBAutomation class.
*/
ADBAutomation::~ADBAutomation()
{
	delete modeAutomation;
}

/**
 * Tests the current directory that the program is looking at.
 */
void ADBAutomation::testDirectory()
{
	system("dir");
}

/**
 * Runs the mode automation program.
 */
void ADBAutomation::run()
{
	modeAutomation->run();
}

/**
* Entry point of the program.
*
* @return 0 if program was terminated successfully, -1 otherwise.
*/
int main()
{
	ADBAutomation automation;
	automation.run();

	return 0;
}