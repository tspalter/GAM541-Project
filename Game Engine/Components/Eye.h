/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Eye.h
Purpose: Boss Attack Component for a Boss that looks around, killing the player if it sees it.
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Thomas Spalter
- End Header --------------------------------------------------------*/

#pragma once
#include "Component.h"

class GameObject;

class Eye : public Component {
public:
	Eye();
	~Eye();

	void Update();

	void Serialize(rapidjson::GenericArray<false, rapidjson::Value> input);
	void Serialize(std::ifstream& InputStream);

	void HandleEvent(Event* pEvent);

public:

	int mTimer, mTimerLimit;
	int mWaitDuration, mAttackDuration;
	unsigned int mAction;
	bool isStunned;
	bool mSwinging;
	float mAttackWidth;
	float mAttackAngle;
	float mAttackAngleMod;
	float mAttackLength;
	float mDamage;


private:
	void UpdateTimersAndAngles();
private:
};
