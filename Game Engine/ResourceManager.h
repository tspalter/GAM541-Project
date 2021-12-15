/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: ResourceManager.cpp
Purpose: Loads and maintains resources for use
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne
- End Header --------------------------------------------------------*/

#pragma once

#include <unordered_map>
// #include <gl/GL.h>
// #include "SDL_surface.h"

struct SDL_Surface;


class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();
	
	SDL_Surface* LoadSurface(const char* pFilePath);
	unsigned int LoadTexture(const char* pFilePath);

	void ReInit() { mTextures.clear(); }
public:


private:
private:

	std::unordered_map<std::string, SDL_Surface*> mSurfaces;
	std::unordered_map<std::string, unsigned int> mTextures;
};
