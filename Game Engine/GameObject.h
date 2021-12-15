/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObject.h
Purpose: Creates objects and handles collection of components
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne, Adam Rhoades
- End Header --------------------------------------------------------*/

#pragma once

#include <vector>

/*
class Transform;
class Sprite;
class Controller;
class UpDown;
*/
class Component;
class Event;

class GameObject {
public:
	GameObject();
	~GameObject();

	void Update();
	Component* AddComponent(unsigned int type);
	Component* GetComponent(unsigned int type);

	void HandleEvent(Event* pEvent);

public:
	std::vector<Component*> mComponents;
	int mId;
	bool mDestroy;


private:
private:
};
