/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Component.h
Purpose: Allows subroutines added to objects to be created under a generic "component" type
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_2
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 15, 2020
- End Header --------------------------------------------------------*/

#pragma once

#include <fstream>
#include <SDL.h>
#include "rapidjson/document.h"

class GameObject;
class Event;

enum COMPONENT_TYPE {
	TYPE_TRANSFORM,
	TYPE_SPRITE,
	TYPE_BODY,
	TYPE_PLAYER_CONTROLLER,
	TYPE_UP_DOWN,
	TYPE_SLIME,
	TYPE_CHARACTER,
	TYPE_AUGMENTOR,
	TYPE_SNIPER,
	TYPE_AUDIOCLIP,
	TYPE_RUNNER,
	TYPE_LEFT_RIGHT,
	TYPE_OBJECTIVE,
	TYPE_BOSS_ATTACK,
	TYPE_INVINCIBILITY,
	TYPE_EYE
};

// Abstract Class
class Component {
public:
	Component(unsigned int type);
	virtual ~Component();

	// has one or more pure virtual function
	virtual void Update() = 0;

	unsigned int GetType() { return mType; }

	virtual void Serialize(std::ifstream& InputStream) = 0;
	virtual void Serialize(rapidjson::GenericArray<false, rapidjson::Value>) = 0;

	virtual void HandleEvent(Event *pEvent) { }

public:
	GameObject* mpOwner;


private:
private:
	unsigned int mType;
};
