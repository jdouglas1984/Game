#include "ImageLoader.h"
#include "picoPNG.h"
#include "IOManager.h"
#include "Errors.h"

GLTexture ImageLoader::LoadPng(std::string filePath)
{
	GLTexture texture = {};

	unsigned long width, height;

	std::vector<unsigned char> in;
	std::vector<unsigned char> out;

	if (IOManager::readFileToBuffer(filePath, in) == false)
	{
		FatalError("Failed to load PNG file");
	}

	int error = decodePNG(out, width, height, &(in[0]), in.size());

	if (error != 0)
	{
		FatalError("decodePNG failed with error: " + std::to_string(error));
	}
	//generate, bind and fill texture with image
	glGenTextures(1, &(texture.id));
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));
	//parameters for handling the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	glGenerateMipmap(GL_TEXTURE_2D);

	texture.width = width;
	texture.height = height;

	glBindTexture(0, GL_TEXTURE_2D);

	return texture;
}