/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: EventManager.h
Purpose: Handles the storage and broadcasting of events
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne
- End Header --------------------------------------------------------*/

#pragma once

#include <list>
#include <unordered_map>

class Character;
class GameObject;

enum EventType
{
	COLLIDE,
	PLAYER_HIT,
	SHOVE,
	PULSE,
	NUM
};

class Event {
public:
	Event(EventType Type) {
		mType = Type;
		mTimer = 0.0f;
		mChars[0] = mChars[1] = nullptr;
		mDamage = 0.0f;
	}
	virtual ~Event() {}

public:
	EventType mType;
	float mTimer;

	float mDamage;
	Character* mChars[2];

private:
private:
};

/****************************************/

class EventManager {
public:
	EventManager();
	~EventManager();

	void BroadcastEvent(Event* pEvent);
	void BroadcastEventToSubscribers(Event* pEvent);
	void AddTimedEvent(Event* pEvent);
	void Update(float FrameTime);
	void Reset();

	void Subscribe(EventType Et, GameObject* pGameObject);
	
public:
	std::list<Event*> mEvents;

	std::unordered_map<EventType, std::list<GameObject*>> mSubscriptions;


private:
private:
};
