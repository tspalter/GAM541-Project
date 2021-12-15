/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Objective.h
Purpose: Objective Component. Used for keys and Bosses that need to be defeated before leaving a room.
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Adam Rhoades
- End Header --------------------------------------------------------*/

#pragma once

#include "Component.h"

enum OBJECTIVE_TYPE {
	KEY,
	EXIT,
	BOSS
};

class Objective : public Component {
public:
	Objective();
	~Objective();

	void Update();
	void Serialize(std::ifstream& InputStream);
	void Serialize(rapidjson::GenericArray<false, rapidjson::Value>);

	void HandleEvent(Event* pEvent);

public:
	bool mCompleted;
	bool mIsAudioPlayed;
	unsigned int mObjectiveType;


private:
	float mRadius;
	int mDissapearDuration;
	
private:
};
