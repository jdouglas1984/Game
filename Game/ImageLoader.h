#pragma once

#include "GLTexture.h"
#include <string>

class ImageLoader
{
public:
	static GLTexture LoadPng(std::string filePath);
};

