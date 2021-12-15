/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: LeftRight.h
Purpose: Basic AI for guards to patrol, and attack 3 times during their patrol duration. Fully customizable in json.
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Adam Rhoades, Dylan Washburne
- End Header --------------------------------------------------------*/

#pragma once
#include "Component.h"

class GameObject;

enum ACTION_TYPE {
	WALK_RIGHT,
	WAIT_RIGHT,
	WALK_LEFT,
	WAIT_LEFT
};

class LeftRight : public Component {
public:
	LeftRight();
	~LeftRight();

	void Update();

	void Serialize(rapidjson::GenericArray<false, rapidjson::Value> input);
	void Serialize(std::ifstream& InputStream);

	void HandleEvent(Event* pEvent);

public:
	
	int mTimer, mTimerLimit, mSwingTimer;
	int numActions;
	int mMoveDuration, mSwingDuration;
	int mSpeed;
	unsigned int mAction;
	bool isStunned;
	bool mSwinging;
	float mAttackWidth;
	float mAttackAngle;
	float mAttackAngleMod;
	float mAttackLength;
	float mDamage;
	bool MiddleAttack;


private:
	void UpdateTimersAndAngles();
private:
};
