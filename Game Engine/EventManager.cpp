/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: EventManager.cpp
Purpose: Handles the storage and broadcasting of events
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne
- End Header --------------------------------------------------------*/

#include "EventManager.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include <assert.h>

extern GameObjectManager* gpGameObjectManager;

EventManager::EventManager() {
	//
}

EventManager::~EventManager() {
	std::list<Event*>::iterator it = mEvents.begin();

	while (it != mEvents.end()) {
		Event* pEvent = *it;
		delete pEvent;
		it = mEvents.erase(it);
	}

	for (auto pPair : mSubscriptions) {
		pPair.second.clear();
	}

	mSubscriptions.clear();
}


void EventManager::BroadcastEvent(Event* pEvent) {
	for (auto pGO : gpGameObjectManager->mGameObjects)
		pGO->HandleEvent(pEvent);
}

void EventManager::BroadcastEventToSubscribers(Event* pEvent) {
	std::list<GameObject*>& listOfSubscribers = mSubscriptions[pEvent->mType];
	assert(!listOfSubscribers.empty()); 

	for (auto pGO : listOfSubscribers)
		pGO->HandleEvent(pEvent);
}


void EventManager::AddTimedEvent(Event* pEvent) {
	mEvents.push_back(pEvent);
}

void EventManager::Update(float FrameTime) {
	std::list<Event*>::iterator it = mEvents.begin();

	while (it != mEvents.end()) {
		Event* pEvent = *it;

		pEvent->mTimer -= FrameTime;
		if (pEvent->mTimer <= 0.0f) {
			BroadcastEvent(pEvent);
			delete pEvent;
			it = mEvents.erase(it);
		}
		else {
			++it;
		}
	}
}

void EventManager::Reset() {
	std::list<Event*>::iterator it = mEvents.begin();

	while (it != mEvents.end()) {
		Event* pEvent = *it;
		delete pEvent;
		it = mEvents.erase(it);
	}

	for (auto pPair : mSubscriptions) {
		pPair.second.clear();
	}

	mSubscriptions.clear();
}

void EventManager::Subscribe(EventType Et, GameObject* pGameObject)
{
	std::list<GameObject*>& listOfSubscribers = mSubscriptions[Et];

	for (auto pGO : listOfSubscribers)
		if (pGO == pGameObject)
			return;

	listOfSubscribers.push_back(pGameObject);
}
