
#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <string>
#include <vector>

#include "GL/glew.h"

class Program //NEEDS OPENGL CONTEXT!!!
{
public:
	Program();
	~Program();

	void loadShaders_FromFile(const std::string &vspath, const std::vector<const char *> &vsinnames, const std::string &fspath);
	GLuint* addUniforms(const std::vector<const char *> &names);

	void useProgram() const;
	GLuint getID() const;

private:
	GLuint ID;
	GLuint vsID;
	std::string vsSrc;
	GLuint fsID;
	std::string fsSrc;

	bool loaded;

	bool load_FromFile(const std::string &vspath, const std::string &fspath);
	bool compile();
	bool link();

	static bool readFile(const std::string &path, std::string &src);
};

#endif