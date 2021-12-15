/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: CollisionManager.h
Purpose: Handles the calculation and broadcasting of collisions
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne
- End Header --------------------------------------------------------*/

#pragma once

#include "EventManager.h"
#include "Components/Component.h"
#include <vector>

class Character;

class CollideEvent : public Event {
public:
	CollideEvent(Character* self, Character* target);
	~CollideEvent();

public:
	// Character* mChars[2];
};

class CollisionManager {
public:
	CollisionManager();
	~CollisionManager();

	void Reset();

public:
	bool CollideCirc(Character* self_, Character* target_, float ang_, float angwidth_, float in_, float out_);
	bool CollideRect(Character* self_, Character* target_, float ang_, float length_, float width_);

	std::vector<CollideEvent*> mContacts;


private:
private:
};
