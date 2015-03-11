#pragma once
#include <map>
#include "GLTexture.h"
class TextureCache
{
public:
	TextureCache();
	~TextureCache();

	GLTexture GetTexture(std::string texturePath);

private:
	std::map<std::string, GLTexture> m_textureMap;
};

