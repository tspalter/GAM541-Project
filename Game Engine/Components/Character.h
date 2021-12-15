/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Character.h
Purpose: For any unit that can be interacted with in combat, handles collision area and health
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_3
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: November 8, 2020
- End Header --------------------------------------------------------*/

#pragma once

#include "Component.h"
#include <vector>

class Character : public Component {
public:
	Character();
	~Character();

	void Update();

	void Serialize(std::ifstream& InputStream);
	void Serialize(rapidjson::GenericArray<false, rapidjson::Value>);

	void HandleEvent(Event* pEvent);

public:
	void AddTelegraph(float x, float y, float ang, float width, float in, float out, float fill);
	void AddTelegraphColor(float x, float y, float ang, float width, float in, float out, float fill, float r1, float g1, float b1, float a1, float r2, float g2, float b2, float a2);
	bool CollideCirc(float x_, float y_, float ang_, float angwidth_, float in_, float out_);
	
	void AddSquare(float x, float y, float ang, float length, float width, float fill);
	void AddRect(float x, float y, float ang, float length, float width, float fill);
	void AddRectColor(float x, float y, float ang, float length, float width, float fill, float r1, float g1, float b1, float a1, float r2, float g2, float b2, float a2);
public:
	int mRadius, mShadowOffset;
	float mHP, mHPMax;
	bool mFriendly;
	bool mIsStunned;
	int mStunnedDuration;
	bool mHasHealthBar;
	bool mGodMode;

	class Telegraph {
	public:
		Telegraph(float, float, float, float, float, float, float);
		~Telegraph();
	public:
		float mX, mY;
		float mIn, mOut;
		float mAng, mAngWidth;
		float mColor1[4], mColor2[4];
		float mFill;


	private:
	};
	std::vector<Telegraph*> mTelegraphs;

	class TeleRect {
	public:
		TeleRect(float, float, float, float, float, float);
		~TeleRect();
	public:
		float mX, mY;
		float mLength, mWidth;
		float mAng;
		float mColor1[4], mColor2[4];
		float mFill;


	private:
	};
	std::vector<TeleRect*> mRectangles;

private:
	
	
private:
};
