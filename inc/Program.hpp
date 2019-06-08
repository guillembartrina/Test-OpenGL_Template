
#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <string>
#include <vector>

#include "GL/glew.h"

class Program
{
	public:
	
	Program();
	~Program();
	
	void setVS(const std::string& path, const std::vector<const char*>& names);
	void setFS(const std::string& path);
	
	void addUniforms(const std::vector<const char*>& names, std::vector<GLuint>& locations);
	
	GLuint getID() const;
	
	private:
	
	GLuint ID;
	GLuint vsID;
	GLuint fsID;
	
	//save uniforms?
	
	static char* readFile(const std::string& path, GLint& size); //reimplement??
	
};

#endif