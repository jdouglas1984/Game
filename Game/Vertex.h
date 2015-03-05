#pragma once
#include "include\SDL2\glew.h"

struct Position
{
	float x;
	float y;
};

struct Color
{
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};


struct Vertex
{
	Position position;
	Color color;
};