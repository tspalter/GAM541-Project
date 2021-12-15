/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Eye.cpp
Purpose: Boss Attack Component for a Boss that looks around, killing the player if it sees it.
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Thomas Spalter
- End Header --------------------------------------------------------*/


# define PI           3.14159265358979323846  /* pi */

#include "Eye.h"
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

Eye::Eye() : Component(TYPE_EYE) {

	mAttackWidth = .15f;
	mAttackLength = 400.f;
	mDamage = 1.f;


}

Eye::~Eye() {

}

void Eye::Update() {


	Character* pC = static_cast<Character*>(mpOwner->GetComponent(TYPE_CHARACTER));
	Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
	Sprite* pS = static_cast<Sprite*>(mpOwner->GetComponent(TYPE_SPRITE));


	mTimer = pS->mpSpriteAnimator->mTimer;

	float angle = -PI/2 + ((pS->mpSpriteAnimator->mCurrentIndex % 16) * (PI / 8));
	pC->AddTelegraphColor(pT->mPositionX, pT->mPositionY - 16.f, angle, mAttackWidth, 20, mAttackLength, (float)(mTimerLimit - mTimer) / mTimerLimit, .8, .2, .2, .5, .8, .3, .3, .5);
	
	for (auto pObject : gpGameObjectManager->mGameObjects) {
		Character* pChar = static_cast<Character*>(pObject->GetComponent(TYPE_CHARACTER));
		if (pChar == nullptr || pChar->mFriendly == false)
			continue;

		if (pChar->CollideCirc(pT->mPositionX, pT->mPositionY - 16.f, angle, mAttackWidth, 0, mAttackLength)) {
			pChar->mHP -= mDamage * 100;
		}

	}
	


}

void Eye::Serialize(std::ifstream& InputStream) {

}


void Eye::Serialize(rapidjson::GenericArray<false, rapidjson::Value> input) {


	if (input[0].HasMember("attackWidth")) {
		mAttackWidth = input[0]["attackWidth"].GetFloat();
	}

	if (input[0].HasMember("attackLength")) {
		mAttackLength = input[0]["attackLength"].GetFloat();
	}

	if (input[0].HasMember("damage")) {
		mDamage = input[0]["damage"].GetFloat();
	}

}

void Eye::HandleEvent(Event* pEvent) {
	if (pEvent->mType == EventType::PLAYER_HIT) {

	}
}
