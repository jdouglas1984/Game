#pragma once

#include "include\SDL2\glew.h"
#include <string>
#include <fstream>

class GLSLProgram
{
public:
	GLSLProgram();
	~GLSLProgram();
	void CompileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
	void LinkShaders();
	void AddAttribute(const std::string& attributeName);
	void UseProgram();
	void StopProgram();

private:
	int m_numAttributes;
	GLuint m_programID;;
	GLuint m_vertexShaderID;
	GLuint m_fragmentShaderID;
	void CompileShader(const std::string filePath, GLuint id);
};

