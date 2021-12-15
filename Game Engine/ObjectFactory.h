
/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: ObjectFactory.h
Purpose: Handler of all object-type creations
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne, Adam Rhoades, Thomas Spalter
- End Header --------------------------------------------------------*/

#pragma once

class GameObject;

class ObjectFactory {
public:
	ObjectFactory();
	~ObjectFactory();

	void RegisterCreator(const char* type, bool a);

	GameObject* LoadGameObject(const char* pFileName);
	GameObject* CreateGameObject();
	void LoadLevel(const char* pFileName);
public:


private:
private:
};
