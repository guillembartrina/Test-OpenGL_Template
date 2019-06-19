
#ifndef APP_HPP
#define APP_HPP

#include <stdexcept>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"

#include "Program.hpp"
#include "Camera.hpp"
#include "Drawable.hpp"

class App
{
public:

    App(const char* name, unsigned int W, unsigned int H);
    ~App();

    void run();

private:

    GLFWwindow* window;

    unsigned int W, H;
    double timecount;

    Program* program;
    GLuint* locations;

    Camera cameraL, cameraR;

    std::vector<Drawable*> drawables;

    void update();

    void handleEvents();
    static void callback_key(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void callback_cursor(GLFWwindow *window, double xpos, double ypos);
    static void callback_resize(GLFWwindow *window, int width, int height);

    void draw() const;
};

#endif