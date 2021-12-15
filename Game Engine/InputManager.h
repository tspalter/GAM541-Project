/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: InputManager.h
Purpose: Handles game inputs
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne
- End Header --------------------------------------------------------*/

#pragma once

#include "SDL_stdinc.h"

class InputManager
{
public:
	InputManager();
	~InputManager();

	void Update( int screenSize[]);
	bool IsKeyPressed(unsigned int KeyScanCode);
	bool IsKeyTriggered(unsigned int KeyScanCode);
	bool IsKeyReleased(unsigned int KeyScanCode);

	int mMouseX, mMouseY;
	bool IsMousePressed();
	bool IsMouseTriggered();
	bool IsMouseReleased();
public:



private:
private:
	Uint8 mCurrentState[512];
	Uint8 mPreviousState[512];

	bool mCurrentMouse;
	bool mPreviousMouse;

};
