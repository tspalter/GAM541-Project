/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: BossAttack.h
Purpose: Boss Attack Component for a boss with Attacks on an interval and no movement.
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Adam Rhoades
- End Header --------------------------------------------------------*/

#pragma once
#include "Component.h"

class GameObject;

enum BOSS_ACTION_TYPE {
	ATTACK,
	WAIT
};

class BossAttack : public Component {
public:
	BossAttack();
	~BossAttack();

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
