#include "GLSLProgram.h"
#include "Errors.h"
#include <vector>

//basic constructor
GLSLProgram::GLSLProgram() :
m_numAttributes(0), m_programID(0), m_vertexShaderID(0), m_fragmentShaderID(0)
{
}

GLSLProgram::~GLSLProgram()
{
}

GLuint GLSLProgram::GetUniformLocation(const std::string& uniformName)
{
	GLuint location = glGetUniformLocation(m_programID, uniformName.c_str());

	if (location == GL_INVALID_INDEX)
	{
		FatalError("Failed to get " + uniformName + " location");
	}

	return location;
}

//compiles vertex and fragment shaders passed in as well as creates the program for glUseProgram()
void GLSLProgram::CompileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
	//program for linked vertex and fragment shaders
	m_programID = glCreateProgram();
	//vars to store created shaders
	m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	//error check for shader creation
	if (m_vertexShaderID == 0)
	{
		FatalError("Failed to create vertex shader");
	}

	if (m_fragmentShaderID == 0)
	{
		FatalError("Failed to create fragment shader");
	}
	//calls compile shader function to compile and attach shaders to 
	//vars passed in
	CompileShader(vertexShaderFilePath, m_vertexShaderID);
	CompileShader(fragmentShaderFilePath, m_fragmentShaderID);
}

void GLSLProgram::LinkShaders()
{
	//Attach our shaders to our program
	glAttachShader(m_programID, m_vertexShaderID);
	glAttachShader(m_programID, m_fragmentShaderID);

	//Link our program
	glLinkProgram(m_programID);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(m_programID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);

		//We don't need the program anymore.
		glDeleteProgram(m_programID);
		//Don't leak shaders either.
		glDeleteShader(m_vertexShaderID);
		glDeleteShader(m_fragmentShaderID);

		std::printf("%s\n", &(errorLog[0]));
		FatalError("Shaders failed to link");
	}

	//Always detach shaders after a successful link.
	glDetachShader(m_programID, m_vertexShaderID);
	glDetachShader(m_programID, m_fragmentShaderID);
	glDeleteShader(m_vertexShaderID);
	glDeleteShader(m_fragmentShaderID);
}

void GLSLProgram::AddAttribute(const std::string& attributeName)
{
	glBindAttribLocation(m_programID, m_numAttributes++, attributeName.c_str());
}
//takes in path name and shader id var to be attached to
void GLSLProgram::CompileShader(const std::string filePath, GLuint id)
{
	//opens file to be compiled
	std::ifstream shaderFile(filePath);
	//error check for file
	if (shaderFile.fail())
	{
		perror(filePath.c_str());
		FatalError("Failed to open " + filePath);
	}
	//vars to store file being read in
	std::string fileContents = "";
	std::string line;
	//reads the file in and stores it in fileContents
	while (std::getline(shaderFile, line))
	{
		fileContents += line + "\n";
	}
	//close file 
	shaderFile.close();

	const char* contentsPtr = fileContents.c_str();
	glShaderSource(id, 1, &contentsPtr, nullptr);
	//compiles the shader that has been read in
	glCompileShader(id);
	//error check to see if the shader has compiled,
	//if not prints out an error message
	GLint isCompiled = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(id); // Don't leak the shader.

		std::printf("%s\n", &(errorLog[0]));
		FatalError("Shaders failed to compile");
	}
}
//called to use the given program
void GLSLProgram::UseProgram()
{
	glUseProgram(m_programID);
	for (int i = 0; i < m_numAttributes; i++)
	{
		glEnableVertexAttribArray(i);
	}
}
//stops the program from being used
void GLSLProgram::StopProgram()
{
	glUseProgram(0);
	for (int i = 0; i < m_numAttributes; i++)
	{
		glDisableVertexAttribArray(i);
	}
}

