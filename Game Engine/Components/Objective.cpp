/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Objective.h
Purpose: Objective Component. Used for keys and Bosses that need to be defeated before leaving a room.
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Adam Rhoades
- End Header --------------------------------------------------------*/

#include "Objective.h"
#include "..\GameObjectManager.h"
#include "..\GameObject.h"
#include "../EventManager.h"
#include "../CollisionManager.h"
#include "../EventManager.h"
#include "..\FrameRateController.h"
#include "Component.h"
#include "Transform.h"
#include "Character.h"
#include "AudioClip.h"

extern GameObjectManager* gpGameObjectManager;
extern EventManager* gpEventManager;
extern FrameRateController* gpFRC;

extern bool DEBUG;

Objective::Objective() : Component(TYPE_OBJECTIVE) {
	// mVert = 0;
	// mHoriz = 0;
	mRadius = 10.0f;
	mCompleted = false;
	mIsAudioPlayed = false;
	mDissapearDuration = 700;
}

Objective::~Objective() {
	//
}

void Objective::Update() {
	Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
	Character* pC = static_cast<Character*>(mpOwner->GetComponent(TYPE_CHARACTER));
	AudioClip* pAC = static_cast<AudioClip*>(mpOwner->GetComponent(TYPE_AUDIOCLIP));

	/*
	pT->mVelHoriz = 0;
	pT->mVelVert = 0;
	pT->mSpriteOffsetY = 0.0f;
	pT->mAngle = 0.0f;
	*/
	for (auto pGO : gpGameObjectManager->mGameObjects) {
		Character* pChar = static_cast<Character*>(pGO->GetComponent(TYPE_CHARACTER));
		if (pChar == nullptr || pChar->mFriendly == false)
			continue;

		Transform* pTrans = static_cast<Transform*>(pGO->GetComponent(TYPE_TRANSFORM));
		if (pow(pT->mPositionX - pTrans->mPositionX, 2) + pow(pT->mPositionY - pTrans->mPositionY, 2) <= pow(pChar->mRadius + mRadius, 2)) {


			if (mObjectiveType == KEY)
			{
				mCompleted = true;
				if (!mIsAudioPlayed)
				{
					pAC->PlayOneShot("Key");
					mIsAudioPlayed = true;
				}
			}
			else if (mObjectiveType == EXIT)
			{
				// Check for all other objectives completed
				bool bObjectivesCompleted = true;
				for (auto pGO : gpGameObjectManager->mGameObjects) {
					Objective* pO = static_cast<Objective*>(pGO->GetComponent(TYPE_OBJECTIVE));
					if (pO == nullptr) continue;
					if (pO->mCompleted == false && pO->mObjectiveType != EXIT)
					{
						bObjectivesCompleted = false;
					}
				}
				if (bObjectivesCompleted)
				{
					mCompleted = true;
					if (!mIsAudioPlayed)
					{
						pAC->PlayOneShot("DoorOpen");
						mIsAudioPlayed = true;
					}
				}
			}
		}
	}

	if (mCompleted)
	{
		pT->mVelHoriz = .01f;

	}

	if (mCompleted && mObjectiveType == KEY)
	{
		mDissapearDuration -= gpFRC->GetFrameTime();

		if (mDissapearDuration <= 0) {
			gpGameObjectManager->DeleteObject(mpOwner);
		}
	}

	// pT->mPositionX += pT->mVelHoriz * gpFRC->GetDeltaTime();
	// pT->mPositionY += pT->mVelVert * gpFRC->GetDeltaTime();

	if (DEBUG) {
		pC->AddRectColor(pT->mPositionX + 32, pT->mPositionY, 3.14159 / 2, 64, 8, 1, .4, .4, .8, 1, .2, .2, .8, 1);
	}
}

void Objective::Serialize(std::ifstream& InputStream) {
	InputStream >> mRadius;
	mCompleted = false;

}

void Objective::Serialize(rapidjson::GenericArray<false, rapidjson::Value> input) {

	if (input[0].HasMember("radius")) {
		mRadius = input[0]["radius"].GetInt();
	}

	if (input[0].HasMember("objectiveType")) {
		mObjectiveType = input[0]["objectiveType"].GetInt();
	}

	gpEventManager->Subscribe(EventType::PLAYER_HIT, mpOwner);

	mCompleted = false;
}

void Objective::HandleEvent(Event* pEvent) {
}
