/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: BossAttack.cpp
Purpose: Boss Attack Component for a boss with Attacks on an interval and no movement.
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Adam Rhoades
- End Header --------------------------------------------------------*/

#include "BossAttack.h"
#include "Transform.h"
#include "Sprite.h"
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

BossAttack::BossAttack() : Component(TYPE_BOSS_ATTACK) {
	mAction = WAIT;
	mTimerLimit = 1000;
	mTimer = mTimerLimit;
	
	
}

BossAttack::~BossAttack() {

}

void BossAttack::Update() {


	Character* pC = static_cast<Character*>(mpOwner->GetComponent(TYPE_CHARACTER));
	Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
	if (!pC->mIsStunned)
	{
		mTimer -= gpFRC->GetFrameTime();



		if (!mSwinging && mTimer <= 50 && mAction == ATTACK)
		{
			mSwinging = true;
		}

	
		if (mAction == WAIT)
		{

		}
		else if (mAction == ATTACK)
		{
			pC->AddTelegraphColor(pT->mPositionX, pT->mPositionY, mAttackAngle, mAttackWidth, 20, mAttackLength, (float)(mAttackDuration - mTimer) / mAttackDuration, .8, .2, .2, .5, .8, .3, .3, .5);

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

					if (pChar->CollideCirc(pT->mPositionX, pT->mPositionY, mAttackAngle, mAttackWidth, 0, mAttackLength)) {
						pChar->mHP -= mDamage * 100;
					}

				}
				mSwinging = false;
			}
		}

		if (mTimer <= 0) {
			mAction++;
			mAction %= 2;
			UpdateTimersAndAngles();
		}



		
	}
	else
	{
		pT->mVelHoriz = 0;
	}
	
	


}

void BossAttack::UpdateTimersAndAngles()
{
	if (mAction == WAIT)
		mTimer = mTimerLimit = mWaitDuration;
	else if (mAction == ATTACK)
		mTimer = mTimerLimit = mAttackDuration;

	mAttackAngle = mAttackAngleMod;

	Character* pC = static_cast<Character*>(mpOwner->GetComponent(TYPE_CHARACTER));
	Sprite* pS = static_cast<Sprite*>(mpOwner->GetComponent(TYPE_SPRITE));
	if (pS->mIsAnimated)
	{
		if (mAction == WAIT) {
			if (!pS->mpSpriteAnimator->mIsIdling)
				pS->mpSpriteAnimator->StartIdling();
		}

	}

}

void BossAttack::Serialize(std::ifstream& InputStream) {

}


void BossAttack::Serialize(rapidjson::GenericArray<false, rapidjson::Value> input) {

	if (input[0].HasMember("startAction")) {
		mAction = input[0]["startAction"].GetInt();
	}

	if (input[0].HasMember("waitDuration")) {
		mWaitDuration = input[0]["waitDuration"].GetInt();
	}

	if (input[0].HasMember("attackDuration")) {
		mAttackDuration = input[0]["attackDuration"].GetInt();
	}

	if (input[0].HasMember("attackWidth")) {
		mAttackWidth = input[0]["attackWidth"].GetFloat();
	}

	if (input[0].HasMember("attackAngleMod")) {
		mAttackAngleMod = input[0]["attackAngleMod"].GetFloat();
	}

	if (input[0].HasMember("attackLength")) {
		mAttackLength = input[0]["attackLength"].GetFloat();
	}

	if (input[0].HasMember("damage")) {
		mDamage = input[0]["damage"].GetFloat();
	}

	UpdateTimersAndAngles();

}

void BossAttack::HandleEvent(Event* pEvent) {
	if (pEvent->mType == EventType::PLAYER_HIT) {

	}
}
