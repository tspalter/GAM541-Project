/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: AudioManager.cpp
Purpose: Audio Manager for Soundfx and Background Music
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Arthur chang
- End Header --------------------------------------------------------*/

#pragma once

#include "fmod.hpp"
#include "fmod_studio.hpp"
#include "common.h"
#include "Components/AudioEvent.h"
#include <vector>



class AudioManager
{
public :

	AudioManager();
	~AudioManager();

	void Update();
	void SetMasterBusVolume(float vol);
	void InitEvents();
	void PlayOneShot(std::string eventName);
	void PlayOneShot(std::string eventName, int i);
	void SetPlayed(std::string eventName, bool isPlayed);

public : 
	FMOD_RESULT result;
	FMOD::Studio::System* system = NULL;
	FMOD::System* coreSystem = NULL;
	std::vector<AudioEvent*> mEventList;
};