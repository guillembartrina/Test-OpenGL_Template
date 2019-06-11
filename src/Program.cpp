
#include "Program.hpp"

#include <fstream>
#include <iostream>
#include <vector>

Program::Program()
{
	ID = glCreateProgram();
	
	vs = false;
	fs = false;
}

Program::~Program()
{
	if(vsID)
	{
		glDetachShader(ID, vsID);
		glDeleteShader(vsID);
	}
	
	if(fsID)
	{
		glDetachShader(ID, fsID);
		glDeleteShader(fsID);
	}
	
	glDeleteProgram(ID);
}

void Program::setVS(const std::string& path, const std::vector<const char*>& names)
{
	if(vs)
	{
		glDetachShader(ID, vsID);
		glDeleteShader(vsID);
	}
	
	GLint len;
	const GLchar* str = readFile(path, len);
	
	vsID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsID, 1, &str, &len);
	glCompileShader(vsID);
	
	vs = !checkError(vsID);
	
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
	if(fs)
	{
		glDetachShader(ID, fsID);
		glDeleteShader(fsID);
	}
	
	GLint len;
	const GLchar* str = readFile(path, len);
	
	fsID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsID, 1, &str, &len);
	glCompileShader(fsID);
	
	fs = !checkError(fsID);
	
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

bool Program::checkError(GLuint id) const
{
	GLint compiled = GL_FALSE;;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
	if(compiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
		
		for(int i = 0; i < maxLength; i++) std::cerr << errorLog[i];

		glDeleteShader(id);
		
		return true;
	}
	
	return false;
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