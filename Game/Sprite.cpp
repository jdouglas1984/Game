#include "Sprite.h"
#include "Vertex.h"
#include "ResourceManager.h"

#include <cstddef>

Sprite::Sprite() :
m_vboID(0)
{
}


Sprite::~Sprite()
{
	if (m_vboID != 0)
	{
		glDeleteBuffers(1, &m_vboID);
	}
}

void Sprite::InitSprite(float x, float y, float width, float height, std::string texturePath)
{
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;

	m_texture = ResourceManager::getTexture(texturePath);

	//error check
	if (m_vboID == 0)
	{
		glGenBuffers(1, &m_vboID);
	}

	Vertex vertexData[6];
	//First triangle
	vertexData[0].SetPosition(x + width, y + height);
	vertexData[0].SetUV(1.0f, 1.0f);

	vertexData[1].SetPosition(x, y + height);
	vertexData[1].SetUV(0.0f, 1.0f);

	vertexData[2].SetPosition(x , y);
	vertexData[2].SetUV(0.0f, 0.0f);

	//Second triangle
	vertexData[3].SetPosition(x, y);
	vertexData[3].SetUV(0.0f, 0.0f);

	vertexData[4].SetPosition(x + width, y);
	vertexData[4].SetUV(1.0f, 0.0f);

	vertexData[5].SetPosition(x + width, y + height);
	vertexData[5].SetUV(1.0f, 1.0f);

	vertexData[4].SetColor(0,0,255,255);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::RenderSprite()
{
	glBindTexture(GL_TEXTURE_2D, m_texture.id);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

	glEnableVertexAttribArray(0);
	//position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	//color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}