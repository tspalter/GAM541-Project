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

#include "StealthModerator.h"
#include <string>

#include "GameObject.h"
#include "Components/Component.h"
#include "Components/Character.h"
#include "Components/Objective.h"
#include "Components/Body.h"
#include "Components/Transform.h"

#include "ResourceManager.h"
#include "GameObjectManager.h"
#include "ObjectFactory.h"
#include "FrameRateController.h"

extern ResourceManager* gpResourceManager;
extern GameObjectManager* gpGameObjectManager;
extern ObjectFactory* gpObjectFactory;
extern FrameRateController* gpFRC;

extern int gGameType;

StealthModerator::StealthModerator() {
	mStage = 0;
	mTimer = 0.0f;
	mTransition = true;
	mTransitionTimer = mTransitionTimerLimit = 900;
	mManualOverride = false;
	mManualBack = false;
	mManualRestart = false;
	mLastStage = 0;
}

StealthModerator::~StealthModerator() {
	//
}

void StealthModerator::Update() {

	
	if (mManualRestart && mStage == 666)
	{
		gpGameObjectManager->~GameObjectManager();
		mStage = mLastStage;
		gpObjectFactory->LoadLevel(("..\\Resources\\StealthLevel" + std::to_string(mStage) + ".json").c_str());
		mTransitionTimer = mTransitionTimerLimit;
		mManualRestart = false;
		mManualOverride = false;
		mManualBack = false;
	}

	if (mManualRestart && mStage == 99)
	{
		gpGameObjectManager->~GameObjectManager();
		mStage = 1;
		gpObjectFactory->LoadLevel(("..\\Resources\\StealthLevel" + std::to_string(mStage) + ".json").c_str());
		mTransitionTimer = mTransitionTimerLimit;
		mManualRestart = false;
		mManualOverride = false;
		mManualBack = false;
	}

	if (mManualRestart && mStage > 0 && mStage < 99) {
		mManualRestart = false;
		gpGameObjectManager->~GameObjectManager();
		gpObjectFactory->LoadLevel(("..\\Resources\\StealthLevel" + std::to_string(mStage) + ".json").c_str());
	}
	if (mManualRestart && mStage < 0)
	{
		gpGameObjectManager->~GameObjectManager();
		gpObjectFactory->LoadLevel(("..\\Resources\\Opening" + std::to_string(mStage + 3) + ".json").c_str());
		mTransitionTimer = 3000;
		mManualRestart = false;
	}
	if (mManualRestart && mStage == 0)
	{
		gpGameObjectManager->~GameObjectManager();
		gpObjectFactory->LoadLevel("..\\Resources\\Title.json");
		mTransitionTimer = mTransitionTimerLimit;
		mManualRestart = false;
	}

	if (mManualBack && (mStage == 99 || mStage == 666))
	{
		mStage = 0;
		gpGameObjectManager->~GameObjectManager();
		gpObjectFactory->LoadLevel("..\\Resources\\Title.json");
		mTransitionTimer = mTransitionTimerLimit;
		mManualRestart = false;
		mManualOverride = false;
		mManualBack = false;
	}
	
	
	if (mStage == 99 || mStage == 666) {
		return;
	}
	mLastStage = mStage;

	bool clearLevel = false;
	bool playerAlive = false;
	if (mStage > 0 && mStage < 99)
	{
		clearLevel = true;
		for (auto pGO : gpGameObjectManager->mGameObjects) {
			Character* pC = static_cast<Character*>(pGO->GetComponent(TYPE_CHARACTER));
			if (pC == nullptr) continue;
			if (pC->mFriendly == true)
			{
				playerAlive = true;
			}


		}

		for (auto pGO : gpGameObjectManager->mGameObjects) {
			Objective* pO = static_cast<Objective*>(pGO->GetComponent(TYPE_OBJECTIVE));
			if (pO == nullptr) continue;
			if (pO->mCompleted == false)
			{
				clearLevel = false;
			}
		}

		if (!playerAlive) {
			mStage = 666;
			
			return;
		}
	}

	if (mStage < 0 || mStage > 99)
	{
		clearLevel = true;
	}
	

	if (clearLevel || mManualOverride || mManualBack) {

		mTransitionTimer -= gpFRC->GetFrameTime();
		if (mStage == 0 || mStage == 99 || mStage == 666)
			mTransitionTimer = 0;
		if (mStage < 0 && mManualOverride)
			mTransitionTimer = 0;

		for (auto pGO : gpGameObjectManager->mGameObjects) {
			Character* pC = static_cast<Character*>(pGO->GetComponent(TYPE_CHARACTER));
			if (pC == nullptr) continue;
			if (pC->mFriendly == true)
			{
				Transform* pT = static_cast<Transform*>(pGO->GetComponent(TYPE_TRANSFORM));
				Body* pB = static_cast<Body*>(pGO->GetComponent(TYPE_BODY));
				pC->AddTelegraphColor(pT->mPositionX, pT->mPositionY - pB->mHeight / 2.f, 0, M_PI, 1500 * ((float)mTransitionTimer/mTransitionTimerLimit), 1500, 0, 0, 0, 0, .7, 0, 0, 0, .7);
			}
		}

		if (mTransitionTimer <= 0 || mManualOverride || mManualBack) {
			if (mManualBack)
				mStage--;
			else
				mStage++;

			if (mStage < -3)
				mStage++;

			if (mStage <= 9 && mStage >= 1) {
				gpGameObjectManager->~GameObjectManager();
				gpObjectFactory->LoadLevel(("..\\Resources\\StealthLevel" + std::to_string(mStage) + ".json").c_str());
				mTransitionTimer = mTransitionTimerLimit;
				mManualOverride = false;
				mManualBack = false;
				mManualRestart = false;
			}
			else if (mStage == 0)
			{
				gpGameObjectManager->~GameObjectManager();
				gpObjectFactory->LoadLevel("..\\Resources\\Title.json");
				mTransitionTimer = mTransitionTimerLimit;
				mManualOverride = false;
				mManualBack = false;
				mManualRestart = false;
			}
			else if (mStage < 0 )
			{
				if (mManualOverride)
				{
					gpGameObjectManager->~GameObjectManager();
					gpObjectFactory->LoadLevel("..\\Resources\\Opening2.json");
					mTransitionTimer = 3000;
					mManualOverride = false;
					mManualBack = false;
					mManualRestart = false;
					mStage = -1;
				}
				else
				{
					gpGameObjectManager->~GameObjectManager();
					gpObjectFactory->LoadLevel(("..\\Resources\\Opening" + std::to_string(mStage + 3) + ".json").c_str());
					mTransitionTimer = 3000;
					mManualOverride = false;
					mManualBack = false;
					mManualRestart = false;
				}
			}
			else if (mStage > 100 && mStage < 105)
			{
				gpGameObjectManager->~GameObjectManager();
				gpObjectFactory->LoadLevel(("..\\Resources\\Credits" + std::to_string(mStage - 100) + ".json").c_str());
				mTransitionTimer = 2000;
				mManualOverride = false;
				mManualBack = false;
			}
			else if (mStage == 105)
			{
				mStage = 0;
				gpGameObjectManager->~GameObjectManager();
				gpObjectFactory->LoadLevel("..\\Resources\\Title.json");
				mTransitionTimer = mTransitionTimerLimit;
				mManualRestart = false;
				mManualOverride = false;
				mManualBack = false;
			}
			else{
				mStage = 99;
				gpGameObjectManager->~GameObjectManager();
			}
		}


		
		
	}
	/*
	if (gameType == 2 || mStage == 0 || mStage == 99 || mStage == 666) {
		return;
	}

	bool nextLevel = false;
	bool prevLevel = false;
	bool playerAlive = false;
	for (auto pGO : gpGameObjectManager->mGameObjects) {
		Character* pC = static_cast<Character*>(pGO->GetComponent(TYPE_CHARACTER));
		if (pC == nullptr) { continue; }
		if (pC->mFriendly == false) {
			clearLevel = false;
			break;
		}
		else {
			playerAlive = true;
		}
	}

	if (!playerAlive) {
		mStage = 666;
		return;
	}

	if (nextLevel) {
		mStage++;
		if (mStage <= 4) {
			gpObjectFactory->LoadLevel(("..\\Resources\\StealthLevel" + std::to_string(mStage) + ".json").c_str());
		}
		else {
			mStage = 99;
		}
	}

	if (previousLevel)
	{
		if (mStage > 1)
		{
			mStage--;
			gpObjectFactory->LoadLevel(("..\\Resources\\StealthLevel" + std::to_string(mStage) + ".json").c_str());
		}
	}
	*/
}


