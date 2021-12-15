/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Controller.cpp
Purpose: Handles player movement and attacking
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne, Adam Rhoades
- End Header --------------------------------------------------------*/

#include "Controller.h"
#include "Sprite.h"
#include "..\InputManager.h"
#include "..\..\SDL2.0 Lib\include\SDL_scancode.h"
#include "..\GameObject.h"
#include "Character.h"
#include "Body.h"
#include "BossAttack.h"
#include "Eye.h"
#include "math.h"
#include "..\FrameRateController.h"
#include "../GameObjectManager.h"
#include "../EventManager.h"
#include "../CollisionManager.h"
#include "AudioClip.h"

extern FrameRateController* gpFRC;
extern InputManager* gpInputManager;
extern GameObjectManager* gpGameObjectManager;
extern EventManager* gpEventManager;
extern AudioManager* gpAudioManager;
extern int gGameType;


PlayerHitEvent::PlayerHitEvent() : Event(EventType::PLAYER_HIT) {
	//
}

PlayerHitEvent::~PlayerHitEvent() {
	//
}

/**************************************/

Controller::Controller() : Component(TYPE_PLAYER_CONTROLLER) {
	// mVelHoriz = 0.0f;
	// mVelVert = 0.0f;

	mAcceleration = 1600;
	mMaxSpeed = 160;
	mGodMode = false;

	mSlowMod = 0.25;
	mExcessSlowMod = 2.0f;

	mDashTimer = mDashCooldown = 5000;

	if (gGameType == 1)
	{
		mSwingTime = 800;
		mSwingDelay = 400;
		mSwingTimer = mSwingTime + mSwingDelay;
		mSwingAng = 0.0f;
		mSwingWidth = 0.9f;
		mSwingLen = 220.f;
		mSwinging = mCleaver = false;
	}
	else if (gGameType == 3)
	{
		mSwingTime = 800;
		mSwingDelay = 1;
		mSwingTimer = mSwingTime + mSwingDelay;
		mSwingAng = 0.0f;
		mSwingWidth = 0.3f;
		mSwingLen = 100.0f;
		mSwinging = mCleaver = false;
	}
}

Controller::~Controller() {
	//
}

extern bool DEBUG;

void Controller::Update() {

	Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));

	float h_mod, v_mod;
	h_mod = ((gpInputManager->IsKeyPressed(SDL_SCANCODE_RIGHT) || gpInputManager->IsKeyPressed(SDL_SCANCODE_D)) - (gpInputManager->IsKeyPressed(SDL_SCANCODE_LEFT) || gpInputManager->IsKeyPressed(SDL_SCANCODE_A)));
	v_mod = ((gpInputManager->IsKeyPressed(SDL_SCANCODE_DOWN) || gpInputManager->IsKeyPressed(SDL_SCANCODE_S)) - (gpInputManager->IsKeyPressed(SDL_SCANCODE_UP) || gpInputManager->IsKeyPressed(SDL_SCANCODE_W)));

	// h_mod = (gpInputManager->IsKeyPressed(SDL_SCANCODE_D) - gpInputManager->IsKeyPressed(SDL_SCANCODE_A));
	// v_mod = (gpInputManager->IsKeyPressed(SDL_SCANCODE_S) - gpInputManager->IsKeyPressed(SDL_SCANCODE_W));

	if (h_mod != 0 && v_mod != 0) {
		h_mod *= 0.7;
		v_mod *= 0.7;
	}

	mDashTimer += gpFRC->GetFrameTime();
	if (gpInputManager->IsKeyTriggered(SDL_SCANCODE_SPACE) || gpInputManager->IsKeyTriggered(SDL_SCANCODE_UP)) {
		/*
		mDashTimer = 0;

		if (h_mod == 0 && v_mod == 0) {
			pT->mVelHoriz += 1000;
		}
		else {
			pT->mVelHoriz += 1000 * h_mod;
			pT->mVelVert += 1000 * v_mod;
		}
		*/
		if (mIsGrounded)
			Jump(pT);
		else if (mJumpsLeft > 0)
		{
			mJumpsLeft--;
			Jump(pT);
		}

	}

	if (h_mod != 0.0 && (gpInputManager->IsKeyPressed(SDL_SCANCODE_LCTRL) || gpInputManager->IsKeyPressed(SDL_SCANCODE_RCTRL)) && mDashTimer >= mDashCooldown)
	{
		AudioClip* pAC = static_cast<AudioClip*>(mpOwner->GetComponent(TYPE_AUDIOCLIP));
		pAC->PlayOneShot("Dash");
		mDashTimer = 0;
		//if already triggered within the Limit
		if (h_mod == 0 && v_mod == 0) {
			pT->mVelHoriz += 1000;
		}
		else {
			pT->mVelHoriz += 1000 * h_mod;
			pT->mVelVert += 1000 * v_mod;
		}
		

		
	}

	float dTime = gpFRC->GetDeltaTime();
	


	Body* pB = static_cast<Body*>(mpOwner->GetComponent(TYPE_BODY));

	/*** Vert ***/
	pT->mVelVert += 800 * dTime;  // gravity

	{
		bool collision = false;
		bool bounce = false;
		float pos = 0.0;

		for (auto pObject : gpGameObjectManager->mGameObjects) {
			Body* pBody = static_cast<Body*>(pObject->GetComponent(TYPE_BODY));
			if (!pBody || pBody->mpOwner == mpOwner || !pBody->mWall) { continue; }

			bool velDown = pT->mVelVert > 0 ? 1 : 0;

			Transform* pTrans = static_cast<Transform*>(pObject->GetComponent(TYPE_TRANSFORM));
			if (
				pT->mPositionY + (pT->mVelVert * dTime * velDown) >= pTrans->mPositionY - pBody->mHeight
				&& pT->mPositionY - pB->mHeight + (pT->mVelVert * dTime * !velDown) <= pTrans->mPositionY
				&& pT->mPositionX - pB->mWidth / 2 <= pTrans->mPositionX + pBody->mWidth / 2
				&& pT->mPositionX + pB->mWidth / 2 >= pTrans->mPositionX - pBody->mWidth / 2
			) {
				if (pBody->mSemisolid && (pT->mVelVert <= 0 || pT->mPositionY > pTrans->mPositionY-pBody->mHeight)) {
					continue;
				}

				if (!collision) {
					collision = true;

					pos = pTrans->mPositionY + (pT->mVelVert >= 0 ? -pBody->mHeight - .001 : pB->mHeight + .001);
				}
				else {
					if (pos > pTrans->mPositionY + (pT->mVelVert >= 0 ? -pBody->mHeight - .001 : pB->mHeight + .001)) {
						//
					}
				}

				/***** Add wall-specific collisions here *****/
				if (pBody->mBounce) {
					bounce = true;
				}
			}

			float wallJumpTolerance = 1.0f;
			if (
				pT->mPositionY + (pT->mVelVert * dTime) >= pTrans->mPositionY - pBody->mHeight
				&& pT->mPositionY - pB->mHeight + (pT->mVelVert * dTime) <= pTrans->mPositionY
				&& pT->mPositionY < pTrans->mPositionY
				&& pT->mPositionX - pB->mWidth / 2 <= pTrans->mPositionX + ((pBody->mWidth / 2) + wallJumpTolerance)
				&& pT->mPositionX + pB->mWidth / 2 >= pTrans->mPositionX - ((pBody->mWidth / 2) + wallJumpTolerance)
				)
			{

				mIsGrounded = true;
				mJumpsLeft = mJumps;
			}
			
					
		}



		if (!mIsPrevGrounded)
		{
			if (mIsGrounded)
			{
				AudioClip* pAC = static_cast<AudioClip*>(mpOwner->GetComponent(TYPE_AUDIOCLIP));
				pAC->PlayOneShot("Grounded");
			}
		}


		mIsPrevGrounded = mIsGrounded;

		if (collision) {
			pT->mPositionY = pos;

			if (bounce) {
				pT->mVelVert = 800 * (pT->mVelVert > 0.f ? -1.f : 0.f);
			}
			else {
				pT->mVelVert = 0.f;
			}
		}
		else
			mIsGrounded = false;

		


	}


	/*** Horiz ***/
	pT->mVelHoriz += mAcceleration * h_mod * dTime;
	{
		bool collision = false;
		float pos = 0.0;


		for (auto pObject : gpGameObjectManager->mGameObjects) {
			Body* pBody = static_cast<Body*>(pObject->GetComponent(TYPE_BODY));
			if (!pBody || pBody->mpOwner == mpOwner) { continue; }

			Transform* pTrans = static_cast<Transform*>(pObject->GetComponent(TYPE_TRANSFORM));

			if (
				pT->mPositionY >= pTrans->mPositionY - pBody->mHeight
				&& pT->mPositionY - pB->mHeight <= pTrans->mPositionY
				&& pT->mPositionX - pB->mWidth / 2 + (pT->mVelHoriz * dTime) <= pTrans->mPositionX + pBody->mWidth / 2
				&& pT->mPositionX + pB->mWidth / 2 + (pT->mVelHoriz * dTime) >= pTrans->mPositionX - pBody->mWidth / 2
			) {
				if (pBody->mSemisolid) { continue; }
				
				if (!collision) {
					collision = true;
					pos = pTrans->mPositionX + (pB->mWidth/2 + pBody->mWidth/2 + .001) * (pT->mVelHoriz >= 0 ? -1 : 1);
				}
				
				// pT->mVelHoriz = 0.0f;
			}
		}

		if (collision) {
			pT->mVelHoriz = 0.0f;
			pT->mPositionX = pos;
		}

	}


	/*** Slowing ***/
	float ang = atan2f(pT->mVelVert, pT->mVelHoriz);
	if (pT->mVelHoriz != 0) {
		if (fabsf(pT->mVelHoriz) - fabsf(mAcceleration * cosf(ang) * mSlowMod * dTime) <= 0) {
			pT->mVelHoriz = 0;
		}
		else {
			pT->mVelHoriz -= mAcceleration * cosf(ang) * mSlowMod * dTime;
		}
	}

	/*
	if (pT->mVelVert != 0) {
		if (fabsf(pT->mVelVert) - fabsf(mAcceleration * sinf(ang) * mSlowMod * dTime) <= 0) {
			pT->mVelVert = 0;
		}
		else {
			pT->mVelVert -= mAcceleration * sinf(ang) * mSlowMod * dTime;
		}
	}
	*/

	if (pT->mVelVert > 600.f) {
		pT->mVelVert = 600.f;
	}
	if (pT->mVelVert < -700.0f)
		pT->mVelVert = -700.0f;
	

	// TODO: Strip out code from when game was top-down
	// float sqVel = (pT->mVelHoriz * pT->mVelHoriz + pT->mVelVert * pT->mVelVert) / (mMaxSpeed * mMaxSpeed);
	float sqVel = (pT->mVelHoriz * pT->mVelHoriz) / (mMaxSpeed * mMaxSpeed);
	if (sqVel > 1.0f) {
		// ang = atan2f(mVelVert, mVelHoriz);
		if(fabsf(pT->mVelHoriz) - mAcceleration*mExcessSlowMod*dTime <= mMaxSpeed){
		// if (sqrtf(pT->mVelHoriz * pT->mVelHoriz + pT->mVelVert * pT->mVelVert) - mAcceleration * mExcessSlowMod * dTime <= mMaxSpeed) {
			/*
			pT->mVelHoriz /= sqVel;
			pT->mVelVert /= sqVel;
			*/
			pT->mVelHoriz /= sqVel;
		}
		else {
			pT->mVelHoriz -= mAcceleration * cosf(ang) * mExcessSlowMod * dTime;
			/*
			pT->mVelHoriz -= mAcceleration * cosf(ang) * mExcessSlowMod * dTime;
			pT->mVelVert -= mAcceleration * sinf(ang) * mExcessSlowMod * dTime;
			*/
		}
	}

	/*
	if (sqVel > 1.0f) {
		mVelHoriz /= sqVel;
		mVelVert /= sqVel;
	}
	*/


	/*** Final Position ***/
	// Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
	// pT->mPositionX += mVelHoriz * dTime;
	// pT->mPositionY += mVelVert * dTime;



	/*** Attacks ***/
	mSwingTimer += gpFRC->GetFrameTime();
	if ((gpInputManager->IsMouseTriggered() || ((SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))) && gGameType == 1) && !mSwinging && mSwingTimer >= mSwingTime+mSwingDelay) {
		mSwinging = true;
		mSwingTimer = 0;
		mCleaver = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT);
	}

	Character* pC = static_cast<Character*>(mpOwner->GetComponent(TYPE_CHARACTER));
	Sprite* pS = static_cast<Sprite*>(mpOwner->GetComponent(TYPE_SPRITE));
	if (mSwinging) {
		if (pS->mIsAnimated)
		{
			if (!pS->mpSpriteAnimator->mIsAttacking)
				pS->mpSpriteAnimator->StartAttacking();
		}
		mSwingAng = atan2(gpInputManager->mMouseY - (pT->mPositionY-24.f), gpInputManager->mMouseX - pT->mPositionX);
		// mSwingAng > 0.0 && mSwingAng < 1.0 ? pT->mSpriteOffsetY = -30 : pT->mSpriteOffsetY = 0;
		// mSwingAng + mSwingWidth > 0 ? pT->mSpriteOffsetX = -30 : pT->mSpriteOffsetX = 0;
		if (!mCleaver) {
			pC->AddTelegraphColor(pT->mPositionX, pT->mPositionY-24.f, mSwingAng, mSwingWidth, 20, mSwingLen, (float)mSwingTimer / mSwingTime, .2, .2, .8, .5, .3, .3, .8, .5);

		}
		else {
			if (gGameType == 1)
			{
				pC->AddTelegraphColor(pT->mPositionX - 100 * cosf(mSwingAng), pT->mPositionY - 100 * sinf(mSwingAng), mSwingAng, mSwingWidth * 0.2, 120, 700, (float)mSwingTimer / mSwingTime, .2, .2, .8, .5, .3, .3, .8, .5);
				pC->mHP = pC->mHPMax;
			}
			
		}

		if (mSwingTimer >= mSwingTime) {
			AudioClip* pAC = static_cast<AudioClip*>(mpOwner->GetComponent(TYPE_AUDIOCLIP));
			pAC->PlayOneShot("Slash");
			pS->mpSpriteAnimator->mIsAttacking = false;
			mSwinging = false;			
			// pC->mHP -= 1;

			for (auto pObject : gpGameObjectManager->mGameObjects) {
				Character* pChar = static_cast<Character*>(pObject->GetComponent(TYPE_CHARACTER));
				BossAttack* pBA = static_cast<BossAttack*>(pObject->GetComponent(TYPE_BOSS_ATTACK));
				Eye* pE = static_cast<Eye*>(pObject->GetComponent(TYPE_EYE));
				Sprite* pS = static_cast<Sprite*>(pObject->GetComponent(TYPE_SPRITE));
				if (pChar == nullptr || pChar->mFriendly == true)
					continue;

				if (!mCleaver) {
					if (pChar->CollideCirc(pT->mPositionX, pT->mPositionY-24.f, mSwingAng, mSwingWidth, 0, mSwingLen)) {
						if (gGameType == 1 || (gGameType == 3 && (pBA != nullptr || pE != nullptr)))
						{
							if(mGodMode)
								pChar->mHP -= 3.0f;
							else
								pChar->mHP -= 1.0f;
							if (pS != nullptr && pE != nullptr)
							{
								pS->mpSpriteAnimator->mIdlingPhase++;
								pS->mpSpriteAnimator->StartIdling();
							}

						}
						else if (gGameType == 3)
						{
							pChar->mIsStunned = true;
							pChar->mStunnedDuration = 2000;
							pS->mpSpriteAnimator->StartIdling();
							
						}
					}
				}
				else {
					if (gGameType == 1)
					{
						if (pChar->CollideCirc(pT->mPositionX - 100 * cosf(mSwingAng), pT->mPositionY - 100 * sinf(mSwingAng), mSwingAng, mSwingWidth * 0.2, 120, 700)) {
							pChar->mHP -= 10.0f;
						}
					}
					
				}
				

				/*
				Transform* pT = static_cast<Transform*>(pObject->GetComponent(TYPE_TRANSFORM));
				if (pT == nullptr)
					continue;

				float dist = powf(pT->mPositionX - pT->mPositionX, 2) + powf(pT->mPositionY - pT->mPositionY, 2);
				if (dist <= powf(120 + pChar->mRadius, 2)) {
					float ang = atan2f(pT->mPositionY - pT->mPositionY, pT->mPositionX - pT->mPositionX);
					float angMin = atan2f((pT->mPositionY + pChar->mRadius * sinf(ang - 3.14159 / 2)) - pT->mPositionY, (pT->mPositionX + pChar->mRadius * cosf(ang - 3.14159 / 2)) - pT->mPositionX);
					float angMax = atan2f((pT->mPositionY + pChar->mRadius * sinf(ang + 3.14159 / 2)) - pT->mPositionY, (pT->mPositionX + pChar->mRadius * cosf(ang + 3.14159 / 2)) - pT->mPositionX);

					float swingMin = mSwingAng - mSwingWidth;
					float swingMax = mSwingAng + mSwingWidth;

					if (dist < powf(pChar->mRadius, 2) || 
						(
							(angMin < swingMax && angMax > swingMin) 
							|| angMin < swingMax - 2 * 3.14159
							|| angMax > swingMin + 2 * 3.14159
							|| (angMax < angMin 
								&& (
									(angMin - 2*3.14159 < swingMax && angMax > swingMin)
									|| (angMin < swingMax && angMax + 2*3.14159 > swingMin)
								)
							)
						)
					) {
						pChar->mHP -= 1;
					}
					
				} */
			}
		}
	}

	if (mDashTimer < mDashCooldown)
		pC->AddRectColor(pT->mPositionX + 32.f, pT->mPositionY, -3.14159/2.0f, 50.f, 6.f, (float)mDashTimer / mDashCooldown, .4, .4, .8, 1.f, .2, .2, .8, 1.f);


	/*** Milestone 3 ***/
	// pT->mAngle += (gpInputManager->IsKeyPressed(SDL_SCANCODE_E) - gpInputManager->IsKeyPressed(SDL_SCANCODE_Q)) * 2.0f * dTime;

	if (DEBUG) {
		pC->AddRectColor(pT->mPositionX, pT->mPositionY-24.f, -3.14159 / 2.0f, 48.f, 16.f, 1.f, .4, .8, .4, 0.5f, .2, .8, .2, 0.2f);
	}
}

void Controller::Jump(Transform* pT)
{
	AudioClip* pAC = static_cast<AudioClip*>(mpOwner->GetComponent(TYPE_AUDIOCLIP));
	pAC->PlayOneShot("Jump");
	pT->mVelVert = -490; // jumping
}


void Controller::HandleEvent(Event* pEvent) {
	if (pEvent->mType == EventType::COLLIDE) {
		CollideEvent* pCollideEvent = static_cast<CollideEvent*>(pEvent);
		if (pCollideEvent->mChars[1] == mpOwner->GetComponent(TYPE_CHARACTER)) {
 			Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
			if (pT != nullptr) {
				// pT->mPositionX = 30.0f;
			}
			
			// PlayerHitEvent phe;
			// gpEventManager->BroadcastEvent(&phe);

			// PlayerHitEvent* pPHE = new PlayerHitEvent();
			// pPHE->mTimer = 2.0f;
			// gpEventManager->AddTimedEvent(pPHE);

			// PlayerHitEvent phe;
			
			/*
			Event* phe = new Event(EventType::PLAYER_HIT);
			gpEventManager->BroadcastEventToSubscribers(phe);
			*/
		}
	}
	
	if (pEvent->mType == EventType::SHOVE) {
		if (pEvent->mChars[1] == mpOwner->GetComponent(TYPE_CHARACTER)) {
			Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
			Transform* pTrans = static_cast<Transform*>(pEvent->mChars[0]->mpOwner->GetComponent(TYPE_TRANSFORM));
			
			float ang = atan2f(pTrans->mPositionY - pT->mPositionY, pTrans->mPositionX - pT->mPositionX);
			/*float pushback = 1000.f;
			if (powf(pT->mPositionY - pTrans->mPositionY, 2) + powf(pT->mPositionX - pTrans->mPositionX, 2) <= powf(60.f, 2)) {
				pushback *= 3;
			}*/

			pT->mVelHoriz -= pEvent->mDamage * cosf(ang) *gpFRC->GetDeltaTime();
			pT->mVelVert -= pEvent->mDamage * sinf(ang) *gpFRC->GetDeltaTime();
		}
	}
	
}


void Controller::Serialize(rapidjson::GenericArray<false, rapidjson::Value> input) {
	if (input[0].HasMember("jumps")) {
		mJumps = input[0]["jumps"].GetFloat();
		mJumpsLeft = mJumps;
	}
}



