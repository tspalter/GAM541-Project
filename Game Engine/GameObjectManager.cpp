/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObjectManager.cpp
Purpose: Handler of all object-type creations
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne
- End Header --------------------------------------------------------*/

#include "GameObjectManager.h"
#include "GameObject.h"
#include <vector>
#include <algorithm>

GameObjectManager::GameObjectManager() {
	mDestroy = false;
	mNextID = 0;
}

GameObjectManager::~GameObjectManager() {
	for (auto pGO : mGameObjects) {
		pGO->~GameObject();
		delete pGO;
	}

	mGameObjects.clear();
}

void GameObjectManager::DeleteObject(GameObject* pGO) {
	if (!mDestroy) {
		mDestroy = true;
		std::vector<GameObject*>::iterator it;
		it = std::find(mGameObjects.begin(), mGameObjects.end(), pGO);
		mGameObjects.erase(it);
	}
}
