
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

	void loadShaders_FromFile(const std::string& vspath, const std::vector<const char*>& vsinnames, const std::string& fspath);
	void addUniforms(const std::vector<const char*>& names, std::vector<GLuint>& locations);

	void useProgram() const;
	
	
	private:
	
	GLuint ID;
	GLuint vsID;
	std::string vsSrc;
	GLuint fsID;
	std::string fsSrc;

	bool loaded;	
	//save uniforms?

	bool load_FromFile(const std::string& vspath, const std::string& fspath);
	bool compile();
	bool linkAndValidate();
	
	static bool readFile(const std::string& path, std::string& src);
};

#endif