#pragma once
#ifndef _TEMPEST_TRIALS_AUTOMATION_H_
#define _TEMPEST_TRIALS_AUTOMATION_H_

#include "ModeAutomation.h"

class TempestTrialsAutomation : public ModeAutomation 
{
private:
	int tempestScore;	// Current tempest trials score.
	int scoreGain;		// Amount of points gained per Tempest Trials run.

public:
	TempestTrialsAutomation();

private:
	void initializeScores();

public:
	void run();
};

#endif	// _TEMPEST_TRIALS_AUTOMATION_H_