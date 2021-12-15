#pragma once

#include "Component.h"

class Sniper : public Component {
public:
	Sniper();
	~Sniper();

	void Serialize(std::ifstream& InputStream) { }
	void Serialize(rapidjson::GenericArray<false, rapidjson::Value>){ }
	void Update();
public:
	float mShotAng, mShotL, mShotW;
	int mShotTimer, mShotTime, mShotDelay;
	bool mShooting;
	// float mInner, mSlice;


private:
private:
};
