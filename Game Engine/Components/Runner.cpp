
#include "Runner.h"
#include "Transform.h"
#include "Character.h"
#include "Sprite.h"
#include "Body.h"
#include "../InputManager.h"
#include "../GameObject.h"
#include "../EventManager.h"
#include "../GameObjectManager.h"
#include "../ObjectFactory.h"
#include "../ResourceManager.h"
#include "../GameObjectManager.h"
#include "../FrameRateController.h"
#include <stdlib.h>
#include <time.h>

extern ObjectFactory* gpObjectFactory;
extern ResourceManager* gpResourceManager;
extern GameObjectManager* gpGameObjectManager;
extern FrameRateController* gpFRC;
extern InputManager* gpInputManager;

extern bool DEBUG;

Runner::Runner() : Component(TYPE_RUNNER) {
	mTimer = 0;
	mGravity = 1600.f;
	mAcc = 2400.f;
	mVelMax = 3000.f;

	mWallTime = mWallDelay = 0.2;
	mObstacleTime = mObstacleDelay = 1.6;
	mMissileTime = mMissileDelay = 6.f;

	mMissileActive = false;
	mMissileLaunchTime = mMissileLaunchDelay = 2.f;
	mMissileY = mMissileTracking = 100.f;

	mFail = false;

	// gpObjectFactory->LoadGameObject("../Resources/RunnerWall.json");
	
	for (int i = 0; i <= 1264 / 64; i++) {
		GameObject* pWall = gpObjectFactory->LoadGameObject("../Resources/RunnerWall.json");
		Transform* pT = static_cast<Transform*>(pWall->GetComponent(TYPE_TRANSFORM));
		pT->mPositionX = i * 64;

		pWall = gpObjectFactory->LoadGameObject("../Resources/RunnerWall.json");
		pT = static_cast<Transform*>(pWall->GetComponent(TYPE_TRANSFORM));
		pT->mPositionX = i * 64;
		pT->mPositionY = 800;
	}

	srand(time(NULL));
}

Runner::~Runner() {
	//
}

void Runner::Update() {
	float delta = gpFRC->GetDeltaTime();

	Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
	Body* pB = static_cast<Body*>(mpOwner->GetComponent(TYPE_BODY));
	Character* pC = static_cast<Character*>(mpOwner->GetComponent(TYPE_CHARACTER));

	if (mFail) {
		for (auto pGO : gpGameObjectManager->mGameObjects) {
			Transform* pTrans = static_cast<Transform*>(pGO->GetComponent(TYPE_TRANSFORM));
			pTrans->mVelHoriz = 0.f;
			pTrans->mVelVert = 0.f;
		}

		pC->AddTelegraph(pT->mPositionX, pT->mPositionY - pB->mHeight / 2.f, 0, M_PI, 128, 1200, 0.f);
	}
	else {
		/*** Upkeep ***/
		mTimer += delta;

		for (auto pGO : gpGameObjectManager->mGameObjects) {
			if (pGO == mpOwner) { continue; }

			Transform* pTrans = static_cast<Transform*>(pGO->GetComponent(TYPE_TRANSFORM));
			if (pTrans->mPositionX < 0.f) {
				pGO->mDestroy = true;
				continue;
			}

			Body* pBody = static_cast<Body*>(pGO->GetComponent(TYPE_BODY));

			if (
				pBody != nullptr
				&& pT->mPositionX - pB->mWidth / 2.f < pTrans->mPositionX + pBody->mWidth / 2.f
				&& pT->mPositionX + pB->mWidth / 2.f > pTrans->mPositionX - pBody->mWidth / 2.f
				&& pT->mPositionY - pB->mHeight < pTrans->mPositionY
				&& pT->mPositionY > pTrans->mPositionY - pBody->mHeight
			) {
				if (pBody->mFriendly) {
					pGO->mDestroy = true;
				}
				else {
					mFail = true;
				}
			}
		}

		/*** (De)Spawning ***/
		if (mTimer > mWallTime) {
			mWallTime += mWallDelay;

			gpObjectFactory->LoadGameObject("../Resources/RunnerWall.json");

			GameObject* pWall = gpObjectFactory->LoadGameObject("../Resources/RunnerWall.json");
			Transform* pTrans = static_cast<Transform*>(pWall->GetComponent(TYPE_TRANSFORM));
			pTrans->mPositionY = 800;
		}

		if (mTimer > mObstacleTime) {
			if (mObstacleTime == 5.f * mObstacleDelay) {
				mObstacleTime += 5.f;

				GameObject* pWall = gpObjectFactory->LoadGameObject("../Resources/RunnerObstacle.json");
				Body* pBody = static_cast<Body*>(pWall->GetComponent(TYPE_BODY));
				Transform* pTrans = static_cast<Transform*>(pWall->GetComponent(TYPE_TRANSFORM));
				pTrans->mPositionY = 532.f;
				pTrans->mPositionX = 1700.f;
				pTrans->mHeight = 500.f;
				pTrans->mWidth = 16.f;
				pBody->mHeight = 450.f;
				pBody->mWidth = 3.f;

				pWall = gpObjectFactory->LoadGameObject("../Resources/RunnerObstacle.json");
				pBody = static_cast<Body*>(pWall->GetComponent(TYPE_BODY));
				pTrans = static_cast<Transform*>(pWall->GetComponent(TYPE_TRANSFORM));
				pTrans->mPositionY = 738.f;
				pTrans->mPositionX = 2000.f;
				pTrans->mHeight = 500.f;
				pTrans->mWidth = 16.f;
				pBody->mHeight = 450.f;
				pBody->mWidth = 3.f;

				pWall = gpObjectFactory->LoadGameObject("../Resources/RunnerObstacle.json");
				pBody = static_cast<Body*>(pWall->GetComponent(TYPE_BODY));
				pTrans = static_cast<Transform*>(pWall->GetComponent(TYPE_TRANSFORM));
				pTrans->mPositionY = 532.f;
				pTrans->mPositionX = 2300.f;
				pTrans->mHeight = 500.f;
				pTrans->mWidth = 16.f;
				pBody->mHeight = 450.f;
				pBody->mWidth = 3.f;

				/*******/

				pWall = gpObjectFactory->LoadGameObject("../Resources/RunnerCoin.json");
				pTrans = static_cast<Transform*>(pWall->GetComponent(TYPE_TRANSFORM));
				pTrans->mPositionX = 1925.f;
				pWall = gpObjectFactory->LoadGameObject("../Resources/RunnerCoin.json");
				pTrans = static_cast<Transform*>(pWall->GetComponent(TYPE_TRANSFORM));
				pTrans->mPositionX = 1975.f;
				pWall = gpObjectFactory->LoadGameObject("../Resources/RunnerCoin.json");
				pTrans = static_cast<Transform*>(pWall->GetComponent(TYPE_TRANSFORM));
				pTrans->mPositionX = 2025.f;
			}
			mObstacleTime += mObstacleDelay;

			GameObject* pWall = gpObjectFactory->LoadGameObject("../Resources/RunnerObstacle.json");
			Transform* pTrans = static_cast<Transform*>(pWall->GetComponent(TYPE_TRANSFORM));
			pTrans->mPositionY = rand() % 414 + 320;

			if (mTimer > mObstacleDelay * 12.f) {
				pWall = gpObjectFactory->LoadGameObject("../Resources/RunnerObstacle.json");
				pTrans = static_cast<Transform*>(pWall->GetComponent(TYPE_TRANSFORM));
				pTrans->mPositionY = rand() % 414 + 320;
			}

			{
				float h = rand() % 640 + 112;

				pWall = gpObjectFactory->LoadGameObject("../Resources/RunnerCoin.json");
				pTrans = static_cast<Transform*>(pWall->GetComponent(TYPE_TRANSFORM));
				pTrans->mPositionX = 1425.f;
				pTrans->mPositionY = h;
				pWall = gpObjectFactory->LoadGameObject("../Resources/RunnerCoin.json");
				pTrans = static_cast<Transform*>(pWall->GetComponent(TYPE_TRANSFORM));
				pTrans->mPositionX = 1475.f;
				pTrans->mPositionY = h;
				pWall = gpObjectFactory->LoadGameObject("../Resources/RunnerCoin.json");
				pTrans = static_cast<Transform*>(pWall->GetComponent(TYPE_TRANSFORM));
				pTrans->mPositionX = 1525.f;
				pTrans->mPositionY = h;
			}
		}

		/*** Missiles ***/
		if (mTimer > mMissileTime) {
			mMissileTime += mMissileDelay;
			mMissileActive = true;
			mMissileLaunchTime = 0.f;
			mMissileY = pT->mPositionY;
		}

		if (mMissileActive) {
			mMissileLaunchTime += delta;
			
			if (fabsf(mMissileY - pT->mPositionY) < mMissileTracking * delta) {
				mMissileY = pT->mPositionY;
			}
			else {
				mMissileY += delta * mMissileTracking * (mMissileY > pT->mPositionY ? -1.f : 1.f);
			}
			
			// pC->AddRect(1200.f, mMissileY - pB->mHeight / 2.f, M_PI, 1200.f, 32.f, mMissileLaunchTime / mMissileLaunchDelay);
			pC->AddTelegraph(1200.f + 64.f, mMissileY - pB->mHeight / 2.f, M_PI, M_PI / 16.f, 72.f, 256.f, mMissileLaunchTime / mMissileLaunchDelay);

			if (mMissileLaunchTime >= mMissileLaunchDelay) {
				mMissileActive = false;
				GameObject* pMissile = gpObjectFactory->LoadGameObject("../Resources/RunnerMissile.json");
				Transform* pTrans = static_cast<Transform*>(pMissile->GetComponent(TYPE_TRANSFORM));
				pTrans->mPositionY = mMissileY - pB->mHeight / 2.f + 8.f;
			}
		}

		/*** Player Behaviour ***/
		if (gpInputManager->IsKeyPressed(SDL_SCANCODE_W) || gpInputManager->IsKeyPressed(SDL_SCANCODE_SPACE)) {
			pT->mVelVert -= mAcc * delta;
		}
		else {
			if (pT->mVelVert >= mVelMax) {
				pT->mVelVert = mVelMax;
			}
			else {
				if (gpInputManager->IsKeyPressed(SDL_SCANCODE_S)) {
					pT->mVelVert += mGravity * delta * 8.f;
				}
				else {
					pT->mVelVert += mGravity * delta;
				}
			}
		}

		if (pT->mVelVert < 0.f) {
			if (pT->mPositionY - pB->mHeight < 64.f) {
				pT->mPositionY = 64.f + pB->mHeight;
				pT->mVelVert = 0.f;
			}
		}
		else {
			if (pT->mPositionY > 736.f) {
				pT->mPositionY = 736.f;
				pT->mVelVert = 0.f;
			}
		}
	}


}

void Runner::Serialize(rapidjson::GenericArray<false, rapidjson::Value> input) {
	if (input[0].HasMember("sample")) {
		// mHeight = input[0]["height"].GetFloat();
	}
}
