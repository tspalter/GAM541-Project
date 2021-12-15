/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Body.cpp
Purpose: Handles Physics
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne, Adam Rhoades
- End Header --------------------------------------------------------*/

#include "Body.h"
#include "Character.h"
#include "Transform.h"
#include "../GameObject.h"
#include "../EventManager.h"
#include "../GameObjectManager.h"
#include "../ObjectFactory.h"

extern GameObjectManager* gpGameObjectManager;
extern ObjectFactory* gpObjectFactory;

extern bool DEBUG;

Body::Body() : Component(TYPE_BODY) {
	mWidth = mHeight = 20.0f;
	mWall = false;
	mSemisolid = false;
	mBounce = false;
	mFriendly = false;
}

Body::~Body() {
	//
}

void Body::Update() {
	if (DEBUG) {
		Character* pC = static_cast<Character*>(mpOwner->GetComponent(TYPE_CHARACTER));
		Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
		if (pC != nullptr && pT != nullptr) {
			pC->AddRect(pT->mPositionX, pT->mPositionY, -M_PI / 2, mHeight, mWidth, 0);
		}
	}
}

void Body::Serialize(rapidjson::GenericArray<false, rapidjson::Value> input) {
	if (input[0].HasMember("height")) {
		mHeight = input[0]["height"].GetFloat();
	}

	if (input[0].HasMember("width")) {
		mWidth = input[0]["width"].GetFloat();
	}

	if (input[0].HasMember("wall")) {
		mWall = input[0]["wall"].GetBool();
	}

	if (input[0].HasMember("bounce")) {
		mBounce = input[0]["bounce"].GetBool();
	}

	if (input[0].HasMember("semi")) {
		mSemisolid = input[0]["semi"].GetBool();
	}

	if (input[0].HasMember("friendly")) {
		mFriendly = input[0]["friendly"].GetBool();
	}
}

