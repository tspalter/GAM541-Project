/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: FrameRateController.h
Purpose: Maintains the frame rate and tracks delta_time
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne
- End Header --------------------------------------------------------*/


#pragma once


class FrameRateController {
public:
	FrameRateController(unsigned int MaxFrameRate);
	~FrameRateController();

	void FrameStart();
	void FrameEnd(bool isPaused);
	unsigned int GetFrameTime();
	float GetDeltaTime();
public:


private:
	unsigned int mTickStart;
	unsigned int mTickEnd;
	unsigned int mMaxFrameRate;
	unsigned int mTicksPerFrame;
	unsigned int mFrameTime;
	float mDeltaTime;
private:

};
