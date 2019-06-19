
#include "Program.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

Program::Program()
{
	ID = glCreateProgram();

	loaded = false;
}

Program::~Program()
{
	if (loaded)
	{
		glDetachShader(ID, vsID);
		glDeleteShader(vsID);
		glDetachShader(ID, fsID);
		glDeleteShader(fsID);
	}

	glDeleteProgram(ID);
}

void Program::loadShaders_FromFile(const std::string &vspath, const std::vector<const char *> &vsinnames, const std::string &fspath)
{
	if (loaded)
	{
		glDetachShader(ID, vsID);
		glDeleteShader(vsID);
		glDetachShader(ID, fsID);
		glDeleteShader(fsID);
	}

	if (not load_FromFile(vspath, fspath))
		std::cerr << "Error loading shaders" << std::endl;
	if (not compile())
		std::cerr << "Error compiling shaders" << std::endl;

	for (unsigned int i = 0; i < vsinnames.size(); i++)
	{
		glBindAttribLocation(ID, i, vsinnames[i]);
	}

	if (not link())
		std::cerr << "Error compiling shaders" << std::endl;

	loaded = true;
}

GLuint* Program::addUniforms(const std::vector<const char *> &names)
{
	if (loaded)
	{
		GLuint* locations = (GLuint*)malloc(sizeof(GLuint)*names.size());

		for (unsigned int i = 0; i < names.size(); i++)
		{
			locations[i] = glGetUniformLocation(ID, names[i]);
		}

		return locations;
	}
	else std::cerr << "Adding uniforms while not loaded program" << std::endl;

	return nullptr;
}

void Program::useProgram() const
{
	if (loaded)
	{
		glUseProgram(ID);
	}
	else std::cerr << "Using program while not loaded program" << std::endl;
}

GLuint Program::getID() const
{
	return ID;
}

bool Program::load_FromFile(const std::string &vspath, const std::string &fspath)
{
	return (readFile(vspath, vsSrc) and readFile(fspath, fsSrc));
}

bool Program::compile()
{
	vsID = glCreateShader(GL_VERTEX_SHADER);

	const GLchar *srcVS = vsSrc.c_str();
	glShaderSource(vsID, 1, &srcVS, 0);
	glCompileShader(vsID);

	GLint compiled;
	glGetShaderiv(vsID, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE)
	{
		GLint maxLen;
		glGetShaderiv(vsID, GL_INFO_LOG_LENGTH, &maxLen);

		char *log = (char *)malloc(maxLen);
		glGetShaderInfoLog(vsID, maxLen, &maxLen, log);
		std::cerr << "VS compile error:\n"
				  << log << std::endl;
		free((void *)log);

		return false;
	}

	glAttachShader(ID, vsID);

	fsID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar *srcFS = fsSrc.c_str();
	glShaderSource(fsID, 1, &srcFS, 0);
	glCompileShader(fsID);

	glGetShaderiv(fsID, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE)
	{
		GLint maxLen;
		glGetShaderiv(fsID, GL_INFO_LOG_LENGTH, &maxLen);

		char *log = (char *)malloc(maxLen);
		glGetShaderInfoLog(fsID, maxLen, &maxLen, log);
		std::cerr << "FS compile error:\n"
				  << log << std::endl;
		free((void *)log);

		return false;
	}

	glAttachShader(ID, fsID);

	return true;
}

bool Program::link()
{
	glLinkProgram(ID);

	GLint linked;
	glGetProgramiv(ID, GL_LINK_STATUS, &linked);
	if (linked == GL_FALSE)
	{
		GLint maxLen;
		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &maxLen);

		char *log = (char *)malloc(maxLen);
		glGetProgramInfoLog(ID, maxLen, &maxLen, log);
		std::cerr << "link error:\n"
				  << log << std::endl;
		free((void *)log);

		return false;
	}

	glValidateProgram(ID);

	GLint validated;
	glGetProgramiv(ID, GL_VALIDATE_STATUS, &validated);
	if (validated == GL_FALSE)
	{
		GLint maxLen;
		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &maxLen);

		char *log = (char *)malloc(maxLen);
		glGetProgramInfoLog(ID, maxLen, &maxLen, log);
		std::cerr << "validate error:\n"
				  << log << std::endl;
		free((void *)log);

		return false;
	}

	return true;
}

bool Program::readFile(const std::string &path, std::string &src)
{
	src.clear();

	std::ifstream file(path);

	if (file.is_open())
	{
		std::stringstream ss;
		ss << file.rdbuf();
		src = ss.str();

		file.close();
	}
	else
		return false;

	return true;
}