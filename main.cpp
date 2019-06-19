
#include <string>
#include <iostream>
#include <fstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"

#include "Camera.hpp"
#include "Program.hpp"
#include "model.hpp"
#include "Drawable.hpp"

Camera c1;
Camera c2;

int w, h;

bool moving = true;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
		moving = true;
	if (key == GLFW_KEY_Z && action == GLFW_RELEASE)
		moving = false;
}

static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
	int w, h;
	glfwGetWindowSize(window, &w, &h);

	double diffX = xpos - w / 2;
	if (moving)
		c1.rotateX_VRP(-diffX / 360.f);
	if (moving)
		c2.rotateX_VRP(-diffX / 360.f);

	double diffY = ypos - h / 2;
	if (moving)
		c1.rotateY_VRP(-diffY / 360.f);
	if (moving)
		c2.rotateY_VRP(-diffY / 360.f);

	glfwSetCursorPos(window, w / 2, h / 2);
}

void window_size_callback(GLFWwindow *window, int width, int height)
{
	w = width;
	h = height;

	c1.applyResize(width / 2, height);
	c2.applyResize(width / 2, height);
}

int main()
{
	GLFWwindow *window;

	if (!glfwInit())
		exit(EXIT_FAILURE);

	window = glfwCreateWindow(1000, 500, "OpenGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glfwMakeContextCurrent(window);

	glfwGetWindowSize(window, &w, &h);
	glfwSetCursorPos(window, w / 2, h / 2);

	glewInit();
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	Model m;

	Program p;

	p.loadShaders_FromFile("shaders/vs.vert", {"position", "normal", "ka", "kd", "ks", "n"}, "shaders/fs.frag");
	std::vector<GLuint> locs(5);
	p.addUniforms({"GT", "VT", "PT", "SCOfocus", "focus"}, locs);
	p.useProgram();

	Drawable d(locs[0]);

	m.load("objs/Test.obj");

	d.load_FromModel(&m, p.getID());

	c1.setFocus(glm::vec3(4.0, 0.0, 0.1), glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
	c2.setFocus(glm::vec3(4.0, 0.0, -0.1), glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));

	c1.setOptic_Perspective(90.f * (3.1415926535 / 180.0), 1.0, 0.1, 40.0);
	c2.setOptic_Perspective(90.f * (3.1415926535 / 180.0), 1.0, 0.1, 40.0);

	c1.applyResize(w / 2, h);
	c2.applyResize(w / 2, h);

	glUniform1i(locs[3], GL_FALSE);
	glm::vec3 tmp(-18.0, 1.0, 1.0);
	glUniform3fv(locs[4], 1, &tmp[0]);

	double lasttime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			c1.move(glm::vec3(0.0, 0.0, -0.1));
			c2.move(glm::vec3(0.0, 0.0, -0.1));
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			c1.move(glm::vec3(0.0, 0.0, 0.1));
			c2.move(glm::vec3(0.0, 0.0, 0.1));
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			c1.move(glm::vec3(-0.1, 0.0, 0.0));
			c2.move(glm::vec3(-0.1, 0.0, 0.0));
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			c1.move(glm::vec3(0.1, 0.0, 0.0));
			c2.move(glm::vec3(0.1, 0.0, 0.0));
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, w / 2, h);

		glUniformMatrix4fv(locs[1], 1, GL_FALSE, c1.getVM());
		glUniformMatrix4fv(locs[2], 1, GL_FALSE, c1.getPM());

		d.draw();

		glViewport(w / 2, 0, w / 2, h);

		glUniformMatrix4fv(locs[1], 1, GL_FALSE, c2.getVM());
		glUniformMatrix4fv(locs[2], 1, GL_FALSE, c2.getPM());

		d.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

		while (glfwGetTime() < lasttime + 1.0 / 60.0)
			;
		lasttime += 1.0 / 60.0;
	}

	glfwTerminate();
	return 0;
}