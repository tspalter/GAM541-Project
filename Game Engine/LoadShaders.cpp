/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: LoadShaders.cpp
Purpose:  Loads Shaders
Language: C++
Platform : Microsoft Visual Studio for Windows 10
Project: GAM541 Final Project
Authors: Dylan Washburne
- End Header --------------------------------------------------------*/

#include "LoadShaders.h"

extern GLint gRenderID, gCircID, gRectID;

void LoadShaders() {
	gRenderID = glCreateProgram();

	// Vertex Shader
	{

		std::ifstream openFile("Shaders/Transform.vert");
		std::string vertexFile((std::istreambuf_iterator<char>(openFile)), std::istreambuf_iterator<char>());
		openFile.close();

		const char* vertexShaderCode = vertexFile.c_str();


		// const char* vertexShaderCode = "";

		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLint status;

		glShaderSource(vertexShaderID, 1, &vertexShaderCode, NULL);
		glCompileShader(vertexShaderID);
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &status);
		if (status == GL_TRUE) {
			glAttachShader(gRenderID, vertexShaderID);
		}
	}

	// Fragment Shader
	{

		std::ifstream openFile("Shaders/Transform.frag");
		std::string fragFile((std::istreambuf_iterator<char>(openFile)), std::istreambuf_iterator<char>());
		openFile.close();

		const char* fragmentShaderCode = fragFile.c_str();


		// const char* fragmentShaderCode = "";

		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		GLint status;

		glShaderSource(fragmentShaderID, 1, &fragmentShaderCode, NULL);
		glCompileShader(fragmentShaderID);
		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &status);
		if (status == GL_TRUE) {
			glAttachShader(gRenderID, fragmentShaderID);
		}
	}

	// Link the program
	glLinkProgram(gRenderID);
	glUseProgram(gRenderID);



	gCircID = glCreateProgram();
	// Vertex Shader
	{

		std::ifstream openFile("Shaders/Circ.vert");
		std::string vertexFile((std::istreambuf_iterator<char>(openFile)), std::istreambuf_iterator<char>());
		openFile.close();

		const char* vertexShaderCode = vertexFile.c_str();


		// const char* vertexShaderCode = "";

		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLint status;

		glShaderSource(vertexShaderID, 1, &vertexShaderCode, NULL);
		glCompileShader(vertexShaderID);
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &status);
		if (status == GL_TRUE) {
			glAttachShader(gCircID, vertexShaderID);
		}
	}

	// Fragment Shader
	{

		std::ifstream openFile("Shaders/Circ.frag");
		std::string fragFile((std::istreambuf_iterator<char>(openFile)), std::istreambuf_iterator<char>());
		openFile.close();

		const char* fragmentShaderCode = fragFile.c_str();

		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		GLint status;

		glShaderSource(fragmentShaderID, 1, &fragmentShaderCode, NULL);
		glCompileShader(fragmentShaderID);
		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &status);
		if (status == GL_TRUE) {
			glAttachShader(gCircID, fragmentShaderID);
		}
	}

	glLinkProgram(gCircID);


	gRectID = glCreateProgram();
	// Vertex Shader
	{

		std::ifstream openFile("Shaders/Rect.vert");
		std::string vertexFile((std::istreambuf_iterator<char>(openFile)), std::istreambuf_iterator<char>());
		openFile.close();

		const char* vertexShaderCode = vertexFile.c_str();

		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLint status;

		glShaderSource(vertexShaderID, 1, &vertexShaderCode, NULL);
		glCompileShader(vertexShaderID);
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &status);
		if (status == GL_TRUE) {
			glAttachShader(gRectID, vertexShaderID);
		}
	}

	// Fragment Shader
	{

		std::ifstream openFile("Shaders/Rect.frag");
		std::string fragFile((std::istreambuf_iterator<char>(openFile)), std::istreambuf_iterator<char>());
		openFile.close();

		const char* fragmentShaderCode = fragFile.c_str();

		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		GLint status;

		glShaderSource(fragmentShaderID, 1, &fragmentShaderCode, NULL);
		glCompileShader(fragmentShaderID);
		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &status);
		if (status == GL_TRUE) {
			glAttachShader(gRectID, fragmentShaderID);
		}
	}

	glLinkProgram(gRectID);
}
