#pragma once
#include "include\SDL2\glew.h"


class Sprite
{
public:
	Sprite();
	~Sprite();
	//Sprite(float x, float y, float width, float height);
	
	void RenderSprite();
	void InitSprite(float x, float y, float width, float height);

private:
	float m_x;
	float m_y;
	int m_width;
	int m_height;
	GLuint m_vboID;
	
};

