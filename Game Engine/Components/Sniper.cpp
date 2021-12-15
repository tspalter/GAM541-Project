#include "Sniper.h"
#include "..\FrameRateController.h"
#include "..\GameObjectManager.h"
#include "..\GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Character.h"
#include "../CollisionManager.h"


extern FrameRateController* gpFRC;
extern GameObjectManager* gpGameObjectManager;
extern CollisionManager* gpCollisionManager;

extern bool DEBUG;

Sniper::Sniper() : Component(TYPE_SNIPER) {
	mShotAng = 0.0f;
	mShotL = 600.0f;
	mShotW = 60.0f;

	mShotTime = mShotTimer = 2000;
	mShotDelay = 1200;

	mShooting = false;
}

Sniper::~Sniper() {
	//
}

void Sniper::Update() {
	Transform* pT = static_cast<Transform*>(this->mpOwner->GetComponent(TYPE_TRANSFORM));
	Character* pC = static_cast<Character*>(this->mpOwner->GetComponent(TYPE_CHARACTER));

	mShotTimer += gpFRC->GetFrameTime();
	if (mShotTimer >= mShotTime + mShotDelay) {
		mShotTimer = 0;
		mShooting = true;

		for (auto pGO : gpGameObjectManager->mGameObjects) {
			if (pGO->GetComponent(TYPE_PLAYER_CONTROLLER)) {
				Transform* pTrans = static_cast<Transform*>(pGO->GetComponent(TYPE_TRANSFORM));
				mShotAng = atan2f(pTrans->mPositionY - pT->mPositionY, pTrans->mPositionX - pT->mPositionX);
				break;
			}
		}
	}

	for (auto pGO : gpGameObjectManager->mGameObjects) {
		Character* pChar = static_cast<Character*>(pGO->GetComponent(TYPE_CHARACTER));
		if (pChar == nullptr || pChar->mFriendly == false)
			continue;

		Transform* pTrans = static_cast<Transform*>(pGO->GetComponent(TYPE_TRANSFORM));
	}
	
	if (mShooting && mShotTimer >= mShotTime) {
		mShooting = false;
		
		for (auto pGO : gpGameObjectManager->mGameObjects) {
			Character* pChar = static_cast<Character*>(pGO->GetComponent(TYPE_CHARACTER));
			if (pChar == nullptr || pChar->mFriendly == false)
				continue;

			if (gpCollisionManager->CollideRect(pC, pChar, mShotAng, mShotL, mShotW)) {
				pChar->mHP -= 1.0f;

				Transform* pTrans = static_cast<Transform*>(pGO->GetComponent(TYPE_TRANSFORM));
				pTrans->mKnockbackHoriz += 800 * cosf(mShotAng);
				pTrans->mKnockbackVert += 800 * sinf(mShotAng);
			}

			/*
			Transform* pTrans = static_cast<Transform*>(pGO->GetComponent(TYPE_TRANSFORM));
			float angLeft, angRight, angBack, angFront;
			float pi = 3.14159;

			angLeft = atan2f((pTrans->mPositionY - pT->mPositionY) + sin(mShotAng + pi / 2.0) * (mShotW / 2.0 + pChar->mRadius), (pTrans->mPositionX - pT->mPositionX) + cos(mShotAng + pi / 2.0) * (mShotW / 2.0 + pChar->mRadius));
			angRight = atan2f((pTrans->mPositionY - pT->mPositionY) + sin(mShotAng - pi / 2.0) * (mShotW / 2.0 + pChar->mRadius), (pTrans->mPositionX - pT->mPositionX) + cos(mShotAng - pi / 2.0) * (mShotW / 2.0 + pChar->mRadius));

			angBack = atan2f((pTrans->mPositionY - pT->mPositionY) + sin(mShotAng) * pChar->mRadius, (pTrans->mPositionX - pT->mPositionX) + cos(mShotAng) * pChar->mRadius) + pi / 2.0;
			angFront = atan2f((pTrans->mPositionY - pT->mPositionY) - sin(mShotAng) * (mShotL + pChar->mRadius), (pTrans->mPositionX - pT->mPositionX) - cos(mShotAng) * (mShotL + pChar->mRadius)) + pi / 2.0;

			if (
				(angLeft > mShotAng && angLeft < mShotAng + pi || angLeft < mShotAng - pi)
				&& (angRight < mShotAng && angRight > mShotAng - pi || angRight > mShotAng + pi)
				&& (angBack > mShotAng && angBack < mShotAng + pi
					|| angBack < mShotAng - pi
					|| angBack > mShotAng + 2.0 * pi
					)
				&& (angFront < mShotAng && angFront > mShotAng - pi
					|| angFront > mShotAng + pi && angFront < mShotAng + 2.0 * pi
					)
				) {
				pChar->mHP -= 1.0f;
				pTrans->mVelHoriz += 800 * cosf(mShotAng);
				pTrans->mVelVert += 800 * sinf(mShotAng);
			}
			*/
		}
		
	}

	if(mShooting)
		pC->AddRect(pT->mPositionX, pT->mPositionY, mShotAng, mShotL, mShotW, (float)mShotTimer/mShotTime);

	if(DEBUG)
		pC->AddRectColor(pT->mPositionX + 32, pT->mPositionY, 3.14159 / 2, 64, 8, (float)mShotTimer / (mShotTime + mShotDelay), .4, .4, .8, 1, .2, .2, .8, 1);
}
