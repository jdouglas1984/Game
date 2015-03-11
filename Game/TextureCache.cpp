#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>


TextureCache::TextureCache()
{
}

TextureCache::~TextureCache()
{
}

GLTexture TextureCache::GetTexture(std::string texturePath)
{
	//look up texture to see if it is in the map
	auto mit = m_textureMap.find(texturePath);

	//check if it's not in the map
	if (mit == m_textureMap.end())
	{
		//load the texture
		GLTexture newTexture = ImageLoader::LoadPng(texturePath);
		//insert to map
		m_textureMap.insert(make_pair(texturePath, newTexture));

		std::cerr << "Loaded texture \n" << std::endl;

		return newTexture;
	}
	std::cerr << "Loaded cached texture \n" << std::endl;
	return mit->second;
}
