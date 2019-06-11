
#include <string>
#include <iostream>
#include <fstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"

#include "Camera.hpp"
#include "Program.hpp"

Camera c;

double posX, posY;

bool moving = false;

// void setFocus_Euler(float theta, float psi, float phi = 0.f);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//if (key == GLFW_KEY_A && action == GLFW_PRESS) moving = true;
	//if (key == GLFW_KEY_A && action == GLFW_RELEASE) moving = false;
	//if (key == GLFW_KEY_S && action == GLFW_PRESS) c.rotateX(30 * (3.14159265 / 180.0), false);
	
	if (key == GLFW_KEY_A && (action != GLFW_RELEASE)) c.rotateX(5 * (3.14159265 / 180.0), true);
	if (key == GLFW_KEY_D && (action != GLFW_RELEASE)) c.rotateX(-5 * (3.14159265 / 180.0), true);
	
	if (key == GLFW_KEY_W && (action != GLFW_RELEASE)) c.rotateY(5 * (3.14159265 / 180.0), true);
	if (key == GLFW_KEY_S && (action != GLFW_RELEASE)) c.rotateY(-5 * (3.14159265 / 180.0), true);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	/*
		double diffX = xpos - posX;
		if(moving) c.rotateX(diffX / 180.f);
		posX = xpos;
		
		double diffY = ypos - posY;
		if(moving) c.rotateY(diffY / 180.f );
		posY = ypos;
		*/
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	c.resize(width, height);
	glViewport(0, 0, width, height);
}

int main()
{
    GLFWwindow* window;

    if (!glfwInit()) exit(EXIT_FAILURE);

    window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
	
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);

    glfwMakeContextCurrent(window);
	glewInit();
	
	Program p;

    glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	
	glfwGetCursorPos(window, &posX, &posY);
	
	p.setVS("shaders/vs.vert", { "position" });
	p.setFS("shaders/fs.frag");
	
	std::vector<GLuint> locs(3);
	p.addUniforms({ "GT", "VT", "PT" }, locs);
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glm::vec3 triangle[12*3] = 
	{
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(-1, 1, 1),
		glm::vec3(-1, 1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(1, -1, -1),	
		glm::vec3(1, -1, -1),
		glm::vec3(1, 1, 1),
		glm::vec3(1, 1, -1),	
		glm::vec3(1, 1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(-1, -1, -1),
		glm::vec3(-1, -1, -1),
		glm::vec3(1, 1, -1),
		glm::vec3(-1, 1, -1),		
		glm::vec3(-1, 1, -1),
		glm::vec3(-1, -1, -1),
		glm::vec3(-1, -1, 1),	
		glm::vec3(-1, 1, -1),
		glm::vec3(-1, -1, 1),
		glm::vec3(-1, 1, 1),		
		glm::vec3(-1, 1, 1),
		glm::vec3(-1, 1, -1),
		glm::vec3(1, 1, -1),
		glm::vec3(-1, 1, 1),
		glm::vec3(1, 1, -1),
		glm::vec3(1, 1, 1),
		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, -1, -1),
		glm::vec3(-1, -1, 1)
	};
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindVertexArray(0);
	
	glm::mat4 gt(1.0);
	//gt = glm::rotate(gt, 0.5f, glm::vec3(0, 1, 0));
	//gt = glm::scale(gt, glm::vec3(0.5, 2, 2));
	
	c.setAs3PCOf(glm::vec3(-2, -2, -2), glm::vec3(2, 2, 2), OpticType::PERSPECTIVE, glm::vec3(1, 0, 0), 4.f, glm::vec3(0, 1, 0));
	
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glBindVertexArray(vao);
		p.useProgram();
		
		glUniformMatrix4fv(locs[0], 1, GL_FALSE, &gt[0][0]);
		glUniformMatrix4fv(locs[1], 1, GL_FALSE, c.getVM());
		glUniformMatrix4fv(locs[2], 1, GL_FALSE, c.getPM());
		
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
		
		glUseProgram(0);
		glBindVertexArray(0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
	
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	
    glfwTerminate();
    return 0;
}