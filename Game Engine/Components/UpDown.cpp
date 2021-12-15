/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: UpDown.cpp
Purpose: Extremely simple controller to demonstrate NPC behaviour
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_2
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 15, 2020
- End Header --------------------------------------------------------*/

#include "UpDown.h"
#include "Transform.h"
#include "Character.h"
#include "..\GameObject.h"
#include "..\FrameRateController.h"
#include "../EventManager.h"
#include "../CollisionManager.h"
#include "../GameObjectManager.h"

extern GameObjectManager* gpGameObjectManager;
extern CollisionManager* gpCollisionManager;
extern FrameRateController* gpFRC;
extern EventManager* gpEventManager;

UpDown::UpDown() : Component(TYPE_UP_DOWN) {
	mGoingUp = false;
	mTimerLimit = 1000;
	mTimer = mTimerLimit;
	
}

UpDown::~UpDown() {
	//
}

void UpDown::Update() {
	mTimer -= gpFRC->GetFrameTime();

	if (mTimer <= 0) {
		mGoingUp = !mGoingUp;
		mTimer = mTimerLimit;
	}

	Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
	// pT->mPositionY += (mGoingUp ? -1 : 1) * 100.0f * gpFRC->GetDeltaTime();

	Character* pC = static_cast<Character*>(mpOwner->GetComponent(TYPE_CHARACTER));
	pC->AddTelegraphColor(pT->mPositionX, pT->mPositionY, 0, 10, 0, 150, 0.f, .2, .8, .2, .5, .2, .8, .2, .5);

	for (auto pGO : gpGameObjectManager->mGameObjects) {
		Character* pChar = static_cast<Character*>(pGO->GetComponent(TYPE_CHARACTER));
		Transform* pTrans = static_cast<Transform*>(pGO->GetComponent(TYPE_TRANSFORM));

		if (pChar == nullptr) {
			continue;
		}

		float dist = powf(pTrans->mPositionY - pT->mPositionY, 2) + powf(pTrans->mPositionX - pT->mPositionX, 2);
		if ( powf(pTrans->mPositionY - pT->mPositionY, 2) + powf(pTrans->mPositionX - pT->mPositionX, 2) <= powf(150.f + pChar->mRadius, 2) ) {
			Event* shove = new Event(EventType::SHOVE);
			shove->mChars[0] = pC;
			shove->mChars[1] = pChar;
			shove->mDamage = (1.f - powf(dist / powf(150.f + pChar->mRadius, 2), 2)) * 2000.f;
			gpEventManager->BroadcastEvent(shove);
		}
	}

}

void UpDown::Serialize(std::ifstream& InputStream) {
	int goingUp;

	InputStream >> goingUp;
	InputStream >> mTimerLimit;
	
	mGoingUp = (bool)goingUp;
	mTimer = mTimerLimit;
}

void UpDown::Serialize(rapidjson::GenericArray<false, rapidjson::Value> input) {
	mGoingUp = input[0].GetBool();
	mTimer = mTimerLimit = input[1].GetInt();

	gpEventManager->Subscribe(EventType::PLAYER_HIT, mpOwner);
}

void UpDown::HandleEvent(Event* pEvent) {
	if (pEvent->mType == EventType::PLAYER_HIT) {
		Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
		pT->mWidth += 10;
		pT->mHeight += 10;
	}
}
