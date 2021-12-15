/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Component.cpp
Purpose: Allows subroutines added to objects to be created under a generic "component" type
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_2
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 15, 2020
- End Header --------------------------------------------------------*/

#include "Component.h"

Component::Component(unsigned int type) {
	mType = type;
	mpOwner = nullptr;
}

Component::~Component() {
	//
}
