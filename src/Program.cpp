
#include "Program.hpp"

#include <fstream>

Program::Program()
{
	ID = glCreateProgram();
	
	vsID = 0;
	fsID = 0;
}

Program::~Program()
{
	if(vsID != 0)
	{
		glDetachShader(ID, vsID);
		glDeleteShader(vsID);
	}
	
	if(fsID != 0)
	{
		glDetachShader(ID, fsID);
		glDeleteShader(fsID);
	}
	
	glDeleteProgram(ID);
}

void Program::setVS(const std::string& path, const std::vector<const char*>& names)
{
	if(vsID != 0)
	{
		glDetachShader(ID, vsID);
		glDeleteShader(vsID);
	}
	
	GLint len;
	const GLchar* str = readFile(path, len);
	
	vsID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsID, 1, &str, &len);
	glCompileShader(vsID);
	
	glAttachShader(ID, vsID);
	
	for(unsigned int i = 0; i < names.size(); i++)
	{
		glBindAttribLocation(ID, i, names[i]);
	}
	
	glLinkProgram(ID);
	glValidateProgram(ID);
	
	free((void*)str);
}

void Program::setFS(const std::string& path)
{
	if(fsID != 0)
	{
		glDetachShader(ID, fsID);
		glDeleteShader(fsID);
	}
	
	GLint len;
	const GLchar* str = readFile(path, len);
	
	fsID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsID, 1, &str, &len);
	glCompileShader(fsID);
	
	glAttachShader(ID, fsID);
	
	glLinkProgram(ID);
	glValidateProgram(ID);
	
	free((void*)str);
}

void Program::addUniforms(const std::vector<const char*>& names, std::vector<GLuint>& locations)
{
	for(unsigned int i = 0; i < names.size(); i++)
	{
		locations[i] = glGetUniformLocation(ID, names[i]);
	}
}

void Program::useProgram() const
{
	glUseProgram(ID);
}

char* Program::readFile(const std::string& path, GLint& size)
{
	char* string = nullptr;
	
	std::ifstream file(path);
	
	if(file.is_open())
	{
		file.seekg(0, std::ios::end);
		size = file.tellg();
		file.seekg(0);
		
		string = (char*)malloc(size + 1);
		file.read(string, size);
		
		file.close();
	}
	
	return string;
}