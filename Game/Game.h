#pragma once
#include "include\SDL2\SDL.h"
#include "include\SDL2\glew.h"
#include <iostream>
#include <string>
#include "Sprite.h"
#include "GLSLProgram.h"

enum class m_gameState{Play, Paused, Exit};

class Game
{
public:
	Game();
	~Game();
	void Run();

private:
	void InitSystems();
	void InitShaders();
	void ProcessInput();
	void GameLoop();
	void RenderGame();
	SDL_Window* window;
	int m_windowWidth;
	int m_windowHeight;
	m_gameState m_gameState;
	Sprite m_sprite;
	GLSLProgram m_shaderProgram;
};

