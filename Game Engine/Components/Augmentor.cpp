/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Augmentor.cpp
Purpose: Handles the augmentor's radiation telegraphs
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_3
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: November 13, 2020
- End Header --------------------------------------------------------*/

#include "Augmentor.h"
#include "Character.h"
#include "Transform.h"
#include "..\FrameRateController.h"
#include "..\GameObjectManager.h"
#include "..\GameObject.h"

#include "../CollisionManager.h"
#include "../EventManager.h"

extern FrameRateController* gpFRC;
extern GameObjectManager* gpGameObjectManager;
extern CollisionManager* gpCollisionManager;
extern EventManager* gpEventManager;

Augmentor::Augmentor() : Component(TYPE_AUGMENTOR) {
	mAng = 0.0f;
	mInner = 40;
	mSlice = 80;
	mDamageTimer = mDamageTick = 0.2f;
}

Augmentor::~Augmentor() {
	//
}

void Augmentor::Update() {
	Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
	Character* pC = static_cast<Character*>(mpOwner->GetComponent(TYPE_CHARACTER));

	mAng += gpFRC->GetDeltaTime() * 0.4;

	/*
	if (mAng >= 10.f / 3.f * 3.14159) {
		mAng -= 10.f / 3.f * 3.14159;

		mPulse = true;
		Event* pulse = new Event(EventType::PULSE);
		gpEventManager->BroadcastEvent(pulse);
	}

	if (mPulse) {
		pC->AddTelegraph(pT->mPositionX, pT->mPositionY, 0, 10, 0, 2000.f, mAng/0.3f);
		if (mAng > 0.3f) {
			mPulse = false;
		}
	}
	*/


	mDamageTimer -= gpFRC->GetDeltaTime();
	bool damageBool = false;
	while (mDamageTimer <= 0) {
		damageBool = true;
		mDamageTimer += mDamageTick;
	}

	pC->AddTelegraph(pT->mPositionX, pT->mPositionY, 0, 10, 0, mInner, 1.0);

	if (damageBool) {
		for (auto pGO : gpGameObjectManager->mGameObjects) {
			Character* pChar = static_cast<Character*>(pGO->GetComponent(TYPE_CHARACTER));
			if (pChar == nullptr || pChar->mFriendly == false)
				continue;

			if (gpCollisionManager->CollideCirc(pC, pChar, 0, 10, 0, mInner)) {
				pChar->mHP -= 1.0f;
			}
		}
	}


	for (int i = 0; i < 3; i++) {
		pC->AddTelegraph(pT->mPositionX, pT->mPositionY, mAng + 3.14159/3*2*i, 3.14159/6, mInner, mInner+mSlice, 1.0);
		
		if (damageBool) {
			for (auto pGO : gpGameObjectManager->mGameObjects) {
				Character* pChar = static_cast<Character*>(pGO->GetComponent(TYPE_CHARACTER));
				if (pChar == nullptr || pChar->mFriendly == false)
					continue;

				if (gpCollisionManager->CollideCirc(pC, pChar, mAng + 3.14159 / 3 * 2 * i, 3.14159 / 6, mInner, mInner + mSlice)) {
					pChar->mHP -= 1.0f;
				}
			}
		}
	}

	for (int i = 0; i < 3; i++) {
		pC->AddTelegraph(pT->mPositionX, pT->mPositionY, -mAng + 3.14159/3*2*i, 3.1415/6, mInner+mSlice, mInner+mSlice*2, 1.0);
		
		if (damageBool) {
			for (auto pGO : gpGameObjectManager->mGameObjects) {
				Character* pChar = static_cast<Character*>(pGO->GetComponent(TYPE_CHARACTER));
				if (pChar == nullptr || pChar->mFriendly == false)
					continue;

				if (gpCollisionManager->CollideCirc(pC, pChar, -mAng + 3.14159 / 3 * 2 * i, 3.14159 / 6, mInner + mSlice, mInner + mSlice * 2)) {
					pChar->mHP -= 1.0f;
				}
			}
		}
	}
	
	for (int i = 0; i < 3; i++) {
		pC->AddTelegraph(pT->mPositionX, pT->mPositionY, mAng*0.6 + 3.14159/3*2*i, 3.1415/6, mInner+mSlice*2, mInner+mSlice*3, 1.0);

		if (damageBool) {
			for (auto pGO : gpGameObjectManager->mGameObjects) {
				Character* pChar = static_cast<Character*>(pGO->GetComponent(TYPE_CHARACTER));
				if (pChar == nullptr || pChar->mFriendly == false)
					continue;

				if (gpCollisionManager->CollideCirc(pC, pChar, mAng * 0.6 + 3.14159 / 3 * 2 * i, 3.1415 / 6, mInner + mSlice * 2, mInner + mSlice * 3)) {
					pChar->mHP -= 1.0f;
				}
				
			}
		}
	}
	
	for (int i = 0; i < 3; i++) {
		pC->AddTelegraph(pT->mPositionX, pT->mPositionY, -mAng*0.6 + 3.14159/3*2*i, 3.1415/6, mInner+mSlice*3, mInner+mSlice*4, 1.0);

		if (damageBool) {
			for (auto pGO : gpGameObjectManager->mGameObjects) {
				Character* pChar = static_cast<Character*>(pGO->GetComponent(TYPE_CHARACTER));
				if (pChar == nullptr || pChar->mFriendly == false)
					continue;

				if (gpCollisionManager->CollideCirc(pC, pChar, -mAng * 0.6 + 3.14159 / 3 * 2 * i, 3.1415 / 6, mInner + mSlice * 3, mInner + mSlice * 4)) {
					pChar->mHP -= 1.0f;
				}
			}
		}
	}
}

void Augmentor::Serialize(rapidjson::GenericArray<false, rapidjson::Value>) {
	//
}
