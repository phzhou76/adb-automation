#pragma once
#ifndef _TEMPEST_TRIALS_AUTOMATION_H_
#define _TEMPEST_TRIALS_AUTOMATION_H_

#include <vector>

#include "ModeAutomation.h"

enum TempestTrialsError
{
	INVALID_TOTAL_TYPE,
	INVALID_TOTAL_AMOUNT,
	INVALID_SCORE_TYPE,
	INVALID_SCORE_AMOUNT,
	INVALID_TT_TIME_TYPE,
	INVALID_TT_TIME_AMOUNT
};

class TempestTrialsAutomation : public ModeAutomation 
{
private:
	int tempestScore;							// Current tempest trials score.
	int scoreGain;								// Amount of points gained per Tempest Trials run.
	int secondsToComplete;						// Amount of time required to complete the Tempest Trials run.
	bool isMini;								// Denotes if mini Tempest Trials or not.
	std::vector<std::vector<int> > rewardPoints;	// Points at which rewards are given.
	int nextReward;	// Denotes the next reward point, if it exceeds the size of the rewardsPoint array, then all rewards have been received.

private:
	void printError(TempestTrialsError error);
	void initializeData();
	void updateScores();

protected:
	void enterMode();
	void leaveMode();

public:
	TempestTrialsAutomation(bool mini);
	void run();
};

#endif	// _TEMPEST_TRIALS_AUTOMATION_H_