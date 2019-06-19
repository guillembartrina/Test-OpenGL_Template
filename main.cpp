
#include <string>
#include <iostream>
#include <fstream>

#include "App.hpp"

int main()
{
	App app("OpenGL", 1000, 500);

	try
	{
		app.run();
	}
	catch(std::runtime_error& re)
	{
		std::cerr << "ERROR: " << re.what() << std::endl;
	}
}