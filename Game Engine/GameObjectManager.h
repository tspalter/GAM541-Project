/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObjectManager.h
Purpose: Handler of all object-type creations
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne
- End Header --------------------------------------------------------*/

#pragma once

#include <vector>

class GameObject;

class GameObjectManager {
public:
	GameObjectManager();
	~GameObjectManager();

public:
	std::vector<GameObject*> mGameObjects;

	void DeleteObject(GameObject* pGO);
	bool mDestroy;
	int mNextID;


private:
private:
};
