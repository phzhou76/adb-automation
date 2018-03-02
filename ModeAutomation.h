#pragma once
#ifndef _MODE_AUTOMATION_H_
#define _MODE_AUTOMATION_H_

#include <thread>
#include <mutex>
#include <condition_variable>

enum ModeTypes
{
	TempestTrials,
	ChainChallenge
};

enum StaminaError
{
	INVALID_STAMINA_TYPE,
	INVALID_STAMINA_AMOUNT,
	INVALID_TIME_FORMAT,
	INVALID_TIME_TYPE,
	INVALID_TIME_AMOUNT
};

class ModeAutomation
{
private:
	std::thread thr;				// Thread for running stamina refresh.
	std::mutex mtx;
	std::condition_variable cv;
	bool bIsRunning;				// Denotes if thread should be running.
	bool bExternalChange;			// Denotes if an external change to stamina or countdown will happen.

protected:
	int stamina;		// Current stamina of the player.
	int secondsLeft;	// Number of seconds left until stamina should be incremented.

private:
	void printError(StaminaError error);
	void initializeStamina();

	// Run on a separate thread.
	bool noExternalChanges();
	void adjustStamina();
	void refreshStamina();
	void runStaminaThread();

protected:
	int getStamina();
	void setStamina(int staminaInput);
	int getSecondsLeft();
	void setSecondsLeft(int secondsLeftInput);
	void checkSufficientStamina(int requiredStamina);

public:
	ModeAutomation();		// Default constructor.
	void endStaminaThread();
	virtual void run() = 0;	// Implementation is specific to the mode that is being automated.
};

#endif	// _MODE_AUTOMATION_H_