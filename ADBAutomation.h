#pragma once
#ifndef _ADB_AUTOMATION_H_
#define _ADB_AUTOMATION_H_

/**
 * Starting point for the Android Debug Bridge automation program.
 */
class ADBAutomation
{
private:
	class ModeAutomation * modeAutomation;

private:
	void printError();
	void initializeDirectory();
	void initializeMode();

public:
	ADBAutomation();
	~ADBAutomation();
	void testDirectory();
	void run();
};

#endif	// _ADB_AUTOMATION_H_