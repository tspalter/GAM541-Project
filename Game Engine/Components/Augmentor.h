/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Augmentor.h
Purpose: Handles the augmentor's radiation telegraphs
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_3
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: November 13, 2020
- End Header --------------------------------------------------------*/

#pragma once

#include "Component.h"

class Augmentor : public Component {
public:
	Augmentor();
	~Augmentor();

	void Serialize(std::ifstream& InputStream) { }
	void Serialize(rapidjson::GenericArray<false, rapidjson::Value>);
	void Update();
public:
	float mAng;
	float mDamageTimer, mDamageTick;
	float mInner, mSlice;
	bool mPulse = false;


private:
private:
};
