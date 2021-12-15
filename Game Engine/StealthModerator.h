/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: StealthModerator.cpp
Purpose:  Handles switching levels, Credits, Openings, Transitions, and Player Death/Victory for ConCaveity
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Adam Rhoades, Dylan Washburne
- End Header --------------------------------------------------------*/

#pragma once

class StealthModerator {
public:
	StealthModerator();
	~StealthModerator();

public:
	void Update();
	int mStage,mLastStage;
	float mTimer;
	bool mTransition;
	bool mManualOverride, mManualBack, mManualRestart, mGoToMainMenu;

	int mTransitionTimer, mTransitionTimerLimit;
};
