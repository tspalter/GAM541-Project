/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Invincibility.h
Purpose: Invincibility Component
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

class Invincibility : public Component {
public:
	Invincibility();
	~Invincibility();

	void Serialize(std::ifstream& InputStream) { }
	void Serialize(rapidjson::GenericArray<false, rapidjson::Value>);
	void Update();

	void SetInvincible(float a);

public:
	bool isInvincible = false;
	

private:
	float invinceTimer = 0.0;
private:
};
