/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: UpDown.h
Purpose: Extremely simple controller to demonstrate NPC behaviour
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_2
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 15, 2020
- End Header --------------------------------------------------------*/

#pragma once
#include "Component.h"

class GameObject;

class UpDown : public Component {
public:
	UpDown();
	~UpDown();

	void Update();

	void Serialize(std::ifstream& InputStream);
	void Serialize(rapidjson::GenericArray<false, rapidjson::Value>);

	void HandleEvent(Event* pEvent);

public:
	bool mGoingUp;
	int mTimer, mTimerLimit;


private:
private:
};
