/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Slime.cpp
Purpose: Handles movement patterns for a "slime" object
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_2
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 15, 2020
- End Header --------------------------------------------------------*/

#include "Slime.h"
#include "..\FrameRateController.h"
#include "..\GameObjectManager.h"
#include "..\GameObject.h"
#include "../EventManager.h"
#include "../CollisionManager.h"
#include "../EventManager.h"
#include "Component.h"
#include "Transform.h"
#include "Character.h"

extern FrameRateController* gpFRC;
extern GameObjectManager* gpGameObjectManager;
extern EventManager* gpEventManager;

extern bool DEBUG;

Slime::Slime() : Component(TYPE_SLIME) {
	// mVert = 0;
	// mHoriz = 0;
	mJumpHeight = 50;

	mJumpDelay = 800;
	mJumpDuration = 1200;
	mTimer = mJumpDelay;
	mIsJumping = false;
	mJumpDist = 200.0f;
	mJumpVel = mJumpDist;

	mDamageRadius = 50.0f;
	mDamageKnockback = 200.0f;
	mDamageTime = 250;
}

Slime::~Slime() {
	//
}

void Slime::Update() {
	mTimer -= gpFRC->GetFrameTime();
	Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
	Character* pC = static_cast<Character*>(mpOwner->GetComponent(TYPE_CHARACTER));

	if (mTimer <= 0) {
		if (mIsJumping) {
			mTimer = mJumpDelay;
			pT->mVelHoriz = 0;
			pT->mVelVert = 0;
			pT->mSpriteOffsetY = 0.0f;
			pT->mAngle = 0.0f;
			for (auto pGO : gpGameObjectManager->mGameObjects) {
				Character* pChar = static_cast<Character*>(pGO->GetComponent(TYPE_CHARACTER));
				if (pChar == nullptr || pChar->mFriendly == false)
					continue;

				Transform* pTrans = static_cast<Transform*>(pGO->GetComponent(TYPE_TRANSFORM));
				if (pow(pT->mPositionX - pTrans->mPositionX, 2) + pow(pT->mPositionY - pTrans->mPositionY, 2) <= pow(pChar->mRadius + mDamageRadius, 2)) {
					pChar->mHP -= 1.0f;
					float ang = atan2f(pTrans->mPositionY - pT->mPositionY, pTrans->mPositionX - pT->mPositionX);
					pTrans->mVelHoriz += cosf(ang) * mDamageKnockback;
					pTrans->mVelVert += sinf(ang) * mDamageKnockback;

					CollideEvent* newHit = new CollideEvent(pC, pChar);
					gpEventManager->AddTimedEvent(newHit);
				}
			}
		}
		else {
			mTimer = mJumpDuration;

			GameObject* pTarget = nullptr;
			for (auto pGO : gpGameObjectManager->mGameObjects) {
				if (pGO->GetComponent(TYPE_PLAYER_CONTROLLER) != nullptr) {
					pTarget = pGO;
					break;
				}
			}

			if (nullptr == pTarget)
				return;

			Transform* pT2 = static_cast<Transform*>(pTarget->GetComponent(TYPE_TRANSFORM));

			float ang = atan2f(pT2->mPositionY - pT->mPositionY, pT2->mPositionX - pT->mPositionX);
			float dist = pow(pT2->mPositionX - pT->mPositionX, 2) + pow(pT2->mPositionY - pT->mPositionY, 2);

			if (dist >= mJumpDist * mJumpDist) {
				pT->mVelHoriz = cosf(ang) * mJumpVel;
				pT->mVelVert = sinf(ang) * mJumpVel;
			}
			else {
				dist = sqrtf(dist);
				pT->mVelHoriz = cosf(ang) * dist / mJumpDuration * 1000;
				pT->mVelVert = sinf(ang) * dist / mJumpDuration * 1000;
			}

			// pT->mVelHoriz = cosf(ang) * mJumpDist;
			// pT->mVelVert = sinf(ang) * mJumpDist;
		}
		mIsJumping = !mIsJumping;
	}

	if (mIsJumping) {
		pT->mSpriteOffsetY = -sin(3.14159 * mTimer / mJumpDuration) * mJumpHeight;
		pT->mAngle = 6.28318 * mTimer / mJumpDuration;
	}
	else if (mTimer > mJumpDelay - mDamageTime) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		pC->AddTelegraph(pT->mPositionX, pT->mPositionY, 0, 10, 0, mDamageRadius, -(float)(mTimer-mJumpDelay) / mDamageTime);
	}

	// pT->mPositionX += pT->mVelHoriz * gpFRC->GetDeltaTime();
	// pT->mPositionY += pT->mVelVert * gpFRC->GetDeltaTime();

	if (DEBUG) {
		pC->AddRectColor(pT->mPositionX + 32, pT->mPositionY, 3.14159 / 2, 64, 8, (float)mTimer / mJumpDuration, .4, .4, .8, 1, .2, .2, .8, 1);
	}
}

void Slime::Serialize(std::ifstream& InputStream) {
	InputStream >> mJumpDelay;
	InputStream >> mJumpDuration;
	InputStream >> mJumpDist;

	mTimer = mJumpDelay;
	mJumpVel = mJumpDist / mJumpDuration * 1000;
}

void Slime::Serialize(rapidjson::GenericArray<false, rapidjson::Value> input) {
	mJumpDelay = input[0].GetInt();
	mJumpDuration = input[1].GetInt();
	mJumpDist = input[2].GetFloat();
	mJumpHeight = input[3].GetFloat();

	mTimer = mJumpDelay;
	mJumpVel = mJumpDist / mJumpDuration * 1000;

	gpEventManager->Subscribe(EventType::PLAYER_HIT, mpOwner);
}

void Slime::HandleEvent(Event* pEvent) {
	if (pEvent->mType == EventType::PLAYER_HIT) {
		Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
		pT->mWidth += 10;
		pT->mHeight += 10;
	}
}
