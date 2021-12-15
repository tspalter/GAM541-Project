
/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: AudioEvent.h
Purpose: Audio Event component
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Arthur Chang
- End Header --------------------------------------------------------*/
#pragma once

#include "Component.h"
#include "../AudioManager.h"
#include <vector>

class AudioEvent
{
public:
	AudioEvent();
	~AudioEvent();

	AudioEvent(std::string mEventName, float mEventVolume);

	void SetData(rapidjson::Value& input);
	void SetPlayed(bool isPlayed);

	std::string mEventName;
	float mEventVolume;
	bool mIsSFXPlayed = false;
	
	FMOD::Studio::EventDescription* mEventDescription = NULL;
	FMOD::Studio::EventInstance* mEventInstance = NULL;
};