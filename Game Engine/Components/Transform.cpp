/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Transform.cpp
Purpose: Contains all transformation/translation information for an object
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne, Adam Rhoades
- End Header --------------------------------------------------------*/

#include "Transform.h"
#include "Controller.h"
#include "Sprite.h"
#include "..\GameObject.h"
#include "../FrameRateController.h"

extern FrameRateController* gpFRC;

Transform::Transform() : Component(TYPE_TRANSFORM) {
	mPositionX = mPositionY = 0.0f;
	mVelHoriz = mVelVert = 0.0f;
	mKnockbackHoriz = mKnockbackVert = 0.0f;
	mSpriteOffsetX = mSpriteOffsetY = 0.0f;
	mAngle = 0.0f;
	mWidth = mHeight = 64.0f;
}

Transform::~Transform() {
	//
}


void Transform::Update() {
	mPositionX += mVelHoriz * gpFRC->GetDeltaTime();
	mPositionY += mVelVert * gpFRC->GetDeltaTime();

	mVelHoriz += mKnockbackHoriz;
	mVelVert += mKnockbackVert;
	mKnockbackHoriz = mKnockbackVert = 0.0f;
	
	Sprite* pS = static_cast<Sprite*>(mpOwner->GetComponent(TYPE_SPRITE));
	Controller* pC = static_cast<Controller*>(mpOwner->GetComponent(TYPE_PLAYER_CONTROLLER));
	
	if (pS->mIsAnimated)
	{
		if (pS->mpSpriteAnimator->mCanDash && abs(mVelHoriz) > pC->mMaxSpeed)
		{
			// If not already dashing, and not attacking (attacking takes priority)
			if (!pS->mpSpriteAnimator->mIsDashing && !pS->mpSpriteAnimator->mIsAttacking)
				pS->mpSpriteAnimator->StartDashing();
		}
		else
		{
			pS->mpSpriteAnimator->mIsDashing = false;

			if (mVelVert < -0.5)
			{
				//If not already jumping, and not attacking or dashing (attacking or dashing takes priority)
				if (!pS->mpSpriteAnimator->mIsJumping && !pS->mpSpriteAnimator->mIsAttacking && !pS->mpSpriteAnimator->mIsDashing && pS->mpSpriteAnimator->mCanJump)
					pS->mpSpriteAnimator->StartJumping();
			}
			else if (mVelVert > 0.5)
			{
				//TODO: Start falling
				//If not already falling, and not attacking or dashing (attacking or dashing takes priority)
				if (!pS->mpSpriteAnimator->mIsFalling && !pS->mpSpriteAnimator->mIsAttacking && !pS->mpSpriteAnimator->mIsDashing && pS->mpSpriteAnimator->mCanFall)
					pS->mpSpriteAnimator->StartFalling();

			}
			else if (abs(mVelHoriz) > 0)
			{
				//If not already running, and not attacking or dashing (attacking or dashing takes priority)
				if (!pS->mpSpriteAnimator->mIsRunning && !pS->mpSpriteAnimator->mIsAttacking && !pS->mpSpriteAnimator->mIsDashing && pS->mpSpriteAnimator->mCanRun)
					pS->mpSpriteAnimator->StartRunning();
			}
			else
			{
				//If not already idling, and not attacking or dashing (attacking or dashing takes priority)
				if (!pS->mpSpriteAnimator->mIsIdling && !pS->mpSpriteAnimator->mIsAttacking && !pS->mpSpriteAnimator->mIsDashing && pS->mpSpriteAnimator->mCanIdle)
					pS->mpSpriteAnimator->StartIdling();

			}
		}
		
	}
	
	
}

void Transform::Serialize(std::ifstream& InputStream) {
	InputStream >> mPositionX;
	InputStream >> mPositionY;
}

void Transform::Serialize(rapidjson::GenericArray<false, rapidjson::Value> input) {
	if (input[0].HasMember("x")) {
		mPositionX = input[0]["x"].GetFloat();
	}

	if (input[0].HasMember("y")) {
		mPositionY = input[0]["y"].GetFloat();
	}

	if (input[0].HasMember("width")) {
		mWidth = input[0]["width"].GetFloat();
	}

	if (input[0].HasMember("height")) {
		mHeight = input[0]["height"].GetFloat();
	}
	
	if (input[0].HasMember("angle")) {
		mAngle = input[0]["angle"].GetFloat();
	}

	if (input[0].HasMember("xoffset")) {
		mSpriteOffsetX = input[0]["xoffset"].GetFloat();
	}

	if (input[0].HasMember("yoffset")) {
		mSpriteOffsetY = input[0]["yoffset"].GetFloat();
	}

	if (input[0].HasMember("velhoriz")) {
		mVelHoriz = input[0]["velhoriz"].GetFloat();
	}

	// mPositionX = mPositionY = 5.0f;
}
