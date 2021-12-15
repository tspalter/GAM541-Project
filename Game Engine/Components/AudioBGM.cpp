/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: AudioBGM.cpp
Purpose: Audio Background Music
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Arthur Chang
- End Header --------------------------------------------------------*/

#include "AudioBGM.h"

extern AudioManager* gpAudioManager;

/// <summary>
/// AudioClip
/// </summary>
AudioBGM::AudioBGM() : Component(TYPE_AUDIOCLIP)
{
	
}

AudioBGM::~AudioBGM()
{}



void AudioBGM::Serialize(rapidjson::GenericArray<false, rapidjson::Value>input)
{
	if (input[0].HasMember("EventCount"))
	{
		mEventCount = input[0]["EventCount"].GetInt();
	}

	for (int i = 1; i <= mEventCount; i++)
	{
		AudioEvent* newEvent = new AudioEvent();
		newEvent->SetData(input[i]);
		mEventList.push_back(newEvent);
	}
}

void AudioBGM::Update() {}

void AudioBGM::PlayOneShot(std::string eventName)
{
	for (auto audioEvent : mEventList)
	{
		if (audioEvent->mEventName == eventName)
		{
			audioEvent->mEventInstance->start();
		}
	}
}

