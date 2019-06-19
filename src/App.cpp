
#include "App.hpp"

#include <iostream>

App::App(const char* name, unsigned int W, unsigned int H)
{
    if(!glfwInit()) throw std::runtime_error("Failed to init GLFW");

    this->W = W;
    this->H = H;

    window = glfwCreateWindow(W, H, name, NULL, NULL);
    if (!window)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create window");
	}

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetCursorPos(window, W/2, H/2);

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, callback_key);
	glfwSetCursorPosCallback(window, callback_cursor);
	glfwSetWindowSizeCallback(window, callback_resize);

    glfwMakeContextCurrent(window);

    glewInit();
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //---

    program = new Program();

    program->loadShaders_FromFile("shaders/vs.vert", {"position", "normal", "ka", "kd", "ks", "n"}, "shaders/fs.frag");
    locations = program->addUniforms({"GT", "VT", "PT", "SCOfocus", "focus"});
    program->useProgram();

    glUniform1i(locations[3], GL_FALSE);
	glm::vec3 tmpVec3(-18.0, 1.0, 1.0);
	glUniform3fv(locations[4], 1, &tmpVec3[0]);

    //---

    cameraL.setFocus(glm::vec3(4.0, 0.0, 0.1), glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
    cameraR.setFocus(glm::vec3(4.0, 0.0, -0.1), glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));

    cameraL.setOptic_Perspective(90.f * (3.1415926535 / 180.0), 1.0, 0.1, 40.0);
    cameraR.setOptic_Perspective(90.f * (3.1415926535 / 180.0), 1.0, 0.1, 40.0);

    cameraL.applyResize(W/2, H);
    cameraR.applyResize(W/2, H);

    //---

    drawables.push_back(new Drawable(locations[0]));

    Model tmpMod;
    tmpMod.load("objs/Test.obj");

    drawables.back()->load_FromModel(&tmpMod, program->getID());
}

App::~App()
{
    for(unsigned int i = 0; i < drawables.size(); i++)
    {
        delete drawables[i];
    }

    delete locations;
    delete program;

    glfwDestroyWindow(window);
    glfwTerminate();
}

void App::run()
{
    timecount = glfwGetTime();

    while(!glfwWindowShouldClose(window))
    {
        update();
        handleEvents();
        draw();

        while (glfwGetTime() < (timecount + 1.0/60.0));
		timecount += 1.0/60.0;
    }
}

void App::update()
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraL.move(glm::vec3(0.0, 0.0, -0.1));
        cameraR.move(glm::vec3(0.0, 0.0, -0.1));
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraL.move(glm::vec3(0.0, 0.0, 0.1));
        cameraR.move(glm::vec3(0.0, 0.0, 0.1));
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraL.move(glm::vec3(-0.1, 0.0, 0.0));
        cameraR.move(glm::vec3(-0.1, 0.0, 0.0));
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraL.move(glm::vec3(0.1, 0.0, 0.0));
        cameraR.move(glm::vec3(0.1, 0.0, 0.0));
    }
}

void App::handleEvents()
{
    glfwPollEvents();
}

void App::callback_key(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
}

void App::callback_cursor(GLFWwindow *window, double xpos, double ypos)
{
    App* app = static_cast<App*>(glfwGetWindowUserPointer(window));

    double diffX = xpos - app->W/2;
    app->cameraL.rotateX_VRP(-diffX / 360.f);
    app->cameraR.rotateX_VRP(-diffX / 360.f);

    double diffY = ypos - app->H/2;
    app->cameraL.rotateY_VRP(-diffY / 360.f);
    app->cameraR.rotateY_VRP(-diffY / 360.f);

    glfwSetCursorPos(window, app->W/2, app->H/2);
}

void App::callback_resize(GLFWwindow *window, int width, int height)
{
    App* app = static_cast<App*>(glfwGetWindowUserPointer(window));

    app->W = width;
	app->H = height;

	app->cameraL.applyResize(width/2, height);
	app->cameraR.applyResize(width/2, height);
}

void App::draw() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, W/2, H);

    glUniformMatrix4fv(locations[1], 1, GL_FALSE, cameraL.getVM());
    glUniformMatrix4fv(locations[2], 1, GL_FALSE, cameraL.getPM());

    drawables.back()->draw();

    glViewport(W/2, 0, W/2, H);

    glUniformMatrix4fv(locations[1], 1, GL_FALSE, cameraR.getVM());
    glUniformMatrix4fv(locations[2], 1, GL_FALSE, cameraR.getPM());

    drawables.back()->draw();

    glfwSwapBuffers(window);
}