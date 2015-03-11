#include "Game.h"
#include "Errors.h"

Game::Game() :
	window(nullptr),
	m_windowWidth(800),
	m_windowHeight(600),
	m_gameState(m_gameState::Play),
	m_time(0.0f),
	m_maxFPS(60.0f)
{
}

Game::~Game()
{
}

void Game::Run()
{
	InitSystems();
	m_sprites.push_back(new Sprite());
	m_sprites.back()->InitSprite(-1.0f, -1.0f, 1.0f, 1.0f, "Textures/Idle/frame-2.png");

	m_sprites.push_back(new Sprite());
	m_sprites.back()->InitSprite(0.0f, -1.0f, 1.0f, 1.0f, "Textures/Idle/frame-2.png");

	//m_texture = ImageLoader::LoadPng("Textures/Idle/frame-2.png");
	GameLoop();
}
//initialize SDL, Glew, and OpenGL
void Game::InitSystems()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	//ensures 2 buffers to be swapped while rendering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

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
	
	std::printf("  openGL version %s  \n", glGetString(GL_VERSION));

	//Color of the background for the window
	glClearColor(1.0, 0.0, 1.0, 1.0);
	// 1 turns on vsync, 0 is off
	SDL_GL_SetSwapInterval(0);

	InitShaders();

}

void Game::InitShaders()
{
	m_shaderProgram.CompileShaders("Shaders/basicVertShader.vert", "Shaders/basicFragShader.frag");
	m_shaderProgram.AddAttribute("vertexPosition");
	m_shaderProgram.AddAttribute("vertexColor");
	m_shaderProgram.AddAttribute("vertexUV");
	m_shaderProgram.LinkShaders();
}

//Main game loop, runs til m_gameState is set to exit
void Game::GameLoop()
{
	while (m_gameState != m_gameState::Exit)
	{
		float startTicks = SDL_GetTicks();

		ProcessInput();
		m_time += 0.05f;
		RenderGame();
		CalcFPS();
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10)
		{
			std::cerr << m_fps << std::endl;
			frameCounter = 0;
		}
		float frameTicks = SDL_GetTicks() - startTicks;

		if (1000.0f / m_maxFPS > frameTicks)
		{
			SDL_Delay(1000.0f / m_maxFPS - frameTicks);
		}
		
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
				//std::cerr << "Mouse X = " << event.motion.x << " Mouse Y = " << event.motion.y << std::endl;
				//displays how far the mouse has traveled since the last update
				//std::cerr << "Mouse xRel = " << event.motion.xrel << " Mouse yRel = " << event.motion.yrel << std::endl;
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
	//set the active texture to be used
	glActiveTexture(GL_TEXTURE0);


	//get the uniform from the shader
	GLint textureLocation = m_shaderProgram.GetUniformLocation("playerTexture");
	glUniform1i(textureLocation, 0);

	GLuint timeLocation = m_shaderProgram.GetUniformLocation("time");
	glUniform1f(timeLocation, m_time);

	for (unsigned int i = 0; i < m_sprites.size(); i++)
	{
		m_sprites[i]->RenderSprite();
	}
	
	
	glBindTexture(GL_TEXTURE_2D, 0);

	m_shaderProgram.StopProgram();

	//swaps buffers on window
	SDL_GL_SwapWindow(window);
}

void Game::CalcFPS()
{
	static const int NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;

	static float prevTicks = SDL_GetTicks();
	float currentTicks;

	currentTicks = SDL_GetTicks();

	m_frameTime = currentTicks - prevTicks;

	frameTimes[currentFrame % NUM_SAMPLES] = m_frameTime;

	prevTicks = currentTicks;

	int count;

	currentFrame++;

	if (currentFrame < NUM_SAMPLES)
	{
		count = currentFrame;
	}
	else
	{
		count = NUM_SAMPLES;
	}

	float frameTimeAverage = 0;
	for (int i = 0; i < count; i++)
	{
		frameTimeAverage += frameTimes[i];
	}
	frameTimeAverage /= count;

	if (frameTimeAverage > 0)
	{
		m_fps = 1000.0f / frameTimeAverage;
	}
	else
	{
		m_fps = 1.0f;
	}
	
}