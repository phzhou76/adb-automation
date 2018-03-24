#include "stdafx.h"
#include <stdlib.h>
#include <direct.h>
#include <iostream>

#include "ADBAutomation.h"
#include "TempestTrialsAutomation.h"
#include "ChainChallengeAutomation.h"

/**
* Initializes the current directory of the program to the directory where the
* ADB files are located.
*/
void ADBAutomation::initializeDirectory()
{
	_chdir("C:\\Users\\pzhou\\Documents\\Android SDK Platform Tools\\platform-tools");
}

/**
* Constructor for the ADBAutomation class.
*/
ADBAutomation::ADBAutomation()
{
	initializeDirectory();
	modeAutomation = new TempestTrialsAutomation(false);
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