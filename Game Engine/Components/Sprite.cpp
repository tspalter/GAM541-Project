/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Sprite.cpp
Purpose: Contains all sprite information for an object. Also handles Sprite Animation
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Adam Rhoades, Dylan Washburne
- End Header --------------------------------------------------------*/


#include "Sprite.h"
#include "..\ResourceManager.h"
#include "SDL_surface.h"
#include "..\FrameRateController.h"

extern ResourceManager* gpResourceManager;
extern FrameRateController* gpFRC;


Sprite::Sprite() : Component(TYPE_SPRITE) {
	mpSurface = nullptr;
	mIsAnimated = false;
}

Sprite::~Sprite() {
	//
}

void Sprite::Update() {
	//
	if (mIsAnimated)
		mpSpriteAnimator->Update();
}

void Sprite::Serialize(std::ifstream& InputStream) {
	std::string imageName;
	InputStream >> imageName;

	imageName = "..\\Resources\\" + imageName;
	mpSurface = gpResourceManager->LoadSurface(imageName.c_str());
}

void Sprite::Serialize(rapidjson::GenericArray<false, rapidjson::Value> input) {
	
	mIsAnimated = false;
	if (input[0].HasMember("animated")) {
		mIsAnimated = input[0]["animated"].GetBool();
		mpSpriteAnimator = new SpriteAnimator();
		mpSpriteAnimator->mParentSprite = this;
	}

	
	
	
	if (mIsAnimated){
	
		if (input[0].HasMember("idlingPhase")) {
			mpSpriteAnimator->mIdlingPhase = input[0]["idlingPhase"].GetInt();
		}


		if (input[0].HasMember("idlingImageName"))
		{
			std::string imageName = input[0]["idlingImageName"].GetString();
			mpSpriteAnimator->mIdlingTexture = gpResourceManager->LoadTexture(("..\\Resources\\" + imageName).c_str());
			unsigned int cols = 1;
			unsigned int rows = 1;

			if (input[0].HasMember("idlingColumns")) {
				cols = input[0]["idlingColumns"].GetInt();
				mpSpriteAnimator->mIdlingColumns = cols;
			}

			if (input[0].HasMember("idlingRows")) {
				rows = input[0]["idlingRows"].GetInt();
				mpSpriteAnimator->mIdlingRows = rows;
			}

			for (int j = 0; j < rows; j++)
			{
				for (int i = 0; i < cols; i++)
				{
					if (input[0].HasMember("idlingPhase"))
						mpSpriteAnimator->AddIdlingFrame(i * (1.0f / cols), j * (1.0f / rows), 0.4f);
					else
						mpSpriteAnimator->AddIdlingFrame(i * (1.0f / cols), j * (1.0f / rows), 0.2f);

				}
			}
			mpSpriteAnimator->StartIdling();
		}

		if (input[0].HasMember("phase2Name")) {
			mpSpriteAnimator->hasPhase2 = true;
			std::string imageName = input[0]["phase2Name"].GetString();
			mpSpriteAnimator->mIdlingTextureP2 = gpResourceManager->LoadTexture(("..\\Resources\\" + imageName).c_str());
			unsigned int cols = mpSpriteAnimator->mIdlingColumns;
			unsigned int rows = mpSpriteAnimator->mIdlingRows;

			for (int j = 0; j < rows; j++)
			{
				for (int i = 0; i < cols; i++)
				{
					mpSpriteAnimator->AddIdlingFrameP2(i * (1.0f / cols), j * (1.0f / rows), 0.3f);
				}
			}
			mpSpriteAnimator->StartIdling();
		}

		if (input[0].HasMember("phase3Name")) {
			mpSpriteAnimator->hasPhase3 = true;
			std::string imageName = input[0]["phase3Name"].GetString();
			mpSpriteAnimator->mIdlingTextureP3 = gpResourceManager->LoadTexture(("..\\Resources\\" + imageName).c_str());
			unsigned int cols = mpSpriteAnimator->mIdlingColumns;
			unsigned int rows = mpSpriteAnimator->mIdlingRows;

			for (int j = 0; j < rows; j++)
			{
				for (int i = 0; i < cols; i++)
				{
					mpSpriteAnimator->AddIdlingFrameP3(i * (1.0f / cols), j * (1.0f / rows), 0.2f);
				}
			}
			mpSpriteAnimator->StartIdling();
		}

		if (input[0].HasMember("runningImageName"))
		{
			std::string imageName = input[0]["runningImageName"].GetString();
			mpSpriteAnimator->mRunningTexture = gpResourceManager->LoadTexture(("..\\Resources\\" + imageName).c_str());
			unsigned int cols = 1;
			unsigned int rows = 1;

			if (input[0].HasMember("runningColumns")) {
				cols = input[0]["runningColumns"].GetInt();
				mpSpriteAnimator->mRunningColumns = cols;
			}
			if (input[0].HasMember("runningRows")) {
				rows = input[0]["runningRows"].GetInt();
				mpSpriteAnimator->mRunningRows = rows;
			}

			for (int j = 0; j < rows; j++)
			{
				for (int i = 0; i < cols; i++)
				{
					mpSpriteAnimator->AddRunningFrame(i * (1.0f / cols), j * (1.0f / rows), 0.1f);
				}
			}
		}

		if (input[0].HasMember("jumpingImageName"))
		{
			std::string imageName = input[0]["jumpingImageName"].GetString();
			mpSpriteAnimator->mJumpingTexture = gpResourceManager->LoadTexture(("..\\Resources\\" + imageName).c_str());
			unsigned int cols = 1;
			unsigned int rows = 1;

			if (input[0].HasMember("jumpingColumns")) {
				cols = input[0]["jumpingColumns"].GetInt();
				mpSpriteAnimator->mJumpingColumns = cols;
			}
			if (input[0].HasMember("jumpingRows")) {
				rows = input[0]["jumpingRows"].GetInt();
				mpSpriteAnimator->mJumpingRows = rows;
			}

			for (int j = 0; j < rows; j++)
			{
				for (int i = 0; i < cols; i++)
				{
					mpSpriteAnimator->AddJumpingFrame(i * (1.0f / cols), j * (1.0f / rows), 0.1f);
				}
			}
		}
		if (input[0].HasMember("attackingImageName"))
		{
			std::string imageName = input[0]["attackingImageName"].GetString();
			mpSpriteAnimator->mAttackingTexture = gpResourceManager->LoadTexture(("..\\Resources\\" + imageName).c_str());
			unsigned int cols = 1;
			unsigned int rows = 1;

			if (input[0].HasMember("attackingColumns")) {
				cols = input[0]["attackingColumns"].GetInt();
				mpSpriteAnimator->mAttackingColumns = cols;
			}
			if (input[0].HasMember("attackingRows")) {
				rows = input[0]["attackingRows"].GetInt();
				mpSpriteAnimator->mAttackingRows = rows;
			}

			for (int j = 0; j < rows; j++)
			{
				for (int i = 0; i < cols; i++)
				{
					mpSpriteAnimator->AddAttackingFrame(i * (1.0f / cols), j * (1.0f / rows), 0.05f);
				}
			}
		}

		if (input[0].HasMember("dashingImageName"))
		{
			std::string imageName = input[0]["dashingImageName"].GetString();
			mpSpriteAnimator->mDashingTexture = gpResourceManager->LoadTexture(("..\\Resources\\" + imageName).c_str());
			unsigned int cols = 1;
			unsigned int rows = 1;

			if (input[0].HasMember("dashingColumns")) {
				cols = input[0]["dashingColumns"].GetInt();
				mpSpriteAnimator->mDashingColumns = cols;
			}
			if (input[0].HasMember("dashingRows")) {
				rows = input[0]["dashingRows"].GetInt();
				mpSpriteAnimator->mDashingRows = rows;
			}

			for (int j = 0; j < rows; j++)
			{
				for (int i = 0; i < cols; i++)
				{
					mpSpriteAnimator->AddDashingFrame(i * (1.0f / cols), j * (1.0f / rows), 0.05f);
				}
			}
		}

		if (input[0].HasMember("fallingImageName"))
		{
			std::string imageName = input[0]["fallingImageName"].GetString();
			mpSpriteAnimator->mFallingTexture = gpResourceManager->LoadTexture(("..\\Resources\\" + imageName).c_str());
			unsigned int cols = 1;
			unsigned int rows = 1;

			if (input[0].HasMember("fallingColumns")) {
				cols = input[0]["fallingColumns"].GetInt();
				mpSpriteAnimator->mFallingColumns = cols;
			}
			if (input[0].HasMember("fallingRows")) {
				rows = input[0]["fallingRows"].GetInt();
				mpSpriteAnimator->mFallingRows = rows;
			}

			for (int j = 0; j < rows; j++)
			{
				for (int i = 0; i < cols; i++)
				{
					mpSpriteAnimator->AddFallingFrame(i * (1.0f / cols), j * (1.0f / rows), 0.1f);
				}
			}
		}
		

	}
	else
	{

		std::string imageName = input[0]["imageName"].GetString();
		mTexture = gpResourceManager->LoadTexture(("..\\Resources\\" + imageName).c_str());
		mSpritePath = imageName;
	}

	

	// mpSurface = gpResourceManager->LoadSurface(("..\\Resources\\" + imageName).c_str());
	// mpSurface = gpResourceManager->LoadSurface("..\\Resources\\Angry.bmp");
}




SpriteAnimatorFrame::SpriteAnimatorFrame(float TextureOffsetX, float TextureOffsetY, float Duration)
{
	mTextureOffsetX = TextureOffsetX;
	mTextureOffsetY = TextureOffsetY;
	mDuration = Duration;
}

SpriteAnimator::SpriteAnimator()
{
	mCurrentIndex = 0;
	mPrevIndex = 0;
	mTimer = 0.0f;
	mIdlingPhase = 1;
}
SpriteAnimator::~SpriteAnimator()
{
	for (auto pFrame : mFrames)
		delete pFrame;
	mFrames.clear();

	for (auto pFrame : mIdlingFrames)
		delete pFrame;
	mIdlingFrames.clear();

	for (auto pFrame : mIdlingFramesP2)
		delete pFrame;
	mIdlingFramesP2.clear();


	for (auto pFrame : mIdlingFramesP3)
		delete pFrame;
	mIdlingFramesP3.clear();

	for (auto pFrame : mRunningFrames)
		delete pFrame;
	mRunningFrames.clear();

	for (auto pFrame : mJumpingFrames)
		delete pFrame;
	mJumpingFrames.clear();

	for (auto pFrame : mAttackingFrames)
		delete pFrame;
	mAttackingFrames.clear();

	for (auto pFrame : mDashingFrames)
		delete pFrame;
	mDashingFrames.clear();

	for (auto pFrame : mFallingFrames)
		delete pFrame;
	mFallingFrames.clear();
}

void SpriteAnimator::AddIdlingFrame(float TextureOffsetX, float TextureOffsetY, float Duration)
{
	mIdlingFrames.push_back(new SpriteAnimatorFrame(TextureOffsetX, TextureOffsetY, Duration));
	mCanIdle = true;
}

void SpriteAnimator::AddIdlingFrameP2(float TextureOffsetX, float TextureOffsetY, float Duration)
{
	mIdlingFramesP2.push_back(new SpriteAnimatorFrame(TextureOffsetX, TextureOffsetY, Duration));
}

void SpriteAnimator::AddIdlingFrameP3(float TextureOffsetX, float TextureOffsetY, float Duration)
{
	mIdlingFramesP3.push_back(new SpriteAnimatorFrame(TextureOffsetX, TextureOffsetY, Duration));
}

void SpriteAnimator::AddRunningFrame(float TextureOffsetX, float TextureOffsetY, float Duration)
{
	mRunningFrames.push_back(new SpriteAnimatorFrame(TextureOffsetX, TextureOffsetY, Duration));
	mCanRun = true;
}

void SpriteAnimator::AddAttackingFrame(float TextureOffsetX, float TextureOffsetY, float Duration)
{
	mAttackingFrames.push_back(new SpriteAnimatorFrame(TextureOffsetX, TextureOffsetY, Duration));
	mCanAttack = true;
}

void SpriteAnimator::AddDashingFrame(float TextureOffsetX, float TextureOffsetY, float Duration)
{
	mDashingFrames.push_back(new SpriteAnimatorFrame(TextureOffsetX, TextureOffsetY, Duration));
	mCanDash = true;
}

void SpriteAnimator::AddJumpingFrame(float TextureOffsetX, float TextureOffsetY, float Duration)
{
	mJumpingFrames.push_back(new SpriteAnimatorFrame(TextureOffsetX, TextureOffsetY, Duration));
	mCanJump = true;
}

void SpriteAnimator::AddFallingFrame(float TextureOffsetX, float TextureOffsetY, float Duration)
{
	mFallingFrames.push_back(new SpriteAnimatorFrame(TextureOffsetX, TextureOffsetY, Duration));
	mCanFall = true;
}

void SpriteAnimator::Update()
{
	mTimer -= ((float)gpFRC->GetFrameTime() / 1000);
	if (mTimer <= 0) {
		mPrevIndex = mCurrentIndex;
		++mCurrentIndex;
		mCurrentIndex %= mFrames.size();
		mTimer = mFrames[mCurrentIndex]->mDuration;
		
	}
}


void SpriteAnimator::StartIdling()
{
	mCurrentIndex = 0;
	if(hasPhase2 || hasPhase3)
		mCurrentIndex = (mPrevIndex + 1) % 16;


	if (hasPhase2 && mIdlingPhase == 2)
		mFrames = mIdlingFramesP2;
	else if (hasPhase3 && mIdlingPhase == 3)
		mFrames = mIdlingFramesP3;
	else
		mFrames = mIdlingFrames;

	mIsIdling = true;
	mIsRunning = mIsJumping = mIsAttacking = mIsDashing = mIsFalling = false;
	mTimer = mFrames[mCurrentIndex]->mDuration;
	mParentSprite->mRows = mIdlingRows;
	mParentSprite->mColumns = mIdlingColumns;
	
	if (hasPhase2 && mIdlingPhase == 2)
		mParentSprite->mTexture = mIdlingTextureP2;
	else if (hasPhase3 && mIdlingPhase == 3)
		mParentSprite->mTexture = mIdlingTextureP3;
	else
		mParentSprite->mTexture = mIdlingTexture;
}

void SpriteAnimator::StartRunning()
{
	mCurrentIndex = 0;
	mFrames = mRunningFrames;
	mIsRunning = true;
	mIsIdling = mIsJumping = mIsAttacking = mIsDashing = mIsFalling = false;
	mTimer = mFrames[mCurrentIndex]->mDuration;
	mParentSprite->mRows = mRunningRows;
	mParentSprite->mColumns = mRunningColumns;
	mParentSprite->mTexture = mRunningTexture;
}

void SpriteAnimator::StartAttacking()
{
	mCurrentIndex = 0;
	mFrames = mAttackingFrames;
	mIsAttacking = true;
	mIsIdling = mIsJumping = mIsRunning = mIsDashing = mIsFalling = false;
	mTimer = mFrames[mCurrentIndex]->mDuration;
	mParentSprite->mRows = mAttackingRows;
	mParentSprite->mColumns = mAttackingColumns;
	mParentSprite->mTexture = mAttackingTexture;
}

void SpriteAnimator::StartDashing()
{
	mCurrentIndex = 0;
	mFrames = mDashingFrames;
	mIsDashing = true;
	mIsIdling = mIsJumping = mIsAttacking = mIsRunning = mIsFalling = false;
	mTimer = mFrames[mCurrentIndex]->mDuration;
	mParentSprite->mRows = mDashingRows;
	mParentSprite->mColumns = mDashingColumns;
	mParentSprite->mTexture = mDashingTexture;
}

void SpriteAnimator::StartJumping()
{
	mCurrentIndex = 0;
	mFrames = mJumpingFrames;
	mIsJumping = true;
	mIsIdling = mIsRunning = mIsAttacking = mIsDashing = mIsFalling = false;
	mTimer = mFrames[mCurrentIndex]->mDuration;
	mParentSprite->mRows = mJumpingRows;
	mParentSprite->mColumns = mJumpingColumns;
	mParentSprite->mTexture = mJumpingTexture;
}

void SpriteAnimator::StartFalling()
{
	mCurrentIndex = 0;
	mFrames = mFallingFrames;
	mIsFalling = true;
	mIsIdling = mIsJumping = mIsAttacking = mIsDashing = mIsRunning = false;
	mTimer = mFrames[mCurrentIndex]->mDuration;
	mParentSprite->mRows = mFallingRows;
	mParentSprite->mColumns = mFallingColumns;
	mParentSprite->mTexture = mFallingTexture;
}



std::pair<float, float> SpriteAnimator::GetTextureCoords()
{
	return std::pair<float, float>(mFrames[mCurrentIndex]->mTextureOffsetX, mFrames[mCurrentIndex]->mTextureOffsetY);
}