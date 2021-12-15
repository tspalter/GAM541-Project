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

#include <Windows.h>
#include "ResourceManager.h"
#include "SDL_surface.h"
#include <gl/GL.h>
#include <gdiplus.h>
#include <string>
#include <assert.h>

#pragma comment (lib, "Gdiplus.lib")

ResourceManager::ResourceManager() {
	//
}

ResourceManager::~ResourceManager() {
	for (auto &pSurface : mSurfaces)
		SDL_FreeSurface(pSurface.second);

	mSurfaces.clear();

	// mTextures.clear();
}

SDL_Surface* ResourceManager::LoadSurface(const char* pFilePath) {
	return nullptr;
	// TODO remove
	/*
	// Try to fetch from the hash map
	SDL_Surface* pSurface = mSurfaces[pFilePath];
	if (pSurface)
		return pSurface;

	// Load fresh image
	pSurface = SDL_LoadBMP(pFilePath);
	if (pSurface)
		mSurfaces[pFilePath] = pSurface;

	return pSurface;
	*/
}


unsigned int ResourceManager::LoadTexture(const char* pFilePath) {
	GLuint textureID = mTextures[pFilePath];
	if (textureID)
		return textureID;

	std::string pathString(pFilePath);
	// const wchar_t* pathW = pathString.c_str();
	assert(!pathString.empty());

	Gdiplus::Bitmap bmp(std::wstring(pathString.begin(), pathString.end()).c_str());
	// Gdiplus::Bitmap bmp(L"..\\Resources\\Angry.png");
	Gdiplus::Rect rect(0, 0, bmp.GetWidth(), bmp.GetHeight());

	Gdiplus::BitmapData data;
	bmp.LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &data);
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, data.Width, data.Height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data.Scan0);

	bmp.UnlockBits(&data);

	//glBindTexture(GL_TEXTURE_2D, 0);

	mTextures[pFilePath] = textureID;

	return textureID;

	/*
	GLuint textureID;
	{
		Gdiplus::Bitmap bmp(L"..\\Resources\\Angry.png");
		Gdiplus::Rect rect(0, 0, bmp.GetWidth(), bmp.GetHeight());
		Gdiplus::BitmapData data;
		bmp.LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &data);
		glGenTextures(1, &textureID);

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, data.Width, data.Height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data.Scan0);

		bmp.UnlockBits(&data);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	*/
}
