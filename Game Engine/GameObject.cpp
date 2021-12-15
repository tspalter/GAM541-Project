/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObject.cpp
Purpose: Creates objects and handles collection of components
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne, Adam Rhoades, Thomas Spalter
- End Header --------------------------------------------------------*/


#include "GameObject.h"
#include "EventManager.h"
#include "GameObjectManager.h"

#include "Components/Sprite.h"
#include "Components/Transform.h"
#include "Components/Character.h"
#include "Components/Controller.h"
#include "Components/UpDown.h"
#include "Components/Component.h"
#include "Components/Slime.h"
#include "Components/Augmentor.h"
#include "Components/Sniper.h"
#include "Components/Body.h"
#include "Components/AudioClip.h"
#include "Components/Runner.h"
#include "Components/LeftRight.h"
#include "Components/Objective.h"
#include "Components/BossAttack.h"
#include "Components/Invincibility.h"
#include "Components/Eye.h"

extern GameObjectManager* gpGameObjectManager;

GameObject::GameObject() {
	//
	mId = gpGameObjectManager->mNextID++;
}

GameObject::~GameObject() {
	for (auto pComponent : mComponents)
		delete pComponent;
	mComponents.clear();
}

void GameObject::Update() {
	for (auto pComponent : mComponents)
		pComponent->Update();
}

Component* GameObject::AddComponent(unsigned int type) {
	Component* pNewComponent;
	pNewComponent = GetComponent(type);
	if (pNewComponent == nullptr) {
		switch (type) {
		case TYPE_TRANSFORM:
			pNewComponent = new Transform();
			break;
		case TYPE_SPRITE:
			pNewComponent = new Sprite();
			break;
		case TYPE_CHARACTER:
			pNewComponent = new Character();
			break;
		case TYPE_BODY:
			pNewComponent = new Body();
			break;
		case TYPE_PLAYER_CONTROLLER:
			pNewComponent = new Controller();
			break;
		case TYPE_UP_DOWN:
			pNewComponent = new UpDown();
			break;
		case TYPE_LEFT_RIGHT:
			pNewComponent = new LeftRight();
			break;
		case TYPE_SLIME:
			pNewComponent = new Slime();
			break;
		case TYPE_AUGMENTOR:
			pNewComponent = new Augmentor();
			break;
		case TYPE_SNIPER:
			pNewComponent = new Sniper();
			break;
		case TYPE_AUDIOCLIP:
			pNewComponent = new AudioClip();
			break;
		case TYPE_RUNNER:
			pNewComponent = new Runner();
			break;
		case TYPE_OBJECTIVE:
			pNewComponent = new Objective();
			break;
		case TYPE_BOSS_ATTACK:
			pNewComponent = new BossAttack();
			break;
		case TYPE_INVINCIBILITY:
			pNewComponent = new Invincibility();
			break;
		case TYPE_EYE:
			pNewComponent = new Eye();
			break;
		default:
			pNewComponent = nullptr;
		}
	}

	if (nullptr != pNewComponent) {
		mComponents.push_back(pNewComponent);
		pNewComponent->mpOwner = this;
	}

	return pNewComponent;
}

Component* GameObject::GetComponent(unsigned int type) {
	for (auto pComponent : mComponents)
		if (pComponent->GetType() == type)
			return pComponent;

	return nullptr;
}

void GameObject::HandleEvent(Event *pEvent) {
	for (auto pComponent : mComponents)
		pComponent->HandleEvent(pEvent);
}
