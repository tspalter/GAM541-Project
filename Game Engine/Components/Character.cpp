/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Character.cpp
Purpose: For any unit that can be interacted with in combat, handles collision area and health
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne, Adam Rhoades
- End Header --------------------------------------------------------*/


#include "Character.h"
#include "Transform.h"
#include "AudioClip.h"
#include "../GameObject.h"
#include "../EventManager.h"
#include "../GameObjectManager.h"
#include "../ObjectFactory.h"
#include "..\FrameRateController.h"

extern GameObjectManager* gpGameObjectManager;
extern ObjectFactory* gpObjectFactory;
extern FrameRateController* gpFRC;

extern bool DEBUG;

Character::Character() : Component(TYPE_CHARACTER) {
	mHP = mHPMax = 10.0f;
	mShadowOffset = 0;
	mRadius = 16;
	mFriendly = false;
	mIsStunned = false;
	mStunnedDuration = 100;
	mHasHealthBar = false;
	mGodMode = false;
}

Character::~Character() {
	
	for (auto pTele : mTelegraphs)
		delete pTele;
	mTelegraphs.clear();

	for (auto pRect : mRectangles)
		delete pRect;
	mRectangles.clear();
	
}

void Character::Update() {
	if (DEBUG) {
		Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
		if (pT != nullptr) {
			float ang = atan2f(pT->mVelVert, pT->mVelHoriz);
			float len = sqrtf(powf(pT->mVelHoriz, 2) + powf(pT->mVelVert, 2));
			AddRectColor(pT->mPositionX + cosf(ang)*len/2, pT->mPositionY + sin(ang)*len/2, ang, len, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1);
		}
	}

	if (mIsStunned)
	{
		AudioClip* pAC = static_cast<AudioClip*>(mpOwner->GetComponent(TYPE_AUDIOCLIP));
		pAC->PlayOneShot("Dizzy", 0);

		mStunnedDuration -= gpFRC->GetFrameTime();

		if (mStunnedDuration <= 0) {
			mIsStunned = false;
			pAC->SetPlayed("Dizzy", false);
		}
	}

	/*
	if (mHP <= 0) {
		// gpObjectFactory->LoadGameObject("../Resources/Slime.json");
		gpGameObjectManager->DeleteObject(mpOwner);
	}
	*/
}

void Character::Serialize(std::ifstream& InputStream) {
	//
}

void Character::Serialize(rapidjson::GenericArray<false, rapidjson::Value> input) {

	if (input[0].HasMember("hp")) {
		mHP = mHPMax = input[0]["hp"].GetFloat();
	}

	if (input[0].HasMember("hasHealthBar")) {
		mHasHealthBar = input[0]["hasHealthBar"].GetBool();
	}
	
	if (input[0].HasMember("hpmax")) {
		mHPMax = input[0]["hpmax"].GetFloat();
	}

	if (input[0].HasMember("radius")) {
		mRadius = input[0]["radius"].GetInt();
	}
	
	if (input[0].HasMember("friendly")) {
		mFriendly = input[0]["friendly"].GetBool();
	}
	
	if (input[0].HasMember("shadow")) {
		mShadowOffset = input[0]["shadow"].GetInt();
	}

}

void Character::HandleEvent(Event* pEvent) {
	if (pEvent->mType == EventType::PULSE) {
		Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
		if (pT->mPositionY > 400.f) {
			pT->mPositionY -= 400.f;
		}
		else {
			pT->mPositionY += 400.f;
		}
		// pT->mPositionX -= 50.f;
	}
}


/********************/
/********************/
/********************/
void Character::AddTelegraph(float x_, float y_, float ang_, float angwidth_, float in_, float out_, float fill_) {
	Telegraph* tele = new Telegraph(x_, y_, ang_, angwidth_, in_, out_, fill_);
	tele->mColor1[0] = 0.8;
	tele->mColor1[1] = tele->mColor1[2] = 0.2;
	tele->mColor1[3] = 0.5;
	
	tele->mColor2[0] = 0.8;
	tele->mColor2[1] = tele->mColor2[2] = 0.3;
	tele->mColor2[3] = 0.5;
	mTelegraphs.push_back(tele);
}

void Character::AddTelegraphColor(float x_, float y_, float ang_, float angwidth_, float in_, float out_, float fill_, float r1, float g1, float b1, float a1, float r2, float g2, float b2, float a2) {
	Telegraph* tele = new Telegraph(x_, y_, ang_, angwidth_, in_, out_, fill_);
	tele->mColor1[0] = r1;
	tele->mColor1[1] = g1;
	tele->mColor1[2] = b1;
	tele->mColor1[3] = a1;

	tele->mColor2[0] = r2;
	tele->mColor2[1] = g2;
	tele->mColor2[2] = b2;
	tele->mColor2[3] = a2;
	mTelegraphs.push_back(tele);
}

bool Character::CollideCirc(float x_, float y_, float ang_, float angwidth_, float in_, float out_) {
	/*Character* pChar = static_cast<Character*>(pGO->GetComponent(TYPE_CHARACTER));
	if (pChar == nullptr)
		return false;*/

	Transform* pTrans = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
	if (pTrans == nullptr)
		return false;

	float dist = powf(x_ - pTrans->mPositionX, 2) + powf(y_ - pTrans->mPositionY, 2);

	if (dist <= powf(out_ + mRadius, 2) && dist >= powf((in_ - mRadius >= 0.0 ? in_ - mRadius : 0.0), 2)) {
		float ang = atan2f(pTrans->mPositionY - y_, pTrans->mPositionX - x_);
		float angMin = atan2f((pTrans->mPositionY + mRadius * sinf(ang - 3.14159 / 2)) - y_, (pTrans->mPositionX + mRadius * cosf(ang - 3.14159 / 2)) - x_);
		float angMax = atan2f((pTrans->mPositionY + mRadius * sinf(ang + 3.14159 / 2)) - y_, (pTrans->mPositionX + mRadius * cosf(ang + 3.14159 / 2)) - x_);

		while (ang_ > 3.14159)
			ang_ -= 2 * 3.14159;
		while (ang_ < -3.14159)
			ang_ += 2 * 3.14159;

		float hitAngMin = ang_ - angwidth_;
		float hitAngMax = ang_ + angwidth_;
		//
		//while (hitAngMin < -3.14159)
		//	hitAngMin += 2 * 3.14159;
		//while (hitAngMax > 3.14159)
		//	hitAngMax -= 2 * 3.14159;
		//

		if (dist < powf(mRadius, 2) ||
			(
				(angMin < hitAngMax && angMax > hitAngMin)
				|| angMin < hitAngMax - 2 * 3.14159
				|| angMax > hitAngMin + 2 * 3.14159
				|| (angMax < angMin
					&& (
						(angMin - 2 * 3.14159 < hitAngMax && angMax > hitAngMin)
						|| (angMin < hitAngMax && angMax + 2 * 3.14159 > hitAngMin)
						)
					)
				)
			) {
			// pChar->mHP -= 1;
			return true;
		}
	}

	return false;
}

Character::Telegraph::Telegraph(float x_, float y_, float ang_, float angwidth_, float in_, float out_, float fill_) : mX(x_), mY(y_), mAng(ang_), mAngWidth(angwidth_), mIn(in_), mOut(out_), mFill(fill_) {
	mColor1[0] = mColor1[1] = mColor1[2] = mColor1[3] = 1.0f;
	mColor2[0] = mColor2[1] = mColor2[2] = mColor2[3] = 1.0f;
	// mFill = 0.5f;
}

Character::Telegraph::~Telegraph() {
	//
}

/********************/
/********************/
/********************/
void Character::AddSquare(float x_, float y_, float ang_, float length_, float width_, float fill_) {
	TeleRect* tele = new TeleRect(x_, y_, ang_, length_, width_, fill_);
	tele->mColor1[0] = 0.8;
	tele->mColor1[1] = tele->mColor1[2] = 0.2;
	tele->mColor1[3] = 0.5;

	tele->mColor2[0] = 0.8;
	tele->mColor2[1] = tele->mColor2[2] = 0.3;
	tele->mColor2[3] = 0.5;
	mRectangles.push_back(tele);
}

void Character::AddRect(float x_, float y_, float ang_, float length_, float width_, float fill_) {
	TeleRect* tele = new TeleRect(x_ + length_/2.f * cos(ang_), y_ + length_/2.f * sin(ang_), ang_, length_, width_, fill_);
	tele->mColor1[0] = 0.8;
	tele->mColor1[1] = tele->mColor1[2] = 0.2;
	tele->mColor1[3] = 0.5;

	tele->mColor2[0] = 0.8;
	tele->mColor2[1] = tele->mColor2[2] = 0.3;
	tele->mColor2[3] = 0.5;
	mRectangles.push_back(tele);
}


void Character::AddRectColor(float x_, float y_, float ang_, float length_, float width_, float fill_, float r1, float g1, float b1, float a1, float r2, float g2, float b2, float a2) {
	TeleRect* tele = new TeleRect(x_, y_, ang_, length_, width_, fill_);
	tele->mColor1[0] = r1;
	tele->mColor1[1] = g1;
	tele->mColor1[2] = b1;
	tele->mColor1[3] = a1;

	tele->mColor2[0] = r2;
	tele->mColor2[1] = g2;
	tele->mColor2[2] = b2;
	tele->mColor2[3] = a2;
	mRectangles.push_back(tele);
}


Character::TeleRect::TeleRect(float x_, float y_, float ang_, float length_, float width_, float fill_) : mX(x_), mY(y_), mAng(ang_), mLength(length_), mWidth(width_), mFill(fill_) {
	mColor1[0] = mColor1[1] = mColor1[2] = mColor1[3] = 1.0f;
	mColor2[0] = mColor2[1] = mColor2[2] = mColor2[3] = 1.0f;
	// mFill = 0.5f;
}

Character::TeleRect::~TeleRect() {
	//
}
