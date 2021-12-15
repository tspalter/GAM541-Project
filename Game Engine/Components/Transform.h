/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Transform.h
Purpose: Contains all transformation/translation information for an object
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne, Adam Rhoades
- End Header --------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "../Matrix2D.h"

class GameObject;

class Transform : public Component {
public:
	Transform();
	~Transform();

	void Update();

	void Serialize(std::ifstream& InputStream);
	void Serialize(rapidjson::GenericArray<false, rapidjson::Value>);

public:
	float mPositionX, mPositionY;
	float mVelHoriz, mVelVert;
	float mKnockbackHoriz, mKnockbackVert;
	float mSpriteOffsetX, mSpriteOffsetY;
	float mAngle;

	float mWidth, mHeight;
	// Matrix2D mTransformMatrix;


private:
	// float mPositionX, mPositionY;
};
