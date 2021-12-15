/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: InputManager.cpp
Purpose: Handles game inputs
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne
- End Header --------------------------------------------------------*/

#include "InputManager.h"
#include "memory.h"
#include "SDL.h"

InputManager::InputManager() {
	memset(mCurrentState, 0, 512 * sizeof(Uint8));
	memset(mPreviousState, 0, 512 * sizeof(Uint8));
	mCurrentMouse = mPreviousMouse = false;
}

InputManager::~InputManager() {
	//
}

void InputManager::Update(int screenSize[]) {
	int numberOfFetched = 0;
	const Uint8* currentKeyStates = SDL_GetKeyboardState(&numberOfFetched);

	if (numberOfFetched > 512)
		numberOfFetched = 512;

	memcpy(mPreviousState, mCurrentState, numberOfFetched * sizeof(Uint8));
	memcpy(mCurrentState, currentKeyStates, numberOfFetched * sizeof(Uint8));

	mPreviousMouse = mCurrentMouse;
	mCurrentMouse = SDL_GetMouseState(&mMouseX, &mMouseY) & SDL_BUTTON(SDL_BUTTON_LEFT);
	
	mMouseX /= (float)screenSize[0] / 1200;
	mMouseY /= (float)screenSize[1] / 800;
}

bool InputManager::IsKeyPressed(unsigned int KeyScanCode) {
	if (KeyScanCode >= 512)
		return false;

	if (mCurrentState[KeyScanCode])
		return true;
	return false;
}

bool InputManager::IsKeyTriggered(unsigned int KeyScanCode) {
	if (KeyScanCode >= 512)
		return false;

	if (mCurrentState[KeyScanCode] && !mPreviousState[KeyScanCode])
		return true;
	return false;
}

bool InputManager::IsKeyReleased(unsigned int KeyScanCode) {
	if (KeyScanCode >= 512)
		return false;

	if (mPreviousState[KeyScanCode] && !mCurrentState[KeyScanCode])
		return true;
	return false;
}

bool InputManager::IsMousePressed() {
	return mCurrentMouse;
}

bool InputManager::IsMouseTriggered() {
	return (mCurrentMouse && !mPreviousMouse);
}

bool InputManager::IsMouseReleased() {
	return (!mCurrentMouse && mPreviousMouse);
}
