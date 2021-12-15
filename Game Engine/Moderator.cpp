/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Moderator.cpp
Purpose:  Handles switching levels for Original fighting game.
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne
- End Header --------------------------------------------------------*/

#include "Moderator.h"
#include <string>

#include "GameObject.h"
#include "Components/Component.h"
#include "Components/Character.h"

#include "ResourceManager.h"
#include "GameObjectManager.h"
#include "ObjectFactory.h"

extern ResourceManager* gpResourceManager;
extern GameObjectManager* gpGameObjectManager;
extern ObjectFactory* gpObjectFactory;

extern int gGameType;

Moderator::Moderator() {
	mStage = 0;
	mTimer = 0.0f;
	mTransition = true;
}

Moderator::~Moderator() {
	//
}

void Moderator::Update() {
	if (gGameType != 1 || mStage == 0 || mStage == 99 || mStage == 666) {
		return;
	}

	bool clearLevel = true;
	bool playerAlive = false;
	for (auto pGO : gpGameObjectManager->mGameObjects) {
		Character* pC = static_cast<Character*>(pGO->GetComponent(TYPE_CHARACTER));
		if (pC == nullptr) { continue; }
		if (pC->mFriendly == false) {
			clearLevel = false;
			break;
		}
		else {
			playerAlive = true;
		}
	}

	if (!playerAlive) {
		mStage = 666;
		return;
	}

	if (clearLevel) {
		mStage++;
		if (mStage <= 4) {
			gpObjectFactory->LoadLevel(("..\\Resources\\Level" + std::to_string(mStage) + ".json").c_str());
		}
		else {
			mStage = 99;
		}
	}
}


