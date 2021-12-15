/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: AudioEvent.cpp
Purpose: Audio Event component
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Arthur Chang
- End Header --------------------------------------------------------*/

#include "AudioClip.h"

extern AudioManager* gpAudioManager;

AudioEvent::AudioEvent()
{}

AudioEvent::~AudioEvent()
{}

AudioEvent::AudioEvent(std::string eventName, float eventVolume)
{
	mEventName = eventName;
	mEventVolume = eventVolume;
	ERRCHECK(gpAudioManager->system->getEvent(("event:/" + mEventName).c_str(), &mEventDescription));
	ERRCHECK(mEventDescription->createInstance(&mEventInstance));
	ERRCHECK(mEventInstance->setVolume(mEventVolume));
}

void AudioEvent::SetPlayed(bool isPlayed)
{
	mIsSFXPlayed = isPlayed;
}

void AudioEvent::SetData(rapidjson::Value& input)
{
	if (input.HasMember("name"))
	{
		mEventName = input["name"].GetString();
		ERRCHECK(gpAudioManager->system->getEvent(("event:/" + mEventName).c_str(), &mEventDescription));
	}
	if (input.HasMember("volume"))
	{
		mEventVolume = input["volume"].GetFloat();
	}
	ERRCHECK(mEventDescription->createInstance(&mEventInstance));
	ERRCHECK(mEventInstance->setVolume(mEventVolume));
}