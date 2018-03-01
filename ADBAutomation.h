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

public:
	ADBAutomation();
	~ADBAutomation();

private:
	void initializeDirectory();

public:
	void testDirectory();
	void run();
};

#endif	// _ADB_AUTOMATION_H_