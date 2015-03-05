#include "Game.h"
#include "Errors.h"


Game::Game() :
	window(nullptr),
	m_windowWidth(800),
	m_windowHeight(600),
	m_gameState(m_gameState::Play),
	m_time(0.0f)
{
}


Game::~Game()
{
}

void Game::Run()
{
	InitSystems();
	m_sprite.InitSprite(-0.75f, -0.75f, 1.5f, 1.5f);
	GameLoop();
}
//initialize SDL, Glew, and OpenGL
void Game::InitSystems()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	//creates an SDL window with an OpenGL context
	window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight, SDL_WINDOW_OPENGL);
	//error check to make sure there is a window created
	if (window == nullptr)
	{
		FatalError("SDL window could not be created");
	}
	//add the OpenGL context to the SDL window
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	//if the context fails this prints out an error
	if (glContext == nullptr)
	{
		FatalError("Could not create OpenGL context");
	}
	//initialize and error check Glew
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		FatalError("Could not initialize Glew");
	}
	//ensures 2 buffers to be swapped while rendering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//Color of the background for the window
	glClearColor(1.0, 0.0, 1.0, 1.0);

	InitShaders();

}

void Game::InitShaders()
{
	m_shaderProgram.CompileShaders("Shaders/basicVertShader.vert", "Shaders/basicFragShader.frag");
	m_shaderProgram.AddAttribute("vertexPosition");
	m_shaderProgram.AddAttribute("vertexColor");
	m_shaderProgram.LinkShaders();
}

//Main game loop, runs til m_gameState is set to exit
void Game::GameLoop()
{
	while (m_gameState != m_gameState::Exit)
	{
		ProcessInput();
		m_time += 0.05f;
		RenderGame();
	}
}
//Process all input for the game from keyboard, mouse, etc.
void Game::ProcessInput()
{
	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: m_gameState = m_gameState::Exit;
			break;
		case SDL_MOUSEMOTION:
		{
			//displays mouse x and y pos on the console while the mouse is within the boundaries of the window 
			std::cerr << "Mouse X = " << event.motion.x << " Mouse Y = " << event.motion.y << std::endl;
			//displays how far the mouse has traveled since the last update
			std::cerr << "Mouse xRel = " << event.motion.xrel << " Mouse yRel = " << event.motion.yrel << std::endl;
			break;
		}
		default:
			break;
		}
	}
}
//The data to be rendered to the screen
void Game::RenderGame()
{
	//OpenGL clears to this depth
	glClearDepth(1.0);
	//clears both color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_shaderProgram.UseProgram();

	GLuint timeLocation = m_shaderProgram.GetUniformLocation("time");
	glUniform1f(timeLocation, m_time);

	m_sprite.RenderSprite();
	
	m_shaderProgram.StopProgram();

	//swaps buffers on window
	SDL_GL_SwapWindow(window);
}
