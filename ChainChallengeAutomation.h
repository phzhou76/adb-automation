#pragma once
#ifndef _CHAIN_CHALLENGE_AUTOMATION_H_
#define _CHAIN_CHALLENGE_AUTOMATION_H_

#include "ModeAutomation.h"

class ChainChallengeAutomation : public ModeAutomation
{
private:
	int secondsToComplete;			// Amount of time in seconds to complete a chain challenge.
	bool isDead;					// Denotes if the team is dead after an auto-battle run or not.

private:
	void initializeData();

public:
	ChainChallengeAutomation();		// Default constructor.
	void run();
};

#endif	// _CHAIN_CHALLENGE_AUTOMATION_H_