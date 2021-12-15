/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Controller.h
Purpose: Handles player movement and attacking
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne, Adam Rhoades
- End Header --------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "../EventManager.h"
#include "Transform.h"

class GameObject;
// #include "..\GameObject.h"

class PlayerHitEvent : public Event {
public:
	PlayerHitEvent();
	~PlayerHitEvent();
public:


private:
private:
};

class Controller : public Component {
public:
	Controller();
	~Controller();

	void Update();

	void Serialize(std::ifstream& InputStream) { }
	void Serialize(rapidjson::GenericArray<false, rapidjson::Value>);

	void HandleEvent(Event* pEvent);
	void Jump(Transform* pT);

public:
	float mSwingAng;
	int mMaxSpeed;
	bool mGodMode;


private:
private:
	// float mVelHoriz, mVelVert;
	int mAcceleration;
	float mSlowMod, mExcessSlowMod;
	int mDashTimer, mDashCooldown;

	int mSwingTime, mSwingTimer, mSwingDelay;
	float mSwingWidth, mSwingLen;
	bool mSwinging, mCleaver;
	bool mIsGrounded;
	bool mIsPrevGrounded;
	int mJumpsLeft, mJumps;
};
