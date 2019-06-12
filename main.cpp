
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

Camera c1;
Camera c2;

Model m;

double posX, posY;

bool moving = false;

// void setFocus_Euler(float theta, float psi, float phi = 0.f);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_A && action == GLFW_PRESS) moving = true;
	if (key == GLFW_KEY_A && action == GLFW_RELEASE) moving = false;
	//if (key == GLFW_KEY_S && action == GLFW_PRESS) c.rotateX(30 * (3.14159265 / 180.0), false);
	
	//if (key == GLFW_KEY_A && (action != GLFW_RELEASE)) c.rotateX(5 * (3.14159265 / 180.0), true);
	//if (key == GLFW_KEY_D && (action != GLFW_RELEASE)) c.rotateX(-5 * (3.14159265 / 180.0), true);
	
	//if (key == GLFW_KEY_W && (action != GLFW_RELEASE)) c.rotateY(5 * (3.14159265 / 180.0), true);
	//if (key == GLFW_KEY_S && (action != GLFW_RELEASE)) c.rotateY(-5 * (3.14159265 / 180.0), true);
	
	//if (key == GLFW_KEY_Z && (action != GLFW_RELEASE)) c.rotateZ(5 * (3.14159265 / 180.0), true);
	//if (key == GLFW_KEY_X && (action != GLFW_RELEASE)) c.rotateZ(-5 * (3.14159265 / 180.0), true);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
		double diffX = xpos - posX;
		if(moving) c1.rotateX_VRP(diffX / 360.f);
		if(moving) c2.rotateX_VRP(diffX / 360.f);
		posX = xpos;
		
		double diffY = ypos - posY;
		if(moving) c1.rotateY_VRP(diffY / 360.f);
		if(moving) c2.rotateY_VRP(diffY / 360.f);
		posY = ypos;
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	c1.applyResize(width/2.f, height);
	c2.applyResize(width/2.f, height);
}

void error_callback(int error, const char* description)
{
    std::cerr << "Error: " << description << std::endl;
}

int main()
{
    GLFWwindow* window;

    if (!glfwInit()) exit(EXIT_FAILURE);

    window = glfwCreateWindow(800, 400, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
	
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetErrorCallback(error_callback);

    glfwMakeContextCurrent(window);
	glewInit();
	
	Program p;

    glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	
	glfwGetCursorPos(window, &posX, &posY);
	
	p.setVS("shaders/vs.vert", { "position", "normal", "ka", "kd", "ks", "n" });
	p.setFS("shaders/fs.frag");
	
	std::vector<GLuint> locs(3);
	p.addUniforms({ "GT", "VT", "PT" }, locs);
	
	m.load("objs/box.obj");
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	GLuint vbo[6];
	
	glGenBuffers(6, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_vertices(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_normals(), GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matamb(), GL_STATIC_DRAW);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matdiff(), GL_STATIC_DRAW);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matspec(), GL_STATIC_DRAW);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3, m.VBO_matshin(), GL_STATIC_DRAW);

	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(5);

	glBindVertexArray(0);
	
	/*
	
	glGenBuffers(6, &vbo);
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
	
	*/
	
	glm::mat4 gt(1.0);
	//gt = glm::rotate(gt, 0.5f, glm::vec3(0, 1, 0));
	//gt = glm::scale(gt, glm::vec3(0.5, 2, 2));
	gt = glm::scale(gt, glm::vec3(0.1, 0.1, 0.1));
	
	c1.setAs3PCOf(glm::vec3(-3, -3, -3), glm::vec3(3, 3, 3), OpticType::PERSPECTIVE, glm::vec3(1, 0, 0), 8.f, glm::vec3(0, 1, 0));
	c2.setAs3PCOf(glm::vec3(-3, -3, -3), glm::vec3(3, 3, 3), OpticType::PERSPECTIVE, glm::vec3(1, 0, 0), 8.f, glm::vec3(0, 1, 0));

	c1.move_OBS(glm::vec3(0, 0, 0.2), true);
	c2.move_OBS(glm::vec3(0, 0, -0.2), true);

	
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		/*
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cerr << "OpenGL error: " << err << std::endl;
		}
		*/
		
		glBindVertexArray(vao);
		p.useProgram();

		glViewport(0, 0, 400, 400);
		
		glUniformMatrix4fv(locs[0], 1, GL_FALSE, &gt[0][0]);
		glUniformMatrix4fv(locs[1], 1, GL_FALSE, c1.getVM());
		glUniformMatrix4fv(locs[2], 1, GL_FALSE, c1.getPM());
		
		glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);

		glViewport(400, 0, 400, 400);
		
		glUniformMatrix4fv(locs[0], 1, GL_FALSE, &gt[0][0]);
		glUniformMatrix4fv(locs[1], 1, GL_FALSE, c2.getVM());
		glUniformMatrix4fv(locs[2], 1, GL_FALSE, c2.getPM());
		
		glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);
		
		glUseProgram(0);
		glBindVertexArray(0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
	
	glDeleteBuffers(6, vbo);
	glDeleteVertexArrays(1, &vao);
	
    glfwTerminate();
    return 0;
}