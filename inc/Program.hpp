
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
	
	void useProgram() const;
	
	
	private:
	
	GLuint ID;
	bool vs;
	GLuint vsID;
	bool fs;
	GLuint fsID;
	
	//save uniforms?
	
	bool checkError(GLuint id) const;
	
	static char* readFile(const std::string& path, GLint& size); //reimplement??
};

#endif