#pragma once
#ifndef _CHAIN_CHALLENGE_AUTOMATION_H_
#define _CHAIN_CHALLENGE_AUTOMATION_H_

#include "ModeAutomation.h"

enum ChainChallengeError
{
	INVALID_ANSWER,
	INVALID_COMPLETION_TIME
};

class ChainChallengeAutomation : public ModeAutomation
{
private:
	int secondsToComplete;			// Amount of time in seconds to complete a chain challenge.
	bool teamDies;					// Denotes if the team dies after an auto-battle run or not.

private:
	void printError(ChainChallengeError error);
	void initializeData();
	void enterChainChallenge();
	void leaveChainChallenge();

public:
	ChainChallengeAutomation();		// Default constructor.
	void run();
};

#endif	// _CHAIN_CHALLENGE_AUTOMATION_H_