/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Moderator.h
Purpose:  Handles switching levels for Original fighting game.
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne
- End Header --------------------------------------------------------*/

#pragma once

class Moderator {
public:
	Moderator();
	~Moderator();

public:
	void Update();
	int mStage;
	float mTimer;
	bool mTransition;
};
