/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Invincibility.cpp
Purpose: Invincibility Component
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Arthur Chang
- End Header --------------------------------------------------------*/

#include "Invincibility.h"
#include "..\FrameRateController.h"


extern FrameRateController* gpFRC;

Invincibility::Invincibility() : Component(TYPE_INVINCIBILITY)
{
	
}

Invincibility::~Invincibility()
{}



void Invincibility::Serialize(rapidjson::GenericArray<false, rapidjson::Value>input)
{
}

void Invincibility::SetInvincible(float a)
{
	invinceTimer = a;
}

void Invincibility::Update()
{
	invinceTimer -= gpFRC->GetDeltaTime();
	if (invinceTimer > 0.0f) 
	{
		isInvincible = true;
	}
	else
	{
		isInvincible = false;
	}
}

