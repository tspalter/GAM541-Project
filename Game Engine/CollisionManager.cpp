/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: CollisionManager.cpp
Purpose: Handles the calculation and broadcasting of collisions
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne
- End Header --------------------------------------------------------*/

#include "CollisionManager.h"
#include "Components/Transform.h"
#include "Components/Character.h"
#include "GameObject.h"

CollideEvent::CollideEvent(Character* self_, Character* target_) : Event(EventType::COLLIDE) {
	mChars[0] = self_;
	mChars[1] = target_;
}

CollideEvent::~CollideEvent() {
	//
}

/********************/

CollisionManager::CollisionManager() {
	//
}

CollisionManager::~CollisionManager() {
	for (auto pCont : mContacts)
		delete pCont;
	mContacts.clear();
}

void CollisionManager::Reset() {
	for (auto pCont : mContacts)
		delete pCont;
	mContacts.clear();
}

bool CollisionManager::CollideCirc(Character* self_, Character* target_,  float ang_, float angwidth_, float in_, float out_) {
	Transform* pT = static_cast<Transform*>(self_->mpOwner->GetComponent(TYPE_TRANSFORM));
	float x_ = pT->mPositionX;
	float y_ = pT->mPositionY;

	Transform* pTrans = static_cast<Transform*>(target_->mpOwner->GetComponent(TYPE_TRANSFORM));
	if (pTrans == nullptr)
		return false;

	float dist = powf(x_ - pTrans->mPositionX, 2) + powf(y_ - pTrans->mPositionY, 2);
	if (dist <= powf(out_ + target_->mRadius, 2) && dist >= powf(in_ - target_->mRadius, 2)) {
		float ang = atan2f(pTrans->mPositionY - y_, pTrans->mPositionX - x_);
		float angMin = atan2f((pTrans->mPositionY + target_->mRadius * sinf(ang - 3.14159 / 2)) - y_, (pTrans->mPositionX + target_->mRadius * cosf(ang - 3.14159 / 2)) - x_);
		float angMax = atan2f((pTrans->mPositionY + target_->mRadius * sinf(ang + 3.14159 / 2)) - y_, (pTrans->mPositionX + target_->mRadius * cosf(ang + 3.14159 / 2)) - x_);

		while (ang_ > 3.14159)
			ang_ -= 2 * 3.14159;
		while (ang_ < -3.14159)
			ang_ += 2 * 3.14159;

		float hitAngMin = ang_ - angwidth_;
		float hitAngMax = ang_ + angwidth_;
		//
		//while (hitAngMin < -3.14159)
		//	hitAngMin += 2 * 3.14159;
		//while (hitAngMax > 3.14159)
		//	hitAngMax -= 2 * 3.14159;
		//

		if (dist < powf(target_->mRadius, 2) ||
			(
				(angMin < hitAngMax && angMax > hitAngMin)
				|| angMin < hitAngMax - 2 * 3.14159
				|| angMax > hitAngMin + 2 * 3.14159
				|| (angMax < angMin
					&& (
						(angMin - 2 * 3.14159 < hitAngMax && angMax > hitAngMin)
						|| (angMin < hitAngMax && angMax + 2 * 3.14159 > hitAngMin)
						)
					)
				)
			) {
			// target_->mHP -= 1;
			CollideEvent* newHit = new CollideEvent(self_, target_);
			mContacts.push_back(newHit);
			return true;
		}
	}

	return false;
}

bool CollisionManager::CollideRect(Character* self_, Character* target_, float ang_, float length_, float width_) {
	Transform* pT = static_cast<Transform*>(self_->mpOwner->GetComponent(TYPE_TRANSFORM));
	Transform* pTrans = static_cast<Transform*>(target_->mpOwner->GetComponent(TYPE_TRANSFORM));

	float angLeft, angRight, angBack, angFront;
	float pi = 3.14159;
	
	angLeft = atan2f((pTrans->mPositionY - pT->mPositionY) + sin(ang_ + pi / 2.0) * (width_ / 2.0 + target_->mRadius), (pTrans->mPositionX - pT->mPositionX) + cos(ang_ + pi / 2.0) * (width_ / 2.0 + target_->mRadius));
	angRight = atan2f((pTrans->mPositionY - pT->mPositionY) + sin(ang_ - pi / 2.0) * (width_ / 2.0 + target_->mRadius), (pTrans->mPositionX - pT->mPositionX) + cos(ang_ - pi / 2.0) * (width_ / 2.0 + target_->mRadius));

	angBack = atan2f((pTrans->mPositionY - pT->mPositionY) + sin(ang_) * target_->mRadius, (pTrans->mPositionX - pT->mPositionX) + cos(ang_) * target_->mRadius) + pi / 2.0;
	angFront = atan2f((pTrans->mPositionY - pT->mPositionY) - sin(ang_) * (length_ + target_->mRadius), (pTrans->mPositionX - pT->mPositionX) - cos(ang_) * (length_ + target_->mRadius)) + pi / 2.0;

	if (
		(angLeft > ang_ && angLeft < ang_ + pi || angLeft < ang_ - pi)
		&& (angRight < ang_ && angRight > ang_ - pi || angRight > ang_ + pi)
		&& (angBack > ang_ && angBack < ang_ + pi
			|| angBack < ang_ - pi
			|| angBack > ang_ + 2.0 * pi
			)
		&& (angFront < ang_ && angFront > ang_ - pi
			|| angFront > ang_ + pi && angFront < ang_ + 2.0 * pi
			)
		) 
	{
		/* target_->mHP -= 1.0f;
		pTrans->mVelHoriz += 800 * cosf(ang_);
		pTrans->mVelVert += 800 * sinf(ang_); */

		CollideEvent* newHit = new CollideEvent(self_, target_);
		mContacts.push_back(newHit);
		return true;
	}

	return false;
}
