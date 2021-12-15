/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Sprite.cpp
Purpose: Initializes and loops over the game contents. Handles Menus.
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne, Adam Rhoades, Thomas Spalter, Arthur Chang
- End Header --------------------------------------------------------*/


#include <Windows.h>

#include <SDL.h>
#include "glew.h"
#include <gl/GL.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>


#include "stdio.h"

#include <iostream>
#include <string>

#include "Matrix2D.h"

#include "ObjectFactory.h"
#include "InputManager.h"
#include "FrameRateController.h"
#include "ResourceManager.h"
#include "CollisionManager.h"
#include "Moderator.h"
#include "StealthModerator.h"
#include "LoadShaders.h"

#include "GameObject.h"
#include "GameObjectManager.h"

#include "Components/Component.h"
#include "Components/Controller.h"
#include "Components/Transform.h"
#include "Components/Sprite.h"
#include "Components/Character.h"
#include "Components/Body.h"
#include "Components/LeftRight.h"
#include "Components/BossAttack.h"
#include "Components/Eye.h"
#include "Components/AudioClip.h"

#include <gdiplus.h>
#include "../glm/glm/glm.hpp"
#include "../glm/glm/gtc/matrix_transform.hpp"

#include "AudioManager.h"

#pragma comment (lib, "Gdiplus.lib")

# define PI           3.14159265358979323846  /* pi */


// #pragma comment (lib, "opengl32.lib")
// #pragma comment (lib, "glew32.lib")

typedef int32_t i32;
typedef uint32_t u32;
typedef int32_t b32;

InputManager* gpInputManager;
FrameRateController* gpFRC;
ResourceManager* gpResourceManager;
GameObjectManager* gpGameObjectManager;
ObjectFactory* gpObjectFactory;
CollisionManager* gpCollisionManager;
EventManager* gpEventManager;
//Moderator* gpModerator;
StealthModerator* gpStealthModerator;

AudioManager* gpAudioManager;

bool DEBUG;
int gGameType = 3;

FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}


#pragma comment(lib, "legacy_stdio_definitions.lib")


GLint gRenderID, gCircID, gRectID;



static Uint8* audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play

void my_audio_callback(void* userdata, Uint8* stream, int len) {

	if (audio_len == 0)
		return;

	len = (len > audio_len ? audio_len : len);
	//SDL_memcpy (stream, audio_pos, len); 					// simply copy from one buffer into the other
	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another

	audio_pos += len;
	audio_len -= len;
}

void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}

void triggerScreenChange()
{

}


int main(int argc, char* args[])
{

	/*
	if (AllocConsole()) {
		FILE* file;

		freopen_s(&file, "SONOUT$", "wt", stdout);
		freopen_s(&file, "SONOUT$", "wt", stderr);
		freopen_s(&file, "SONOUT$", "wt", stdin);

		SetConsoleTitle(L"SDL 2.0 Test");
	}
	*/
	//int w, h;
	//GetDesktopResolution(w, h);
	int screenSize[] = { 1200, 800 };

	SDL_Window *pWindow;
	int error = 0;
	bool appIsRunning = true;
	bool appIsPaused = false;
	bool optionsMenuOpen = false;
	bool controlsMenuOpen = false;
	bool confirmationWindowOpen = false;
	bool confirmationWindowOutput = false;
	bool appIsFullscreen = false;
	int mTimer = 0;
	int backgroundFrame = 0;
	bool init = true;
	bool fullscreenToggle = false;
	float currentVolume = 1.0f;
	bool creditsPending = false;
	bool exitPending = false;
	bool mainMenuPending = false;
	bool fullScreenTogglePending = false;

	SDL_Surface* pWindowSurface = NULL;
	SDL_Surface* pImageSurface = NULL;

	gpInputManager = new InputManager();
	gpFRC = new FrameRateController(75);			// ok I know it's odd but my monitor is 75Hz
	gpResourceManager = new ResourceManager();
	gpGameObjectManager = new GameObjectManager();
	gpObjectFactory = new ObjectFactory();
	gpCollisionManager = new CollisionManager();
	gpEventManager = new EventManager();
	//gpModerator = new Moderator();
	gpStealthModerator = new StealthModerator();

	gpAudioManager = new AudioManager();
	gpAudioManager->InitEvents();

	DEBUG = false;

	SDL_GLContext openGL_Context;

	// Initialize SDL
	if((error = SDL_Init( SDL_INIT_VIDEO )) < 0 )
	{
		printf("Couldn't initialize SDL, error %i\n", error);
		return 1;
	}

	// OpenGL 2.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	

	pWindow = SDL_CreateWindow("ConCaveity",		// window title
		SDL_WINDOWPOS_UNDEFINED,					// initial x position
		SDL_WINDOWPOS_UNDEFINED,					// initial y position
		screenSize[0],										// width, in pixels
		screenSize[1],										// height, in pixels
		SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);

	// Check that the window was successfully made
	if (NULL == pWindow)
	{
		// In the event that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	// Create the context, and make it current
	openGL_Context = SDL_GL_CreateContext(pWindow);


	

	if (glewInit() != GLEW_OK)
		printf("Couldn't init GLEW library\n");

	
	// Initialize GDIplus
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	
	LoadShaders();


	int levelNo = 0;

	// gpObjectFactory->LoadLevel(("..\\Resources\\Level" + std::to_string(levelNo) + ".json").c_str());

	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);


	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	

	// 0 -> Positions
	// 1 -> Colors
	// 2 -> Texels
	GLuint bufferIDs[3];

	glGenBuffers(3, bufferIDs);
	
	int vertexNum = 4;
	
	int coordsPerPosition = 3;
	int coordsPerColor = 4;
	int coordsPerTex = 2;

	int positionStride	= coordsPerPosition * sizeof(float);
	int colorStride		= coordsPerColor * sizeof(float);
	int texStride		= coordsPerTex * sizeof(float);

	GLfloat* pPositions = new GLfloat[vertexNum * coordsPerPosition];
	GLfloat* pColors = new GLfloat[vertexNum * coordsPerColor];
	GLfloat* pTex = new GLfloat[vertexNum * coordsPerTex];

	// Vertex 1
	pPositions[0] = -0.5f;	pPositions[1] = -0.5f;	pPositions[2] = 0.0f;
	pColors[0] = 1.0f;		pColors[1] = .0f;		pColors[2] = .0f;		pColors[3] = 1.0f;
	pTex[0] = 0.0f;			pTex[1] = 1.0f;

	// Vertex 2
	pPositions[3] = 0.5f;	pPositions[4] = -0.5f;	pPositions[5] = 0.0f;
	pColors[4] = .0f;		pColors[5] = 1.0f;		pColors[6] = .0f;		pColors[7] = 1.0f;
	pTex[2] = 1.0f;			pTex[3] = 1.0f;

	// Vertex 3
	pPositions[6] = 0.5f;	pPositions[7] = 0.5f;	pPositions[8] = 0.0f;
	pColors[8] = .0f;		pColors[9] = .0f;		pColors[10] = 1.0f;		pColors[11] = 1.0f;
	pTex[4] = 1.0f;			pTex[5] = 0.0f;

	
	// Vertex 4
	pPositions[9] = -0.5f;	pPositions[10] = 0.5f;	pPositions[11] = 0.0f;
	pColors[12] = 1.0f;		pColors[13] = 1.0f;		pColors[14] = 1.0f;		pColors[15] = 1.0f;
	pTex[6] = 0.0f;			pTex[7] = 0.0f;
	

	// Copy from RAM to VRAM
	//Positions
	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, vertexNum * positionStride, pPositions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, coordsPerPosition, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Colors
	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, vertexNum * colorStride, pColors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, coordsPerColor, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Textures
	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, vertexNum * texStride, pTex, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, coordsPerTex, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);

	

	// Block until all OpenGL executions are complete
	glFinish();

	delete[] pPositions;
	delete[] pColors;
	delete[] pTex;

	//GLuint menuScreen = gpResourceManager->LoadTexture("../Resources/529_title.png");
	GLuint endScreen = gpResourceManager->LoadTexture("../Resources/529_end.png");
	GLuint deadScreen = gpResourceManager->LoadTexture("../Resources/529_dead.png");
	//GLuint backgroundImg = gpResourceManager->LoadTexture("../Resources/Bricks_Background.png");
	GLuint backgroundImg0 = gpResourceManager->LoadTexture("../Resources/Bricks_Background_Torches_0.png");
	GLuint backgroundImg1 = gpResourceManager->LoadTexture("../Resources/Bricks_Background_Torches_1.png");
	GLuint backgroundCredits = gpResourceManager->LoadTexture("../Resources/Credits_Background.png");
	/*
	GLuint pauseScreen = gpResourceManager->LoadTexture("../Resources/Concaveity_Pause.png");
	GLuint continueButton = gpResourceManager->LoadTexture("../Resources/Continue_Button.png");
	GLuint controlsText = gpResourceManager->LoadTexture("../Resources/Controls_Text.png");
	GLuint creditsButton = gpResourceManager->LoadTexture("../Resources/Credits_Button.png");
	GLuint mainMenuButton = gpResourceManager->LoadTexture("../Resources/Main_Menu_Button.png");
	GLuint optionsButton = gpResourceManager->LoadTexture("../Resources/Options_Button.png");
	GLuint quitButton = gpResourceManager->LoadTexture("../Resources/Quit_Button.png");
	GLuint startGameButton = gpResourceManager->LoadTexture("../Resources/Start_Game_Button.png");

	GLuint optionsText = gpResourceManager->LoadTexture("../Resources/Options_Text.png");
	GLuint fullScreenToggleButton = gpResourceManager->LoadTexture("../Resources/Full_Screen_Toggle_Button.png");
	GLuint soundOnButton = gpResourceManager->LoadTexture("../Resources/Sound_On_Button.png");
	GLuint soundOffButton = gpResourceManager->LoadTexture("../Resources/Sound_Off_Button.png");
	GLuint backButton = gpResourceManager->LoadTexture("../Resources/Back_Button.png");

	GLuint confirmationScreen = gpResourceManager->LoadTexture("../Resources/Confirmation_Text.png");
	GLuint confirmationRestartScreen = gpResourceManager->LoadTexture("../Resources/Confirmation_Text_Restart.png");
	GLuint yesButton = gpResourceManager->LoadTexture("../Resources/Yes_Button.png");
	GLuint noButton = gpResourceManager->LoadTexture("../Resources/No_Button.png");
	*/



	GLuint pauseScreen = gpResourceManager->LoadTexture("../Resources/Concaveity_Pause.png");
	GLuint continueButton = gpResourceManager->LoadTexture("../Resources/Continue_Button.png");
	GLuint continueButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Continue_Button_Highlighted.png");
	GLuint controlsText = gpResourceManager->LoadTexture("../Resources/Controls_Text.png");
	GLuint creditsButton = gpResourceManager->LoadTexture("../Resources/Credits_Button.png");
	GLuint creditsButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Credits_Button_Highlighted.png");
	GLuint mainMenuButton = gpResourceManager->LoadTexture("../Resources/Main_Menu_Button.png");
	GLuint mainMenuButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Main_Menu_Button_Highlighted.png");
	GLuint optionsButton = gpResourceManager->LoadTexture("../Resources/Options_Button.png");
	GLuint optionsButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Options_Button_Highlighted.png");
	GLuint controlsButton = gpResourceManager->LoadTexture("../Resources/Controls_Button.png");
	GLuint controlsButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Controls_Button_Highlighted.png");
	GLuint quitButton = gpResourceManager->LoadTexture("../Resources/Quit_Button.png");
	GLuint quitButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Quit_Button_Highlighted.png");
	GLuint startGameButton = gpResourceManager->LoadTexture("../Resources/Start_Game_Button.png");
	GLuint startGameButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Start_Game_Button_Highlighted.png");

	GLuint optionsText = gpResourceManager->LoadTexture("../Resources/Options_Text.png");
	GLuint fullScreenToggleButton = gpResourceManager->LoadTexture("../Resources/Full_Screen_Toggle_Button.png");
	GLuint fullScreenToggleButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Full_Screen_Toggle_Button_Highlighted.png");
	GLuint soundOnButton = gpResourceManager->LoadTexture("../Resources/Sound_On_Button.png");
	GLuint soundOnButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Sound_On_Button_Highlighted.png");
	GLuint soundOffButton = gpResourceManager->LoadTexture("../Resources/Sound_Off_Button.png");
	GLuint soundOffButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Sound_Off_Button_Highlighted.png");
	GLuint backButton = gpResourceManager->LoadTexture("../Resources/Back_Button.png");
	GLuint backButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Back_Button_Highlighted.png");

	GLuint confirmationScreen = gpResourceManager->LoadTexture("../Resources/Confirmation_Text.png");
	GLuint confirmationRestartScreen = gpResourceManager->LoadTexture("../Resources/Confirmation_Text_Restart.png");
	GLuint yesButton = gpResourceManager->LoadTexture("../Resources/Yes_Button.png");
	GLuint yesButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Yes_Button_Green.png");
	GLuint noButton = gpResourceManager->LoadTexture("../Resources/No_Button.png");
	GLuint noButtonHighlighted = gpResourceManager->LoadTexture("../Resources/No_Button_Red.png");



	/****************/
	// float transformationMatrix[16];
	float angle = 0.5f;
	int val;
	glm::mat4 projectionMatrix = glm::ortho(0.f, (float)screenSize[0], (float)screenSize[1], 0.f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//gpModerator->mStage = 0;
	gpStealthModerator->mStage = -3;
	gpStealthModerator->mTransitionTimer = 3000;
	//gpObjectFactory->LoadLevel("..\\Resources\\Opening0.json");

	// Game loop
	while(true == appIsRunning)
	{
		
		gpAudioManager->system->update();

		// Get the time at the start of the frame
		gpFRC->FrameStart();

		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				appIsRunning = false;
			}
		} // done with handling events

		gpInputManager->Update(screenSize);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// SDL_FillRect(pWindowSurface, NULL, 0xBBBBBB);

		gpEventManager->Update(gpFRC->GetDeltaTime());
		
		for (auto pColl : gpCollisionManager->mContacts) {
			pColl->mChars[0]->mpOwner->HandleEvent(pColl);
			pColl->mChars[1]->mpOwner->HandleEvent(pColl);
			// gpEventManager->BroadcastEvent(pColl);
			delete pColl;
		}
		gpCollisionManager->mContacts.clear();

		if (!appIsPaused)
		{

			// update
			for (auto pGameObject : gpGameObjectManager->mGameObjects)
				pGameObject->Update();

			for (auto pGameObject : gpGameObjectManager->mGameObjects) {
				if (pGameObject->mDestroy) {
					gpGameObjectManager->DeleteObject(pGameObject);
				}
			}
		}

		glBindVertexArray(vaoID);

		/*******************************/
		/*******************************/
		/*******************************/

		gpGameObjectManager->mDestroy = false;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_ADD);

		if (gGameType == 1 || 3){
			glUseProgram(gRenderID);

			glm::mat4 model(1.0f);
			model = glm::translate(model, glm::vec3(0, 0, 0));
			model = glm::scale(model, glm::vec3(2.f, 2.f, 0.0f));

			int transformationHandle = 4;
			glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

			glActiveTexture(GL_TEXTURE0);


			mTimer -= ((float)gpFRC->GetFrameTime() / 1000);
			if (mTimer <= 0) {
				backgroundFrame++;
				backgroundFrame %= 2;
				mTimer = 25; 
			}

			if (gpStealthModerator->mStage < 0 || gpStealthModerator->mStage > 99)
			{
				glBindTexture(GL_TEXTURE_2D, backgroundCredits);
			}
			else if(backgroundFrame == 0)
				glBindTexture(GL_TEXTURE_2D, backgroundImg0);
			else
				glBindTexture(GL_TEXTURE_2D, backgroundImg1);



			glDrawArrays(GL_QUADS, 0, vertexNum);
		}


		// Update Moderator
		gpStealthModerator->Update();

		//if (gGameType == 1 || gGameType == 2)
			//gpModerator->Update();
		//else
		/*if ((gGameType != 3 && gpModerator->mStage == 0) || (gGameType == 3 && gpStealthModerator->mStage == 0)) {
			glUseProgram(gRenderID);
			// glBindVertexArray(vaoID);

			//for (auto pGameObject : gpGameObjectManager->mGameObjects) {
				

				// glBindVertexArray(vaoID);

				glm::mat4 model(1.0f);
				model = glm::scale(model, glm::vec3(2.f, 2.f, 0.0f));

				// model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				//glBindTexture(GL_TEXTURE_2D, menuScreen);


				//glDrawArrays(GL_QUADS, 0, vertexNum);

				// glBindVertexArray(0);
			//}
		}*/


		if (!appIsPaused)
		{
			gpAudioManager->SetMasterBusVolume(1.0f * currentVolume);
		}

		glClear(GL_DEPTH_BUFFER_BIT);
		if ( gpStealthModerator->mStage == 666) {
			glClear(GL_DEPTH_BUFFER_BIT);

			glUseProgram(gRenderID);
			gpAudioManager->SetMasterBusVolume(0.4f * currentVolume);
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(600.f, 400.0f, 0.f));
				model = glm::scale(model, glm::vec3(1200.0f, -800.0f, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, deadScreen);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}




			// Yes Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(716, 250, 1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 225 && gpInputManager->mMouseY < 275 && gpInputManager->mMouseX > 616 && gpInputManager->mMouseX < 816)
					glBindTexture(GL_TEXTURE_2D, yesButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, yesButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}

			if (gpInputManager->IsMouseTriggered()) {
				if (gpInputManager->mMouseY > 225 && gpInputManager->mMouseY < 275 && gpInputManager->mMouseX > 616 && gpInputManager->mMouseX < 816) {
					gpStealthModerator->mManualRestart = true;
					gpAudioManager->PlayOneShot("Click");
				}
			}
			// End Yes Button

			// No Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(515, 250,  1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 225 && gpInputManager->mMouseY < 275 && gpInputManager->mMouseX > 416 && gpInputManager->mMouseX < 616)
					glBindTexture(GL_TEXTURE_2D, noButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, noButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}

			if (gpInputManager->IsMouseTriggered() || mainMenuPending) {
				if (gpInputManager->mMouseY > 225 && gpInputManager->mMouseY < 275 && gpInputManager->mMouseX > 416 && gpInputManager->mMouseX < 616 && !confirmationWindowOpen) {
					gpAudioManager->PlayOneShot("Click");
					mainMenuPending = true;
					confirmationWindowOpen = true;
				}
				if (!confirmationWindowOpen)
				{
					if (confirmationWindowOutput)
					{
						gpStealthModerator->mStage = 0;
						gpStealthModerator->mTransitionTimer = 2000;
						gpGameObjectManager->~GameObjectManager();
						gpStealthModerator->mManualRestart = false;
						gpStealthModerator->mManualOverride = false;
						gpStealthModerator->mManualBack = false;
						gpObjectFactory->LoadLevel("..\\Resources\\Title.json");
						appIsPaused = false;
						confirmationWindowOutput = false;
					}
					mainMenuPending = false;					
				}
			}


			if (gpInputManager->IsMouseTriggered() || exitPending) {
				if (gpInputManager->mMouseY > 725 && gpInputManager->mMouseY < 775 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen) {
					exitPending = true;
					confirmationWindowOpen = true;

				}
				if (!confirmationWindowOpen)
				{
					if (confirmationWindowOutput)
					{
						appIsRunning = false;	// remember when setting buttons that translation sets the center and scale expands it in both directions
						confirmationWindowOutput = false;
					}
					exitPending = false; 
				}
			}
			// End No Button
		}
		else if (gpStealthModerator->mStage == 99) {

			glClear(GL_DEPTH_BUFFER_BIT);

			glUseProgram(gRenderID);
			
			gpAudioManager->SetMasterBusVolume(0.4f * currentVolume);
			{
				gpAudioManager->PlayOneShot("Victory", 0);
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(600.f, 400.0f, 0.f));
				model = glm::scale(model, glm::vec3(1200.0f, -800.0f, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, endScreen);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}

			// Main Menu Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(150, 600.f, 1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 575 && gpInputManager->mMouseY < 625 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen)
					glBindTexture(GL_TEXTURE_2D, mainMenuButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, mainMenuButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}

			if (gpInputManager->IsMouseTriggered()) {
				if (gpInputManager->mMouseY > 575 && gpInputManager->mMouseY < 625 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen) {
					gpStealthModerator->mStage = 0;
					gpStealthModerator->mTransitionTimer = 2000;
					gpGameObjectManager->~GameObjectManager();
					gpStealthModerator->mManualRestart = false;
					gpStealthModerator->mManualOverride = false;
					gpStealthModerator->mManualBack = false;
					gpObjectFactory->LoadLevel("..\\Resources\\Title.json");
					appIsPaused = false;
					gpAudioManager->PlayOneShot("Click");
					gpAudioManager->SetPlayed("Victory", false);
				}
						
			}
			// Main Menu Button

			// Credits Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(150, 500.f, 1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 475 && gpInputManager->mMouseY < 525 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen)
					glBindTexture(GL_TEXTURE_2D, creditsButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, creditsButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}


			if (gpInputManager->IsMouseTriggered()) {
				if (gpInputManager->mMouseY > 475 && gpInputManager->mMouseY < 525 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen) {
					gpStealthModerator->mStage = 100;
					gpStealthModerator->mTransitionTimer = 2000;
					gpGameObjectManager->~GameObjectManager();
					gpStealthModerator->mManualRestart = false;
					gpStealthModerator->mManualOverride = false;
					gpStealthModerator->mManualBack = false;
					gpObjectFactory->LoadLevel("..\\Resources\\Credits0.json");
					gpAudioManager->PlayOneShot("Click");
					gpAudioManager->SetPlayed("Victory", false);
				}
			}
			// End Credits Button

			// Quit Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(150, 750.f, 1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 725 && gpInputManager->mMouseY < 775 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen)
					glBindTexture(GL_TEXTURE_2D, quitButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, quitButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}

			if (gpInputManager->IsMouseTriggered() || exitPending) {
				if (gpInputManager->mMouseY > 725 && gpInputManager->mMouseY < 775 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen) {
					exitPending = true;
					confirmationWindowOpen = true;

				}
				if (!confirmationWindowOpen)
				{
					if (confirmationWindowOutput)
					{
						gpAudioManager->PlayOneShot("Click");
						appIsRunning = false;	// remember when setting buttons that translation sets the center and scale expands it in both directions
						confirmationWindowOutput = false;
					}
					exitPending = false;
				}
			}
			// End Quit Button


			glDrawArrays(GL_QUADS, 0, vertexNum);
		}
		else {

			// Render terrain
			glUseProgram(gRenderID);
			for (auto pGameObject : gpGameObjectManager->mGameObjects) {
				Transform* pT = static_cast<Transform*>(pGameObject->GetComponent(TYPE_TRANSFORM));
				Sprite* pS = static_cast<Sprite*>(pGameObject->GetComponent(TYPE_SPRITE));
				Controller* pC = static_cast<Controller*>(pGameObject->GetComponent(TYPE_PLAYER_CONTROLLER));
				Body* pB = static_cast<Body*>(pGameObject->GetComponent(TYPE_BODY));

				if (pB == nullptr || !pB->mWall) { continue; }

				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(pT->mPositionX + pT->mSpriteOffsetX, pT->mPositionY + pT->mSpriteOffsetY - pT->mHeight / 2.0f, -.95));
				model = glm::rotate(model, pT->mAngle, glm::vec3(0, 0, 1));
				if (pS->mIsAnimated && pS->mpSpriteAnimator->mIsAttacking)
				{
					if (pC->mSwingAng < -PI / 2 || pC->mSwingAng > PI / 2)
						model = glm::scale(model, glm::vec3(-pT->mWidth, -pT->mHeight, 0.0f));
					else
						model = glm::scale(model, glm::vec3(pT->mWidth, -pT->mHeight, 0.0f));
				}
				else if (pT->mVelHoriz >= 0)
					model = glm::scale(model, glm::vec3(pT->mWidth, -pT->mHeight, 0.0f));
				else
					model = glm::scale(model, glm::vec3(-pT->mWidth, -pT->mHeight, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, pS->mTexture);

				if (pS->mIsAnimated)
				{

					GLfloat* pTex = new GLfloat[vertexNum * coordsPerTex];

					auto currentTextureOffset = pS->mpSpriteAnimator->GetTextureCoords();

					pTex[0] = 0.0f;			pTex[1] = 0.5f;
					pTex[2] = 0.25;			pTex[3] = 0.5f;
					pTex[4] = 0.25f;		pTex[5] = 0.0f;
					pTex[6] = 0.0f;			pTex[7] = 0.0f;

					pTex[0] = currentTextureOffset.first;
					pTex[1] = currentTextureOffset.second + (1.0f / pS->mRows);
					pTex[2] = currentTextureOffset.first + (1.0f / pS->mColumns);
					pTex[3] = currentTextureOffset.second + (1.0f / pS->mRows);
					pTex[4] = currentTextureOffset.first + (1.0f / pS->mColumns);
					pTex[5] = currentTextureOffset.second;
					pTex[6] = currentTextureOffset.first;
					pTex[7] = currentTextureOffset.second;


					glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[2]);
					glBufferData(GL_ARRAY_BUFFER, vertexNum * texStride, pTex, GL_STATIC_DRAW);
					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, coordsPerTex, GL_FLOAT, false, 0, 0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);


					glDrawArrays(GL_QUADS, 0, vertexNum);

					pTex[0] = 0.0f;			pTex[1] = 1.0f;
					pTex[2] = 1.0f;			pTex[3] = 1.0f;
					pTex[4] = 1.0f;			pTex[5] = 0.0f;
					pTex[6] = 0.0f;			pTex[7] = 0.0f;
					glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[2]);
					glBufferData(GL_ARRAY_BUFFER, vertexNum * texStride, pTex, GL_STATIC_DRAW);
					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, coordsPerTex, GL_FLOAT, false, 0, 0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				}
				else
				{

					glDrawArrays(GL_QUADS, 0, vertexNum);
				}
			}

			// Render Shadows (which indicate collision areas)
			

			// glUseProgram(gCircID);
			// glBindVertexArray(vaoID);

			for (auto pGameObject : gpGameObjectManager->mGameObjects) {
				glUseProgram(gCircID);

				Transform* pT = static_cast<Transform*>(pGameObject->GetComponent(TYPE_TRANSFORM));
				Character* pC = static_cast<Character*>(pGameObject->GetComponent(TYPE_CHARACTER));

				if (!pC)
					continue;

				// glBindVertexArray(vaoID);
				int val;

				if(DEBUG)
				{
					glm::mat4 model(1.0f);
					model = glm::translate(model, glm::vec3(pT->mPositionX, pT->mPositionY, -0.9f));
					model = glm::scale(model, glm::vec3(pC->mRadius * 2.f + (DEBUG ? 0.f : pC->mShadowOffset * 2.0f), pC->mRadius * 2.f + (DEBUG ? 0.f : pC->mShadowOffset * 2.0f), 0.0f));

					model = projectionMatrix * model;

					val = glGetUniformLocation(gCircID, "uColor1");
					glUniform4f(val, 0.2, 0.5, 0.2, 0.5);

					val = glGetUniformLocation(gCircID, "uColor2");
					glUniform4f(val, 0.0, 0.0, 0.8, 1.0);

					val = glGetUniformLocation(gCircID, "uFill");
					glUniform1f(val, 1.0);

					val = glGetUniformLocation(gCircID, "uIn");
					glUniform1f(val, 0.0);

					val = glGetUniformLocation(gCircID, "uAngs");
					glUniform2f(val, -10.0, 10.0);


					int transformationHandle = 4;
					glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

					glDrawArrays(GL_QUADS, 0, vertexNum);
				}

				/*** Telegraphs ***/


				while (!pC->mTelegraphs.empty()) {
					// for (auto pRect : pC->mTelegraphs) {
					Character::Telegraph* pTelegraph = pC->mTelegraphs.at(0);
					glm::mat4 model(1.0f);
					model = glm::translate(model, glm::vec3(pTelegraph->mX, pTelegraph->mY, -.9));
					model = glm::rotate(model, pTelegraph->mAng, glm::vec3(0, 0, 1));
					model = glm::scale(model, glm::vec3(pTelegraph->mOut * 2.f, pTelegraph->mOut * 2.f, 0.0f));

					model = projectionMatrix * model;

					val = glGetUniformLocation(gCircID, "uColor1");
					glUniform4f(val, pTelegraph->mColor1[0], pTelegraph->mColor1[1], pTelegraph->mColor1[2], pTelegraph->mColor1[3]);

					val = glGetUniformLocation(gCircID, "uColor2");
					glUniform4f(val, pTelegraph->mColor2[0], pTelegraph->mColor2[1], pTelegraph->mColor2[2], pTelegraph->mColor2[3]);

					val = glGetUniformLocation(gCircID, "uFill");
					glUniform1f(val, pTelegraph->mFill);

					val = glGetUniformLocation(gCircID, "uIn");
					glUniform1f(val, pTelegraph->mIn / pTelegraph->mOut);

					val = glGetUniformLocation(gCircID, "uAngs");
					glUniform2f(val, -pTelegraph->mAngWidth, pTelegraph->mAngWidth);

					int transformationHandle = 4;
					glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

					glDrawArrays(GL_QUADS, 0, vertexNum);

					delete pTelegraph;
					pC->mTelegraphs.erase(pC->mTelegraphs.begin());
				}


				glUseProgram(gRectID);
				while (!pC->mRectangles.empty()) {
					// for (auto pRect : pC->mTelegraphs) {
					Character::TeleRect* pRect = pC->mRectangles.at(0);
					glm::mat4 model(1.0f);
					model = glm::translate(model, glm::vec3(pRect->mX, pRect->mY, -.9));
					model = glm::rotate(model, pRect->mAng, glm::vec3(0, 0, 1));
					model = glm::scale(model, glm::vec3(pRect->mLength, pRect->mWidth, 0.0f));

					model = projectionMatrix * model;

					val = glGetUniformLocation(gRectID, "uColor1");
					glUniform4f(val, pRect->mColor1[0], pRect->mColor1[1], pRect->mColor1[2], pRect->mColor1[3]);

					val = glGetUniformLocation(gRectID, "uColor2");
					glUniform4f(val, pRect->mColor2[0], pRect->mColor2[1], pRect->mColor2[2], pRect->mColor2[3]);

					val = glGetUniformLocation(gRectID, "uFill");
					glUniform1f(val, pRect->mFill);

					int transformationHandle = 4;
					glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

					glDrawArrays(GL_QUADS, 0, vertexNum);

					delete pRect;
					pC->mRectangles.erase(pC->mRectangles.begin());
				}



				// glBindVertexArray(0);
			}
			// glBindVertexArray(0);



			// Render units
			/*
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBlendEquation(GL_FUNC_ADD);
			*/

			glUseProgram(gRenderID);
			// glBindVertexArray(vaoID);

			for (auto pGameObject : gpGameObjectManager->mGameObjects) {
				Transform* pT = static_cast<Transform*>(pGameObject->GetComponent(TYPE_TRANSFORM));
				Sprite* pS = static_cast<Sprite*>(pGameObject->GetComponent(TYPE_SPRITE));
				Controller* pC = static_cast<Controller*>(pGameObject->GetComponent(TYPE_PLAYER_CONTROLLER));
				Body* pB = static_cast<Body*>(pGameObject->GetComponent(TYPE_BODY));
				LeftRight* pLR = static_cast<LeftRight*>(pGameObject->GetComponent(TYPE_LEFT_RIGHT));
				BossAttack* pBA = static_cast<BossAttack*>(pGameObject->GetComponent(TYPE_BOSS_ATTACK));
				Eye* pEye = static_cast<Eye*>(pGameObject->GetComponent(TYPE_EYE));

				if (pB != nullptr && pB->mWall) { continue; }
				//glBindVertexArray(vaoID);

				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(pT->mPositionX + pT->mSpriteOffsetX, pT->mPositionY + pT->mSpriteOffsetY - pT->mHeight / 2.0f, (pEye != nullptr ? 1.0 : (pT->mPositionY - screenSize[1] / 2) / screenSize[1])));
				model = glm::rotate(model, pT->mAngle, glm::vec3(0, 0, 1));
				if (pS->mIsAnimated && pS->mpSpriteAnimator->mIsAttacking && pC != nullptr)
				{
					if (pC->mSwingAng < -PI/2 || pC->mSwingAng > PI/2)
						model = glm::scale(model, glm::vec3(-pT->mWidth, -pT->mHeight, 0.0f));
					else
						model = glm::scale(model, glm::vec3(pT->mWidth, -pT->mHeight, 0.0f));
				}
				else if (pS->mIsAnimated && pS->mpSpriteAnimator->mIsAttacking && pLR != nullptr)
				{
					if (pLR->mAttackAngle < -PI / 2 || pLR->mAttackAngle > PI / 2)
						model = glm::scale(model, glm::vec3(-pT->mWidth, -pT->mHeight, 0.0f));
					else
						model = glm::scale(model, glm::vec3(pT->mWidth, -pT->mHeight, 0.0f));
				}
				else if (pS->mIsAnimated && pS->mpSpriteAnimator->mIsAttacking && pBA != nullptr)
				{
					if (pBA->mAttackAngle < -PI / 2 || pBA->mAttackAngle > PI / 2)
						model = glm::scale(model, glm::vec3(pT->mWidth, -pT->mHeight, 0.0f));
					else
						model = glm::scale(model, glm::vec3(pT->mWidth, -pT->mHeight, 0.0f));
				}
				else if(pT->mVelHoriz >= 0)
					model = glm::scale(model, glm::vec3(pT->mWidth, -pT->mHeight, 0.0f));
				else
					model = glm::scale(model, glm::vec3(-pT->mWidth, -pT->mHeight, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, pS->mTexture);

				if (pS->mIsAnimated)
				{

					GLfloat* pTex = new GLfloat[vertexNum * coordsPerTex];

					auto currentTextureOffset = pS->mpSpriteAnimator->GetTextureCoords();

					pTex[0] = 0.0f;			pTex[1] = 0.5f;
					pTex[2] = 0.25;			pTex[3] = 0.5f;
					pTex[4] = 0.25f;		pTex[5] = 0.0f;
					pTex[6] = 0.0f;			pTex[7] = 0.0f;

					pTex[0] = currentTextureOffset.first;
					pTex[1] = currentTextureOffset.second + (1.0f / pS->mRows);
					pTex[2] = currentTextureOffset.first + (1.0f / pS->mColumns);
					pTex[3] = currentTextureOffset.second + (1.0f / pS->mRows);
					pTex[4] = currentTextureOffset.first + (1.0f / pS->mColumns);
					pTex[5] = currentTextureOffset.second;
					pTex[6] = currentTextureOffset.first;
					pTex[7] = currentTextureOffset.second;

					
					glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[2]);
					glBufferData(GL_ARRAY_BUFFER, vertexNum* texStride, pTex, GL_STATIC_DRAW);
					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, coordsPerTex, GL_FLOAT, false, 0, 0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					

					glDrawArrays(GL_QUADS, 0, vertexNum);

					pTex[0] = 0.0f;			pTex[1] = 1.0f;
					pTex[2] = 1.0f;			pTex[3] = 1.0f;
					pTex[4] = 1.0f;			pTex[5] = 0.0f;
					pTex[6] = 0.0f;			pTex[7] = 0.0f;
					glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[2]);
					glBufferData(GL_ARRAY_BUFFER, vertexNum * texStride, pTex, GL_STATIC_DRAW);
					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, coordsPerTex, GL_FLOAT, false, 0, 0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				}
				else
				{
					
					glDrawArrays(GL_QUADS, 0, vertexNum);
				}
			}


			// Health bars
			glUseProgram(gRectID);

			for (auto pGameObject : gpGameObjectManager->mGameObjects) {
				Transform* pT = static_cast<Transform*>(pGameObject->GetComponent(TYPE_TRANSFORM));
				Character* pC = static_cast<Character*>(pGameObject->GetComponent(TYPE_CHARACTER));

				if (!pC || gGameType == 2 || pC->mHasHealthBar == false || pC->mHP == pC->mHPMax) { continue; }

				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(pT->mPositionX, pT->mPositionY - pT->mHeight + pT->mSpriteOffsetY - 10.0f, 1.0f));
				model = glm::rotate(model, 0.0f, glm::vec3(0, 0, 1));
				model = glm::scale(model, glm::vec3(64.0f, 5.0f, 0.0f));

				model = projectionMatrix * model;

				int val = glGetUniformLocation(gRectID, "uColor1");
				glUniform4f(val, 0.3, 0.8, 0.3, 1.0);

				val = glGetUniformLocation(gRectID, "uColor2");
				glUniform4f(val, 0.4, 0.4, 0.4, 1.0);

				val = glGetUniformLocation(gRectID, "uFill");
				glUniform1f(val, pC->mHP / pC->mHPMax);

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}
		}

		if (SDL_GetWindowFlags(pWindow) & SDL_WINDOW_MINIMIZED )
			appIsPaused = true;
		


		////// Pause Screen
		if (appIsPaused && !optionsMenuOpen && !controlsMenuOpen) {
			glClear(GL_DEPTH_BUFFER_BIT);

			glUseProgram(gRenderID);
			gpAudioManager->SetMasterBusVolume(0.4f * currentVolume);
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(600.f, 400.0f, 0.f));
				model = glm::scale(model, glm::vec3(1200.0f, -800.0f, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, pauseScreen);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}

			// Continue Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(150.0, 300, 1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 275 && gpInputManager->mMouseY < 325 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen)
					glBindTexture(GL_TEXTURE_2D, continueButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, continueButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}

			if (gpInputManager->IsMouseTriggered()) {
				if (gpInputManager->mMouseY > 275 && gpInputManager->mMouseY < 325 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen) {	// continue button
					appIsPaused = false;	// remember when setting buttons that translation sets the center and scale expands it in both directions
					gpAudioManager->PlayOneShot("Click");
				}
			}



			// Options Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(150, 400.f, 1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 375 && gpInputManager->mMouseY < 425 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen)
					glBindTexture(GL_TEXTURE_2D, optionsButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, optionsButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}

			if (gpInputManager->IsMouseTriggered()) {
				if (gpInputManager->mMouseY > 375 && gpInputManager->mMouseY < 425 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen) {
					optionsMenuOpen = true;
					gpAudioManager->PlayOneShot("Click");
				}
			}
			// End Options Button

			// Controls Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(75, 50.f, 1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 25 && gpInputManager->mMouseY < 75 && gpInputManager->mMouseX > -25 && gpInputManager->mMouseX < 175 && !confirmationWindowOpen)
					glBindTexture(GL_TEXTURE_2D, controlsButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, controlsButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}

			if (gpInputManager->IsMouseTriggered()) {
				if (gpInputManager->mMouseY > 25 && gpInputManager->mMouseY < 75 && gpInputManager->mMouseX > -25 && gpInputManager->mMouseX < 175 && !confirmationWindowOpen) {
					controlsMenuOpen = true;
					gpAudioManager->PlayOneShot("Click");
				}
			}
			// End Controls Button



			// Credits Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(150, 500.f, 1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 475 && gpInputManager->mMouseY < 525 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen)
					glBindTexture(GL_TEXTURE_2D, creditsButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, creditsButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}


			if (gpInputManager->IsMouseTriggered() || creditsPending == true) {
				if (gpInputManager->mMouseY > 475 && gpInputManager->mMouseY < 525 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen) {
					gpAudioManager->PlayOneShot("Click");
					creditsPending = true;
					confirmationWindowOpen = true;
					
				}
				if(!confirmationWindowOpen)
				{
					if (confirmationWindowOutput)
					{
						gpStealthModerator->mStage = 100;
						gpStealthModerator->mTransitionTimer = 2000;
						gpGameObjectManager->~GameObjectManager();
						gpStealthModerator->mManualRestart = false;
						gpStealthModerator->mManualOverride = false;
						gpStealthModerator->mManualBack = false;
						gpObjectFactory->LoadLevel("..\\Resources\\Credits0.json");
						appIsPaused = false;
						confirmationWindowOutput = false;
					}
					creditsPending = false;
				}
			}
			// End Credits Button

			// Main Menu Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(150, 600.f, 1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 575 && gpInputManager->mMouseY < 625 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen)
					glBindTexture(GL_TEXTURE_2D, mainMenuButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, mainMenuButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}

			if (gpInputManager->IsMouseTriggered() || mainMenuPending == true) {
				if (gpInputManager->mMouseY > 575 && gpInputManager->mMouseY < 625 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen) {
					gpAudioManager->PlayOneShot("Click");
					mainMenuPending = true;
					confirmationWindowOpen = true;
				}
				if (!confirmationWindowOpen)
				{
					if (confirmationWindowOutput)
					{
						gpStealthModerator->mStage = 0;
						gpStealthModerator->mTransitionTimer = 2000;
						gpGameObjectManager->~GameObjectManager();
						gpStealthModerator->mManualRestart = false;
						gpStealthModerator->mManualOverride = false;
						gpStealthModerator->mManualBack = false;
						gpObjectFactory->LoadLevel("..\\Resources\\Title.json");
						appIsPaused = false;
						confirmationWindowOutput = false;
					}
					mainMenuPending = false;

				}
			}
			// Main Menu Button


			// Quit Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(150, 750.f, 1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 725 && gpInputManager->mMouseY < 775 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen)
					glBindTexture(GL_TEXTURE_2D, quitButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, quitButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}


			if (gpInputManager->IsMouseTriggered() || exitPending) {
				if (gpInputManager->mMouseY > 725 && gpInputManager->mMouseY < 775 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen) {
					gpAudioManager->PlayOneShot("Click");
					exitPending = true;
					confirmationWindowOpen = true;
					
				}
				if (!confirmationWindowOpen)
				{
					if (confirmationWindowOutput)
					{
						appIsRunning = false;	// remember when setting buttons that translation sets the center and scale expands it in both directions
						confirmationWindowOutput = false;
					}
					exitPending = false;
				}
			}
			// End Quit Button


			/*
			// Controls Menu
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(1018, 500.f, 1.f));
				model = glm::scale(model, glm::vec3(256, -256.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, controlsText);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}
			// End Controls Menu
			*/
		}

		//// Main Menu
		if (gpStealthModerator->mStage == 0 && !optionsMenuOpen && !controlsMenuOpen)
		{
			glClear(GL_DEPTH_BUFFER_BIT);

			glUseProgram(gRenderID);

			// Start Game Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(150.0, 300, 1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 275 && gpInputManager->mMouseY < 325 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen)
					glBindTexture(GL_TEXTURE_2D, startGameButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, startGameButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}

			if (gpInputManager->IsMouseTriggered()) {
				if (gpInputManager->mMouseY > 275 && gpInputManager->mMouseY < 325 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen) {
					gpStealthModerator->mManualOverride = true;	// remember when setting buttons that translation sets the center and scale expands it in both directions
					gpAudioManager->PlayOneShot("Click");
				}
			}

			// End Start Game Button


			// Options Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(150, 400.f, 1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 375 && gpInputManager->mMouseY < 425 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen)
					glBindTexture(GL_TEXTURE_2D, optionsButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, optionsButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}

			if (gpInputManager->IsMouseTriggered()) {
				if (gpInputManager->mMouseY > 375 && gpInputManager->mMouseY < 425 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen) {
					optionsMenuOpen = true;
					gpAudioManager->PlayOneShot("Click");
				}
			}
			// End Options Button

			// Controls Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(75, 50.f, 1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 25 && gpInputManager->mMouseY < 75 && gpInputManager->mMouseX > -25 && gpInputManager->mMouseX < 175 && !confirmationWindowOpen)
					glBindTexture(GL_TEXTURE_2D, controlsButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, controlsButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}

			if (gpInputManager->IsMouseTriggered()) {
				if (gpInputManager->mMouseY > 25 && gpInputManager->mMouseY < 75 && gpInputManager->mMouseX > -25 && gpInputManager->mMouseX < 175 && !confirmationWindowOpen) {
					controlsMenuOpen = true;
					gpAudioManager->PlayOneShot("Click");
				}
			}
			// End Controls Button


			// Credits Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(150, 500.f, 1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 475 && gpInputManager->mMouseY < 525 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen)
					glBindTexture(GL_TEXTURE_2D, creditsButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, creditsButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}


			if (gpInputManager->IsMouseTriggered()) {
				if (gpInputManager->mMouseY > 475 && gpInputManager->mMouseY < 525 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen) {
					gpStealthModerator->mStage = 100;
					gpStealthModerator->mTransitionTimer = 2000;
					gpGameObjectManager->~GameObjectManager();
					gpStealthModerator->mManualRestart = false;
					gpStealthModerator->mManualOverride = false;
					gpStealthModerator->mManualBack = false;
					gpObjectFactory->LoadLevel("..\\Resources\\Credits0.json");
					gpAudioManager->PlayOneShot("Click");
				}
			}
			// End Credits Button


			// Quit Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(150, 750.f, 1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 725 && gpInputManager->mMouseY < 775 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen)
					glBindTexture(GL_TEXTURE_2D, quitButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, quitButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}


			if (gpInputManager->IsMouseTriggered() || exitPending) {
				if (gpInputManager->mMouseY > 725 && gpInputManager->mMouseY < 775 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen) {
					gpAudioManager->PlayOneShot("Click");
					exitPending = true;
					confirmationWindowOpen = true;
				}
				if (!confirmationWindowOpen)
				{
					if (confirmationWindowOutput)
					{
						appIsRunning = false;	// remember when setting buttons that translation sets the center and scale expands it in both directions
						confirmationWindowOutput = false;
					}
					exitPending = false;
				}
			}
			// End Quit Button


			/*
			// Controls Menu
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(1018, 500.f, 1.f));
				model = glm::scale(model, glm::vec3(256, -256.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, controlsText);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}
			// End Controls Menu
			*/

		}



		// Options Menu (Within Main Menu or Pause Menu)
		if (optionsMenuOpen) {
			glClear(GL_DEPTH_BUFFER_BIT);

			glUseProgram(gRenderID);

			if (appIsPaused)
			{
				// Update Menu Audio 
				gpAudioManager->SetMasterBusVolume(0.4f * currentVolume);

				//redraw background for pause menu
				{
					glm::mat4 model(1.0f);
					model = glm::translate(model, glm::vec3(600.f, 400.0f, 0.f));
					model = glm::scale(model, glm::vec3(1200.0f, -800.0f, 0.0f));

					model = projectionMatrix * model;

					int transformationHandle = 4;
					glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, pauseScreen);

					glDrawArrays(GL_QUADS, 0, vertexNum);
				}
			}

			// Back to Menu Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(150, 500.f, 1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 475 && gpInputManager->mMouseY < 525 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen)
					glBindTexture(GL_TEXTURE_2D, backButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, backButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}

			if (gpInputManager->IsMouseTriggered()) {
				if (gpInputManager->mMouseY > 475 && gpInputManager->mMouseY < 525 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen) {
					optionsMenuOpen = false;
					gpAudioManager->PlayOneShot("Click");
				}
			}
			// End Back to Menu Button

			// Sound Off Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(1000, 475.f, 1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 450 && gpInputManager->mMouseY < 500 && gpInputManager->mMouseX > 900 && gpInputManager->mMouseX < 1100 && !confirmationWindowOpen)
					glBindTexture(GL_TEXTURE_2D, soundOffButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, soundOffButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}

			if (gpInputManager->IsMouseTriggered()) {
				if (gpInputManager->mMouseY > 450 && gpInputManager->mMouseY < 500 && gpInputManager->mMouseX > 900 && gpInputManager->mMouseX < 1100 && !confirmationWindowOpen)
					currentVolume = 0.0f;
			}
			// End Sound Off Button

			// Sound On Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(1000, 550.f, 1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 525 && gpInputManager->mMouseY < 575 && gpInputManager->mMouseX > 900 && gpInputManager->mMouseX < 1100 && !confirmationWindowOpen)
					glBindTexture(GL_TEXTURE_2D, soundOnButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, soundOnButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}

			if (gpInputManager->IsMouseTriggered()) {
				if (gpInputManager->mMouseY > 525 && gpInputManager->mMouseY < 575 && gpInputManager->mMouseX > 900 && gpInputManager->mMouseX < 1100 && !confirmationWindowOpen)
				{
					currentVolume = 1.0f; 
					gpAudioManager->PlayOneShot("Click");
				}
			}
			// End Sound On Button

			// Fullscreen Toggle Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(1000, 625.f, 1.f));
				model = glm::scale(model, glm::vec3(256, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 600 && gpInputManager->mMouseY < 650 && gpInputManager->mMouseX > 875 && gpInputManager->mMouseX < 1125 && !confirmationWindowOpen)
					glBindTexture(GL_TEXTURE_2D, fullScreenToggleButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, fullScreenToggleButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}

			if (appIsPaused)
			{
				if (gpInputManager->IsMouseTriggered() || fullScreenTogglePending) {
					if (gpInputManager->mMouseY > 600 && gpInputManager->mMouseY < 650 && gpInputManager->mMouseX > 875 && gpInputManager->mMouseX < 1125 && !confirmationWindowOpen) {
						gpAudioManager->PlayOneShot("Click");
						fullScreenTogglePending = true;
						confirmationWindowOpen = true;
					}
					if (!confirmationWindowOpen)
					{
						if (confirmationWindowOutput)
						{
							fullscreenToggle = true;
							confirmationWindowOutput = false;
						}
						fullScreenTogglePending = false;
					}
				}
			}
			else
			{
				if (gpInputManager->IsMouseTriggered()) {
					if (gpInputManager->mMouseY > 600 && gpInputManager->mMouseY < 650 && gpInputManager->mMouseX > 875 && gpInputManager->mMouseX < 1125 && !confirmationWindowOpen)
					{
						fullscreenToggle = true;
						gpAudioManager->PlayOneShot("Click");
					}
				}
			}

			
			// End Fullscreen Toggle Button


			// Options Text
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(1018, 500.f, 1.f));
				model = glm::scale(model, glm::vec3(256, -256.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, optionsText);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}
			// End Options Text

		}


		if (controlsMenuOpen) {
			glClear(GL_DEPTH_BUFFER_BIT);

			glUseProgram(gRenderID);

			if (appIsPaused)
			{
				// Update Menu Audio 
				gpAudioManager->SetMasterBusVolume(0.4f * currentVolume);

				//redraw background for pause menu
				{
					glm::mat4 model(1.0f);
					model = glm::translate(model, glm::vec3(600.f, 400.0f, 0.f));
					model = glm::scale(model, glm::vec3(1200.0f, -800.0f, 0.0f));

					model = projectionMatrix * model;

					int transformationHandle = 4;
					glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, pauseScreen);

					glDrawArrays(GL_QUADS, 0, vertexNum);
				}
			}

			// Back to Menu Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(150, 500.f, 1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 475 && gpInputManager->mMouseY < 525 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen)
					glBindTexture(GL_TEXTURE_2D, backButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, backButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}

			if (gpInputManager->IsMouseTriggered()) {
				if (gpInputManager->mMouseY > 475 && gpInputManager->mMouseY < 525 && gpInputManager->mMouseX > 50 && gpInputManager->mMouseX < 250 && !confirmationWindowOpen) {
					controlsMenuOpen = false;
					gpAudioManager->PlayOneShot("Click");
				}
			}
			// End Back to Menu Button




			// Controls Menu
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(1018, 500.f, 1.f));
				model = glm::scale(model, glm::vec3(256, -256.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, controlsText);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}
			// End Controls Menu

		}


		// Confirmation Window
		if (confirmationWindowOpen) {
			glClear(GL_DEPTH_BUFFER_BIT);

			glUseProgram(gRenderID);

			// Update Menu Audio 
			gpAudioManager->SetMasterBusVolume(0.4f * currentVolume);

			//redraw background for pause menu
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(616, 208, 0.f));
				model = glm::scale(model, glm::vec3(576.0f, -320.0f, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if(fullScreenTogglePending)
					glBindTexture(GL_TEXTURE_2D, confirmationRestartScreen);
				else
					glBindTexture(GL_TEXTURE_2D, confirmationScreen);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}
			

			// Yes Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(716, 208, 1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 183 && gpInputManager->mMouseY < 233 && gpInputManager->mMouseX > 616 && gpInputManager->mMouseX < 816)
					glBindTexture(GL_TEXTURE_2D, yesButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, yesButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}

			if (gpInputManager->IsMouseTriggered()) {
				if (gpInputManager->mMouseY > 183 && gpInputManager->mMouseY < 233 && gpInputManager->mMouseX > 616 && gpInputManager->mMouseX < 816) {
					confirmationWindowOpen = false;
					confirmationWindowOutput = true;
					gpAudioManager->PlayOneShot("Click");
				}
			}
			// End Yes Button

			// No Button
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(515, 208, 1.f));
				model = glm::scale(model, glm::vec3(192, -64.0, 0.0f));

				model = projectionMatrix * model;

				int transformationHandle = 4;
				glUniformMatrix4fv(transformationHandle, 1, false, &model[0][0]);

				glActiveTexture(GL_TEXTURE0);
				if (gpInputManager->mMouseY > 183 && gpInputManager->mMouseY < 233 && gpInputManager->mMouseX > 416 && gpInputManager->mMouseX < 616)
					glBindTexture(GL_TEXTURE_2D, noButtonHighlighted);
				else
					glBindTexture(GL_TEXTURE_2D, noButton);

				glDrawArrays(GL_QUADS, 0, vertexNum);
			}

			if (gpInputManager->IsMouseTriggered()) {
				if (gpInputManager->mMouseY > 183 && gpInputManager->mMouseY < 233 && gpInputManager->mMouseX > 416 && gpInputManager->mMouseX < 616) {
					confirmationWindowOpen = false;
					confirmationWindowOutput = false;
					gpAudioManager->PlayOneShot("Click");
				}
			}
			// End No Button


		}

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);

		glBindVertexArray(0);
		
		/*******************************************************************/
		/*******************************************************************/
		/*******************************************************************/


		

		// SDL_UpdateWindowSurface(pWindow);
		SDL_GL_SwapWindow(pWindow);
		
		
		if (gpInputManager->IsKeyTriggered(SDL_SCANCODE_ESCAPE) && gpStealthModerator->mStage > 0 && gpStealthModerator->mStage < 99 && !optionsMenuOpen && !controlsMenuOpen && !confirmationWindowOpen) {	// pause game
			// appIsRunning = false;
			appIsPaused = !appIsPaused;
		}
		if (gpInputManager->IsKeyTriggered(SDL_SCANCODE_ESCAPE) && gpStealthModerator->mStage == 99)
		{
			appIsRunning = false;
		}

		if (gpInputManager->IsKeyTriggered(SDL_SCANCODE_Q)) {	// end game
			appIsRunning = false;
		}

		if (gpInputManager->IsKeyTriggered(SDL_SCANCODE_RETURN) && !appIsPaused) {	// restart level

			if(gGameType == 3)
				gpStealthModerator->mManualRestart = true;

			appIsPaused = false;
			/*
			gpGameObjectManager->~GameObjectManager();
			gGameType = 1;

			gpModerator->mStage = 1;
			gpObjectFactory->LoadLevel("..\\Resources\\Level1.json");
			*/

		}

		if (gpInputManager->IsKeyTriggered(SDL_SCANCODE_F1)) {	// GOD-Mode
			for (auto pGameObject : gpGameObjectManager->mGameObjects) {
				Character* pChar = static_cast<Character*>(pGameObject->GetComponent(TYPE_CHARACTER));
				Controller* pCont = static_cast<Controller*>(pGameObject->GetComponent(TYPE_PLAYER_CONTROLLER));
				if (pCont != nullptr && pChar != nullptr)
				{
					pChar->mGodMode = !pChar->mGodMode;
					pCont->mGodMode = !pCont->mGodMode;

				}
			}

		}


		if (gpInputManager->IsKeyTriggered(SDL_SCANCODE_F2)) {	// Skip to level 9

			gpGameObjectManager->~GameObjectManager();
			gpObjectFactory->LoadLevel("..\\Resources\\StealthLevel9.json");
			gpStealthModerator->mTransitionTimer = gpStealthModerator->mTransitionTimerLimit;
			gpStealthModerator->mManualOverride = false;
			gpStealthModerator->mManualBack = false;
			gpStealthModerator->mManualRestart = false;
			gpStealthModerator->mStage = 9;
		}
		


		if (gpInputManager->IsKeyTriggered(SDL_SCANCODE_F11) || init || fullscreenToggle) {	// Full screen toggle
			fullscreenToggle = false;
			appIsFullscreen = !appIsFullscreen;
			if (appIsFullscreen) {
				int width, height;
				GetDesktopResolution(width, height);
				screenSize[0] = width;
				screenSize[1] = height;
			}
			else {
				screenSize[0] = 1200;
				screenSize[1] = 800;
			}

			// Uncomment if you want the scaled-up window to use actual-size graphics, but for our purposes I believe that stretching is allowable and preferable to redoing teh camera to handle variable-size windows
			// projectionMatrix = glm::ortho(0.f, (float)screenSize[0], (float)screenSize[1], 0.f);

			glDisable(GL_DEPTH_TEST);
			SDL_DestroyWindow(pWindow);

			if (appIsFullscreen) {
				pWindow = SDL_CreateWindow("ConCaveity",		// window title
					SDL_WINDOWPOS_UNDEFINED,					// initial x position
					SDL_WINDOWPOS_UNDEFINED,					// initial y position
					screenSize[0],										// width, in pixels
					screenSize[1],										// height, in pixels
					SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);
			}
			else {
				pWindow = SDL_CreateWindow("ConCaveity",		// window title
					SDL_WINDOWPOS_UNDEFINED,					// initial x position
					SDL_WINDOWPOS_UNDEFINED,					// initial y position
					screenSize[0],										// width, in pixels
					screenSize[1],										// height, in pixels
					SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
			}

			openGL_Context = SDL_GL_CreateContext(pWindow);

			glGenVertexArrays(1, &vaoID);
			glBindVertexArray(vaoID);

			GLfloat* pPositions = new GLfloat[vertexNum * coordsPerPosition];
			GLfloat* pColors = new GLfloat[vertexNum * coordsPerColor];
			GLfloat* pTex = new GLfloat[vertexNum * coordsPerTex];

			// Vertex 1
			pPositions[0] = -0.5f;	pPositions[1] = -0.5f;	pPositions[2] = 0.0f;
			pColors[0] = 1.0f;		pColors[1] = .0f;		pColors[2] = .0f;		pColors[3] = 1.0f;
			pTex[0] = 0.0f;			pTex[1] = 1.0f;

			// Vertex 2
			pPositions[3] = 0.5f;	pPositions[4] = -0.5f;	pPositions[5] = 0.0f;
			pColors[4] = .0f;		pColors[5] = 1.0f;		pColors[6] = .0f;		pColors[7] = 1.0f;
			pTex[2] = 1.0f;			pTex[3] = 1.0f;

			// Vertex 3
			pPositions[6] = 0.5f;	pPositions[7] = 0.5f;	pPositions[8] = 0.0f;
			pColors[8] = .0f;		pColors[9] = .0f;		pColors[10] = 1.0f;		pColors[11] = 1.0f;
			pTex[4] = 1.0f;			pTex[5] = 0.0f;


			// Vertex 4
			pPositions[9] = -0.5f;	pPositions[10] = 0.5f;	pPositions[11] = 0.0f;
			pColors[12] = 1.0f;		pColors[13] = 1.0f;		pColors[14] = 1.0f;		pColors[15] = 1.0f;
			pTex[6] = 0.0f;			pTex[7] = 0.0f;


			// Copy from RAM to VRAM
			//Positions
			glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[0]);
			glBufferData(GL_ARRAY_BUFFER, vertexNum* positionStride, pPositions, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, coordsPerPosition, GL_FLOAT, false, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// Colors
			glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[1]);
			glBufferData(GL_ARRAY_BUFFER, vertexNum* colorStride, pColors, GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, coordsPerColor, GL_FLOAT, false, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// Textures
			glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[2]);
			glBufferData(GL_ARRAY_BUFFER, vertexNum* texStride, pTex, GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, coordsPerTex, GL_FLOAT, false, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			
			glBindVertexArray(0);

			delete[] pPositions;
			delete[] pColors;
			delete[] pTex;

			LoadShaders();
			gpResourceManager->ReInit();
			gpStealthModerator->mManualRestart = true;

			for (auto pGameObject : gpGameObjectManager->mGameObjects) {
				Sprite* pSprite = static_cast<Sprite*>(pGameObject->GetComponent(TYPE_SPRITE));
				if (pSprite == nullptr) {
					continue;
				}
				
				pSprite->mTexture = gpResourceManager->LoadTexture(pSprite->mSpritePath.c_str());
			}

			endScreen = gpResourceManager->LoadTexture("../Resources/Concaveity_Victory.png");
			deadScreen = gpResourceManager->LoadTexture("../Resources/Concaveity_Retry.png");
			backgroundImg0 = gpResourceManager->LoadTexture("../Resources/Bricks_Background_Torches_0.png");
			backgroundImg1 = gpResourceManager->LoadTexture("../Resources/Bricks_Background_Torches_1.png");
			backgroundCredits = gpResourceManager->LoadTexture("../Resources/Credits_Background.png");

			pauseScreen = gpResourceManager->LoadTexture("../Resources/Concaveity_Pause.png");
			continueButton = gpResourceManager->LoadTexture("../Resources/Continue_Button.png");
			continueButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Continue_Button_Highlighted.png");
			controlsText = gpResourceManager->LoadTexture("../Resources/Controls_Text.png");
			creditsButton = gpResourceManager->LoadTexture("../Resources/Credits_Button.png");
			creditsButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Credits_Button_Highlighted.png");
			mainMenuButton = gpResourceManager->LoadTexture("../Resources/Main_Menu_Button.png");
			mainMenuButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Main_Menu_Button_Highlighted.png");
			optionsButton = gpResourceManager->LoadTexture("../Resources/Options_Button.png");
			optionsButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Options_Button_Highlighted.png");
			controlsButton = gpResourceManager->LoadTexture("../Resources/Controls_Button.png");
			controlsButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Controls_Button_Highlighted.png");
			quitButton = gpResourceManager->LoadTexture("../Resources/Quit_Button.png");
			quitButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Quit_Button_Highlighted.png");
			startGameButton = gpResourceManager->LoadTexture("../Resources/Start_Game_Button.png");
			startGameButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Start_Game_Button_Highlighted.png");

			optionsText = gpResourceManager->LoadTexture("../Resources/Options_Text.png");
			fullScreenToggleButton = gpResourceManager->LoadTexture("../Resources/Full_Screen_Toggle_Button.png");
			fullScreenToggleButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Full_Screen_Toggle_Button_Highlighted.png");
			soundOnButton = gpResourceManager->LoadTexture("../Resources/Sound_On_Button.png");
			soundOnButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Sound_On_Button_Highlighted.png");
			soundOffButton = gpResourceManager->LoadTexture("../Resources/Sound_Off_Button.png");
			soundOffButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Sound_Off_Button_Highlighted.png");
			backButton = gpResourceManager->LoadTexture("../Resources/Back_Button.png");
			backButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Back_Button_Highlighted.png");

			confirmationScreen = gpResourceManager->LoadTexture("../Resources/Confirmation_Text.png");
			confirmationRestartScreen = gpResourceManager->LoadTexture("../Resources/Confirmation_Text_Restart.png");
			yesButton = gpResourceManager->LoadTexture("../Resources/Yes_Button.png");
			yesButtonHighlighted = gpResourceManager->LoadTexture("../Resources/Yes_Button_Green.png");
			noButton = gpResourceManager->LoadTexture("../Resources/No_Button.png");
			noButtonHighlighted = gpResourceManager->LoadTexture("../Resources/No_Button_Highlighted.png");


			glEnable(GL_DEPTH_TEST);
		}

		

		if ((gpInputManager->IsKeyTriggered(SDL_SCANCODE_P) || 
			((gpInputManager->IsMouseTriggered() || SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT) || gpInputManager->IsKeyTriggered(SDL_SCANCODE_ESCAPE) 
				|| gpInputManager->IsKeyTriggered(SDL_SCANCODE_RETURN) || gpInputManager->IsKeyTriggered(SDL_SCANCODE_SPACE)) 
				&& (gpStealthModerator->mStage < 0 || gpStealthModerator->mStage > 99))) 
			&& !appIsPaused && !optionsMenuOpen && !controlsMenuOpen && !confirmationWindowOpen) {	// next level
			/*
			gpGameObjectManager->~GameObjectManager();
			gpEventManager->Reset();
			gpCollisionManager->Reset();

			(levelNo == 4 ? levelNo = 0 : levelNo++);
			gpObjectFactory->LoadLevel(("..\\Resources\\Level" + std::to_string(levelNo) + ".json").c_str());
			*/
			/*
			if (gGameType == 1)
			{

				gpModerator->mStage++;
			
				if (gpModerator->mStage == 0) {
					gpGameObjectManager->~GameObjectManager();
				}
				else if(gpModerator->mStage <= 4) {
					// gpGameObjectManager->~GameObjectManager();
					gpObjectFactory->LoadLevel(("..\\Resources\\Level" + std::to_string(gpModerator->mStage) + ".json").c_str());
				}
			}
			*/
			if (gGameType == 3)
			{
				gpStealthModerator->mManualOverride = true;
			}
			
		}
		
		
		if (gpInputManager->IsKeyTriggered(SDL_SCANCODE_O) && !appIsPaused) {	// prev level
			if (gGameType == 3)
			{
				gpStealthModerator->mManualBack = true;
			}
			//gpGameObjectManager->~GameObjectManager();
			//gpModerator->mStage = 1;
			//gpObjectFactory->LoadLevel("..\\Resources\\Level1.json");
		}

		if (gpInputManager->IsKeyTriggered(SDL_SCANCODE_F)) {
			DEBUG = !DEBUG;
		}
		/*
		if (gpInputManager->IsKeyTriggered(SDL_SCANCODE_2)) {
			gGameType = 2;
			gpGameObjectManager->~GameObjectManager();
			gpObjectFactory->LoadGameObject("../Resources/Runner.json");
		}

		//Start Playing Stealth Prototype
		if (gpInputManager->IsKeyTriggered(SDL_SCANCODE_3)) {
			gGameType = 3;
			gpGameObjectManager->~GameObjectManager();
			gpObjectFactory->LoadLevel("../Resources/StealthLevel1.json");
			gpStealthModerator->mStage = 1;
		}
		*/

		for (auto pGO : gpGameObjectManager->mGameObjects) {
			Character* pC = static_cast<Character*>(pGO->GetComponent(TYPE_CHARACTER));
			Controller* pCont = static_cast<Controller*>(pGO->GetComponent(TYPE_PLAYER_CONTROLLER));
			if (!pC) { continue; }
			if (pC->mHP <= 0) {
				// gpObjectFactory->LoadGameObject("../Resources/Slime.json");
				if (pCont != nullptr && pC->mGodMode)
				{
					pC->mHP = pC->mHPMax;
					continue;
				}
				AudioClip* pAC = static_cast<AudioClip*>(pC->mpOwner->GetComponent(TYPE_AUDIOCLIP));
				pAC->PlayOneShot("Die");
				gpGameObjectManager->DeleteObject(pC->mpOwner);
			}
		}


		// Lock the frame rate
		gpFRC->FrameEnd(appIsPaused);

		if (init)
			init = false;

		
	}

	delete gpObjectFactory;
	delete gpGameObjectManager;
	delete gpFRC;
	delete gpInputManager;
	delete gpCollisionManager;
	delete gpEventManager;

	// Destroy the image
	// SDL_FreeSurface(pImageSurface);
	glDeleteVertexArrays(1, &vaoID);
	glDeleteBuffers(3, bufferIDs);
	glUseProgram(0);
	glDeleteProgram(gRenderID);
	SDL_GL_DeleteContext(openGL_Context);

	gpResourceManager->~ResourceManager();
	

	// Close and destroy the window
	SDL_DestroyWindow(pWindow);

	// Quit SDL subsystems
	SDL_Quit();

	/*
	System:Start
	
	Load 
	Init
	Update
	Draw
	Free

	System:End
	*/
	
	return 0;
}