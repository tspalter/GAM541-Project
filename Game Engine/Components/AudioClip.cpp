/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: AudioClip.cpp
Purpose: Audio Clip component
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Arthur Chang
- End Header --------------------------------------------------------*/

#include "AudioClip.h"

extern AudioManager* gpAudioManager;


AudioClip::AudioClip() : Component(TYPE_AUDIOCLIP)
{
	
}

AudioClip::~AudioClip()
{}



void AudioClip::Serialize(rapidjson::GenericArray<false, rapidjson::Value>input)
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

void AudioClip::Update() {}

void AudioClip::PlayOneShot(std::string eventName) 
{
	for (auto audioEvent : mEventList)
	{
		if (audioEvent->mEventName == eventName)
		{
			audioEvent->mEventInstance->start();
		}
	}
}

void AudioClip::PlayOneShot(std::string eventName, int test)
{
	for (auto audioEvent : mEventList)
	{
		if (audioEvent->mEventName == eventName && !audioEvent->mIsSFXPlayed)
		{
			audioEvent->mEventInstance->start();
			audioEvent->SetPlayed(true);
		}
	}
}

void AudioClip::SetPlayed(std::string eventName, bool isPlayed)
{
	for (auto audioEvent : mEventList)
	{
		if (audioEvent->mEventName == eventName)
		{
			audioEvent->SetPlayed(isPlayed);
		}
	}
}

