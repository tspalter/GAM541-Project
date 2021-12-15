/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: LeftRight.cpp
Purpose: Basic AI for guards to patrol, and attack 3 times during their patrol duration. Fully customizable in json.
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Adam Rhoades, Dylan Washburne
- End Header --------------------------------------------------------*/

#include "LeftRight.h"
#include "Transform.h"
#include "Sprite.h"
#include "Character.h"
#include "..\GameObject.h"
#include "..\FrameRateController.h"
#include "../EventManager.h"
#include "../CollisionManager.h"
#include "../GameObjectManager.h"
#include "AudioClip.h"


extern GameObjectManager* gpGameObjectManager;
extern CollisionManager* gpCollisionManager;
extern FrameRateController* gpFRC;
extern EventManager* gpEventManager;

LeftRight::LeftRight() : Component(TYPE_LEFT_RIGHT) {
	mAction = WALK_RIGHT;
	numActions = 4;
	mTimerLimit = 1000;
	mTimer = mSwingTimer = mTimerLimit;
	MiddleAttack = false;
	
	
}

LeftRight::~LeftRight() {

}

void LeftRight::Update() {


	Character* pC = static_cast<Character*>(mpOwner->GetComponent(TYPE_CHARACTER));
	Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
	if (!pC->mIsStunned)
	{
		mTimer -= gpFRC->GetFrameTime();

		if (!mSwinging && mSwingTimer <= 10 && (MiddleAttack || mAction == WAIT_RIGHT || mAction == WAIT_LEFT))
		{
			mSwingTimer = 100000000000;
			mSwinging = true;
		}

		if (!MiddleAttack && mAction == WALK_RIGHT || mAction == WALK_LEFT)
		{

			if (mTimer <= (mTimerLimit / 2) + (mSwingDuration / 2) && mTimer >= (mTimerLimit / 2) - (mSwingDuration / 2))
			{
				if (!MiddleAttack)
				{
					mSwingTimer = mSwingDuration;
					MiddleAttack = true;
				}
			}
			else
			{
				Sprite* pS = static_cast<Sprite*>(mpOwner->GetComponent(TYPE_SPRITE));
				if (pS->mIsAnimated)
				{
					if (!pS->mpSpriteAnimator->mIsRunning)
						pS->mpSpriteAnimator->StartRunning();
				}

			}
		}

		

		
		

	
		if (mAction == WALK_LEFT)
			pT->mVelHoriz = -1 * mSpeed;
		else if (mAction == WALK_RIGHT)
			pT->mVelHoriz = 1 * mSpeed;
		else
			pT->mVelHoriz = 0;


		//pC->AddTelegraphColor(pT->mPositionX, pT->mPositionY, 0, 10, 0, 150, 0.f, .2, .8, .2, .5, .2, .8, .2, .5);
		if (mAction == WAIT_LEFT || mAction == WAIT_RIGHT || MiddleAttack)
		{
			mSwingTimer -= gpFRC->GetFrameTime();
			pC->AddTelegraphColor(pT->mPositionX, pT->mPositionY-16.f, mAttackAngle, mAttackWidth, 20, mAttackLength, (float)(mSwingDuration - mSwingTimer)/mSwingDuration, .8, .2, .2, .5, .8, .3, .3, .5);

			Sprite* pS = static_cast<Sprite*>(mpOwner->GetComponent(TYPE_SPRITE));
			if (pS->mIsAnimated)
			{
				if (!pS->mpSpriteAnimator->mIsAttacking)
					pS->mpSpriteAnimator->StartAttacking();
				

			}

			if (mSwinging)
			{
				for (auto pObject : gpGameObjectManager->mGameObjects) {
					Character* pChar = static_cast<Character*>(pObject->GetComponent(TYPE_CHARACTER));
					if (pChar == nullptr || pChar->mFriendly == false)
						continue;

					if (pChar->CollideCirc(pT->mPositionX, pT->mPositionY-16.f, mAttackAngle, mAttackWidth, 0, mAttackLength)) {
						pChar->mHP -= mDamage * 100;
					}

				}
				MiddleAttack = false;
				mSwinging = false;
				mSwingTimer = 100000000000;
			}
			
		}

		if (mTimer <= 0) {
			mAction++;
			mAction %= 4;
			UpdateTimersAndAngles();
		}
		
	}
	else
	{
		Sprite* pS = static_cast<Sprite*>(mpOwner->GetComponent(TYPE_SPRITE));
		pT->mVelHoriz = 0;
		if(!pS->mpSpriteAnimator->mIsIdling)
			pS->mpSpriteAnimator->StartIdling();
	}
	
	


}

void LeftRight::UpdateTimersAndAngles()
{
	if (mAction == WALK_LEFT || mAction == WALK_RIGHT)
		mTimer = mTimerLimit = mMoveDuration;
	else if (mAction == WAIT_RIGHT || mAction == WAIT_LEFT)
		mTimer = mTimerLimit = mSwingTimer = mSwingDuration;
	if (mAction == WALK_LEFT || mAction == WAIT_LEFT)
		mAttackAngle = M_PI - mAttackAngleMod;
	else
		mAttackAngle = mAttackAngleMod;

	Character* pC = static_cast<Character*>(mpOwner->GetComponent(TYPE_CHARACTER));
	Sprite* pS = static_cast<Sprite*>(mpOwner->GetComponent(TYPE_SPRITE));
	if (pS->mIsAnimated)
	{
		if (mAction == WALK_LEFT || mAction == WALK_RIGHT) {
			if (!pS->mpSpriteAnimator->mIsRunning)
				pS->mpSpriteAnimator->StartRunning();
		}
		else
		{
			//if (!pS->mpSpriteAnimator->mIsIdling)
				//pS->mpSpriteAnimator->StartIdling();
		}

	}
		

}

void LeftRight::Serialize(std::ifstream& InputStream) {

}


void LeftRight::Serialize(rapidjson::GenericArray<false, rapidjson::Value> input) {

	if (input[0].HasMember("startAction")) {
		mAction = input[0]["startAction"].GetInt();
	}

	if (input[0].HasMember("moveDuration")) {
		mMoveDuration = input[0]["moveDuration"].GetInt();
	}

	if (input[0].HasMember("waitDuration")) {
		mSwingDuration = input[0]["waitDuration"].GetInt();
	}

	if (input[0].HasMember("speed")) {
		mSpeed = input[0]["speed"].GetInt();
	}

	if (input[0].HasMember("visionWidth")) {
		mAttackWidth = input[0]["visionWidth"].GetFloat();
	}

	if (input[0].HasMember("visionAngleMod")) {
		mAttackAngleMod = input[0]["visionAngleMod"].GetFloat();
	}

	if (input[0].HasMember("visionLength")) {
		mAttackLength = input[0]["visionLength"].GetFloat();
	}

	if (input[0].HasMember("damage")) {
		mDamage = input[0]["damage"].GetFloat();
	}

	UpdateTimersAndAngles();

}

void LeftRight::HandleEvent(Event* pEvent) {
	if (pEvent->mType == EventType::PLAYER_HIT) {
		//Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
		//pT->mWidth += 10;
		//pT->mHeight += 10;
	}
}
