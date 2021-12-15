/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Body.h
Purpose: Handles Physics
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne, Adam Rhoades
- End Header --------------------------------------------------------*/

#pragma once

#include "Component.h"

class Body : public Component {
public:
	Body();
	~Body();

	void Update();

	void Serialize(std::ifstream& InputStream) {} // remnant of old serialization process, prior to using json; TODO strip all associated Serializations out
	void Serialize(rapidjson::GenericArray<false, rapidjson::Value>);

	void HandleEvent(Event* pEvent) {}

public:
	float mWidth, mHeight;
	bool mWall, mBounce, mSemisolid;
	bool mFriendly;



private:

};
