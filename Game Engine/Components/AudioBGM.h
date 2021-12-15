/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: AudioBGM.h
Purpose: Audio Background Music
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Arthur Chang
- End Header --------------------------------------------------------*/
#pragma once

#include "Component.h"
#include "../AudioManager.h"
#include <vector>
#include "AudioEvent.h"

class AudioBGM : public Component {
public:
	AudioBGM();
	~AudioBGM();

	void Serialize(std::ifstream& InputStream) { }
	void Serialize(rapidjson::GenericArray<false, rapidjson::Value>);
	void Update();

	void PlayOneShot(std::string eventName);

public:
	
	float mVolume = 1.0f;
	int mEventCount;

	std::vector<AudioEvent*> mEventList;

private:
private:
};
